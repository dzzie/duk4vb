VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CDukTape"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'Author: David Zimmer <dzzie@yahoo.com>
'Site: Sandsprite.com
'License: http://opensource.org/licenses/MIT

Option Explicit
Private hDuk As Long
Private mTimeout As Long
Private mRunning As Boolean
Private mDebugging As Boolean
Private m_comTypes As New Collection
Private m_objs As New Collection

'Error handling same as MSScript control. both with an event
'and an err.raise. You must use On Error handling when using this
'class. (Its easier than all the if duk.AddObject tests.)
Public hadError As Boolean
Public LastError As String
Public LastErrorLine As Long
Public userCOMDir As String

Event Error(ByVal line As Long, ByVal desc As String)
Event dbgOut(msg As String)
Event printOut(msg As String)

Friend Property Get comTypes() As Collection
    Set comTypes = m_comTypes
End Property

Friend Property Get objs() As Collection
    Set objs = m_objs
End Property

Friend Sub doDbgOut(msg As String)
    RaiseEvent dbgOut(msg)
End Sub

Friend Sub doPrintOut(msg As String)
    RaiseEvent printOut(msg)
End Sub

Property Get running() As Boolean
    running = mRunning
End Property

Public Property Get isDebugging() As Boolean
    isDebugging = mDebugging
End Property

'only visible within our ocx because it request special integration
'and whole point of the ocx is to hide complexity and make it easy to use.
Friend Function DebugAttach(Optional attach As Boolean = True) As Boolean
    If attach Then
        If mDebugging Then Exit Function
        'If Not ActiveDebuggerClass Is Nothing Then Exit Function
        If mTimeout <> 0 Then DukOp opd_ScriptTimeout, 0, 0
        If DukOp(opd_debugAttach, hDuk, 1) = 0 Then
            DebugAttach = True
            mDebugging = True
            'Set ActiveDebuggerClass = Me
        Else
            If mTimeout <> 0 Then DukOp opd_ScriptTimeout, 0, mTimeout
        End If
    Else
        If Not mDebugging Then Exit Function
        'If ObjPtr(ActiveDebuggerClass) <> ObjPtr(Me) Then Exit Function
        If mTimeout <> 0 Then DukOp opd_ScriptTimeout, 0, mTimeout
        If DukOp(opd_debugAttach, hDuk, 0) = 0 Then
            DebugAttach = True
            mDebugging = False
            'Set ActiveDebuggerClass = Nothing
        End If
    End If
End Function

Property Let Timeout(v As Long)
    mTimeout = v
    DukOp opd_ScriptTimeout, 0, v 'still required for immediate kills
End Property

Property Get Timeout() As Long
    Timeout = mTimeout
End Property

Property Get context() As Long
    context = hDuk
End Property

Private Sub Class_Initialize()
    If mDuk.hDukLib = 0 Then
        If Not InitDukLib() Then
            hadError = True
            LastError = "Library not initilized"
            RaiseEvent Error(LastErrorLine, LastError)
            Exit Sub
        End If
    End If
    hDuk = DukCreate()
    mDuk.libRefCnt = mDuk.libRefCnt + 1
End Sub

Private Sub Class_Terminate()
    ResetComObjects
    If hDuk <> 0 Then DukOp opd_Destroy, hDuk
    mDuk.libRefCnt = mDuk.libRefCnt - 1
    'If IsIde() And mDuk.libRefCnt = 0 Then
        'in the IDE we release it as soon as not needed..that way IDE doesnt
        'ever hang onto the dll after debugging. If it does we cant recompile the dll
        'without closing VB IDE which is annoying...
        'FreeLibrary mDuk.hDukLib '<--to dangerous to use after initial dev of dll is done..
    'End If
End Sub

Function Reset() As Boolean
    
    'invalid while running!
    If mDebugging Or mRunning Then
        Reset = False
        Exit Function
    End If
    
    ClearErr
    Class_Terminate
    Class_Initialize
    Reset = True
    
End Function

Private Function ParseObjectToCache(name As String, obj As Object) As Boolean
    
    Dim cc As CCOMType
    
    If KeyExistsInCollection(m_comTypes, name) Then
        Set cc = m_comTypes(name)
        If cc.errors.count = 0 Then ParseObjectToCache = True
        Exit Function
    End If
        
    If Not obj Is Nothing Then m_objs.Add obj, name 'some types arent creatable/top level and are retvals
    
    Set cc = New CCOMType
    Set cc.owner = Me
    ParseObjectToCache = cc.LoadType(name)
    m_comTypes.Add cc, name
    
End Function

Private Sub ResetComObjects()

    Dim o As Object
    For Each o In m_comTypes
        Set o = Nothing
    Next
    
    For Each o In m_objs
        Set o = Nothing
    Next
    
    Set m_comTypes = New Collection
    Set m_objs = New Collection
    
End Sub

Private Function comTypeExists(name) As Boolean
    On Error Resume Next
    Dim dd As CCOMType
    Set dd = m_comTypes(CStr(name))
    If Err.Number = 0 Then comTypeExists = True
End Function

Private Sub ClearErr()
    hadError = False
    LastError = Empty
    LastErrorLine = 0
End Sub

Function AddObject(obj As Object, name As String) As Boolean
    
    Dim d
    Dim cc As CCOMType
    Dim result As Boolean
    
    ClearErr
    result = ParseObjectToCache(name, obj)
    Set cc = comTypes(name) 'always added even with errors
       
    If Not result Then
       SetError "AddObject", c2s(cc.errors)
       Exit Function
    End If
        
    If Not Me.AddFile(cc.js_file) Then
        SetError "AddObject", "Error loading js File: " & cc.js_file & " " & LastError
        Exit Function
    End If
        
    For Each d In cc.dependancies
        If Not comTypeExists(d) Then
            If Not AddObject(Nothing, CStr(d)) Then
                Exit Function
            End If
        End If
    Next
    
    AddObject = True
    
End Function

Function AddFile(fpath) As Boolean
    Dim rv As Long
    Dim tmp As String
    
    If hDuk = 0 Then Exit Function
    ClearErr

    debugPrint "ocx: addfile start time: " & fpath
    mDuk.startTime = Now
    mRunning = True
    Set ActiveDukTapeClass = Me
    rv = mDuk.AddFile(hDuk, CStr(fpath))
    mRunning = False
    Set ActiveDukTapeClass = Nothing
    
    If rv = 0 Then
        AddFile = True
        Exit Function
    End If
    
    SetError "AddFile", GetLastString()
    
End Function

Function Eval(js)
    Dim rv As Long
    Dim tmp As String
    
    If hDuk = 0 Then Exit Function
    ClearErr
    
    debugPrint "ocx: eval start time sz:" & Len(js) & " " & Left(js, 10) & "..."
    mDuk.startTime = Now
    mRunning = True
    Set ActiveDukTapeClass = Me
    rv = mDuk.Eval(hDuk, CStr(js))
    Set ActiveDukTapeClass = Nothing
    mRunning = False
    
    If rv = 0 Then
        Eval = GetLastString()
        Exit Function
    End If
    
    SetError "Eval", GetLastString()
    
End Function

 
Private Sub SetError(from As String, msg As String)

    Dim rv As Long, tmp
    Dim b As Long
    
    hadError = True
    LastError = msg
   
    rv = InStr(LastError, "(line")
    If rv > 0 Then
        rv = rv + 5
        b = InStr(rv, LastError, ")")
        If b > 0 Then
            tmp = Trim(Mid(LastError, rv, b - rv))
            If IsNumeric(tmp) Then LastErrorLine = CLng(tmp)
        End If
        If CountOccurances(msg, "(line") > 1 Then 'this handles double line number bug from errCreate standardization..lazy but reliable..
            rv = InStrRev(msg, "(line")
            If rv > 0 Then
                LastError = Trim(Mid(LastError, 1, rv - 1))
            End If
        End If
    End If
    
    'this emulates the behavior of the msscript control..
    RaiseEvent Error(LastErrorLine, from & ": " & LastError)
    Err.Raise 21, "Eval", from & ": " & LastError

End Sub

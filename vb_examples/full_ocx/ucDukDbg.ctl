VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{2668C1EA-1D34-42E2-B89F-6B92F3FF627B}#5.0#0"; "scivb2.ocx"
Begin VB.UserControl ucDukDbg 
   ClientHeight    =   7560
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   13950
   ForwardFocus    =   -1  'True
   ScaleHeight     =   7560
   ScaleWidth      =   13950
   ToolboxBitmap   =   "ucDukDbg.ctx":0000
   Begin sci2.SciSimple scivb 
      Height          =   5730
      Left            =   45
      TabIndex        =   6
      Top             =   540
      Width           =   10905
      _ExtentX        =   19235
      _ExtentY        =   10107
   End
   Begin VB.Timer tmrHideCallTip 
      Enabled         =   0   'False
      Interval        =   600
      Left            =   7920
      Top             =   90
   End
   Begin VB.Frame fraCmd 
      Height          =   600
      Left            =   45
      TabIndex        =   0
      Top             =   6390
      Width           =   13155
      Begin VB.TextBox txtCmd 
         BeginProperty Font 
            Name            =   "System"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Left            =   630
         TabIndex        =   1
         Top             =   180
         Width           =   12255
      End
      Begin VB.Label Label1 
         Caption         =   "duk>"
         BeginProperty Font 
            Name            =   "System"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Left            =   90
         TabIndex        =   2
         Top             =   225
         Width           =   510
      End
   End
   Begin VB.Timer tmrSetStatus 
      Enabled         =   0   'False
      Interval        =   700
      Left            =   9810
      Top             =   135
   End
   Begin MSComctlLib.ImageList ilToolbars_Disabled 
      Left            =   9135
      Top             =   0
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   15
      ImageHeight     =   15
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   10
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0312
            Key             =   "Run"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":041E
            Key             =   "Break"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":052A
            Key             =   "Run to Cursor"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0636
            Key             =   "Clear All Breakpoints"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0740
            Key             =   "Step Over"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":084C
            Key             =   "Step Out"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0958
            Key             =   "Step In"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0A64
            Key             =   "Stop"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0B70
            Key             =   "Start Debugger"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0C7A
            Key             =   "Toggle Breakpoint"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList ilToolbar 
      Left            =   8460
      Top             =   0
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   15
      ImageHeight     =   15
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   11
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0D84
            Key             =   "Run"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0E90
            Key             =   "Start Debugger"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":0F9A
            Key             =   "Break"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":10A4
            Key             =   "Stop"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":11AE
            Key             =   "Toggle Breakpoint"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":12B8
            Key             =   "Clear All Breakpoints"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":13C2
            Key             =   "Step In"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":14CC
            Key             =   "Step Over"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":15D6
            Key             =   "Step Out"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":16E0
            Key             =   "Run to Cursor"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ucDukDbg.ctx":17EA
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.Toolbar tbarDebug 
      Height          =   330
      Left            =   60
      TabIndex        =   3
      Top             =   60
      Width           =   3870
      _ExtentX        =   6826
      _ExtentY        =   582
      ButtonWidth     =   609
      ButtonHeight    =   582
      Style           =   1
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   13
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Run"
            Object.ToolTipText     =   "Run"
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Start Debugger"
            Object.ToolTipText     =   "Start Debugger"
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Break"
            Object.ToolTipText     =   "Break"
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Stop"
            Object.ToolTipText     =   "Stop"
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Toggle Breakpoint"
            Object.ToolTipText     =   "Toggle Breakpoint"
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Clear All Breakpoints"
            Object.ToolTipText     =   "Clear All Breakpoiunts"
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Step In"
            Object.ToolTipText     =   "Step In"
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Step Over"
            Object.ToolTipText     =   "Step Over"
         EndProperty
         BeginProperty Button11 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Step Out"
            Object.ToolTipText     =   "Step Out"
         EndProperty
         BeginProperty Button12 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Run to Cursor"
            Object.ToolTipText     =   "Run to Cursor"
         EndProperty
         BeginProperty Button13 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
      EndProperty
   End
   Begin VB.Label lblInfo 
      Height          =   330
      Left            =   8685
      TabIndex        =   5
      Top             =   90
      Width           =   5010
   End
   Begin VB.Label lblStatus 
      Caption         =   "Status: Idle"
      Height          =   375
      Left            =   4005
      TabIndex        =   4
      Top             =   90
      Width           =   4560
   End
End
Attribute VB_Name = "ucDukDbg"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'Author: David Zimmer <dzzie@yahoo.com>
'Site: Sandsprite.com
'License: http://opensource.org/licenses/MIT

'really we should compile scivb into this ocx so that we can make it public and share it
'as SciSimple from the sci property instead of having to use object...and cant set = type latter :(

Option Explicit

Private WithEvents duk As CDukTape
Attribute duk.VB_VarHelpID = -1
 
Const SC_MARK_CIRCLE = 0
Const SC_MARK_ARROW = 2
Const SC_MARK_BACKGROUND = 22
'http://www.scintilla.org/aprilw/SciLexer.bas
 
Private lastEIP As Long 'used for clearing last line highlight
Private curFile As String
Private userStop As Boolean

Public userCOMDir As String

Public Enum dbgStates
    dsStarted = 0
    dsIdle = 1
    dsPaused = 2
    dsRunning = 3
End Enum
    
Event printOut(msg As String)
Event dbgOut(msg As String)
Event dukErr(line As Long, msg As String)
Event StateChanged(state As dbgStates)

'we cache these per instance and add them to a fresh script engine instance each execution..
Private objCache As New Collection
Private libFiles As New Collection
Private intellisense As New Collection
Private WithEvents ownerForm As Form
Attribute ownerForm.VB_VarHelpID = -1
Private m_DbgState As dbgStates
Private isInitilized As Boolean
Private m_timeout As Long 'in seconds, 0 by default

Property Get Timeout() As Long
    Timeout = m_timeout
End Property

Property Let Timeout(v As Long)
    m_timeout = v
    If Not duk Is Nothing Then duk.Timeout = v
End Property

'use this to get full access to the editor:
'  dim txtjs withevents as sci2.SciSimple
'  set txtjs = dukdbg.sci
Public Property Get sci() As Object
    Set sci = scivb
End Property

Property Get Status() As CStatus
    Set Status = mDuk.Status
End Property

Property Get DebugState() As dbgStates
    DebugState = m_DbgState
End Property

Property Get CurrentFile() As String
    CurrentFile = curFile
End Property

'this only seems to work when debugging..not free run..how to abort execution then?
Public Sub StopExecution()
    If AmIActive(Me) And running Then
        userStop = True
        duk.Timeout = 1
        SendDebuggerCmd dc_stepInto
    End If
End Sub

'note this does not reset the running script..thats up to the user based on state..
Public Sub Reset(Optional objs As Boolean = False, Optional libs As Boolean = False, Optional itsense As Boolean = False)
    Dim o As CCachedObj
    
    If objs Then
        For Each o In objCache
            Set o = Nothing
        Next
        Set objCache = New Collection
    End If
    
    If itsense Then
        For Each o In intellisense
            Set o = Nothing
        Next
        Set intellisense = New Collection
    End If
    
    If libs Then Set libFiles = New Collection
    
End Sub

Function AddIntellisense(className As String, ByVal spaceSeperatedMethodList As String) As Boolean
    
    If Len(className) = 0 Or InStr(className, " ") > 1 Then Exit Function
    If Len(spaceSeperatedMethodList) = 0 Then Exit Function
    
    If InStr(spaceSeperatedMethodList, ",") > 0 Then
        spaceSeperatedMethodList = Join(Split(spaceSeperatedMethodList, ","), " ")
    End If
    
    Dim it As CIntellisenseItem
    
    For Each it In intellisense
        If it.objName = className Then Exit Function
    Next
    
    Set it = New CIntellisenseItem
    it.objName = className
    it.methods = spaceSeperatedMethodList
    intellisense.Add it
    AddIntellisense = True
    
End Function

Function LoadCallTips(fpath As String) As Long
    If Not FileExists(fpath) Then Exit Function
    LoadCallTips = scivb.LoadCallTips(fpath)
End Function

'only have to configure this once per instance unless you reset
Public Function AddObject(obj As Object, name As String) As Boolean
    Dim o As CCachedObj
    
    If running Then Exit Function
    
    For Each o In objCache
        If o.name = name Then Exit Function
    Next
    
    Set o = New CCachedObj
    Set o.obj = obj
    o.name = name
    objCache.Add o
    AddObject = True
    
End Function

'only have to configure this once per instance unless you reset
'note user can not step into lib file source..(my design choice for simplicity of use)
Public Function AddLibFile(fpath As String) As Boolean
    
    Dim f
    
    If running Then Exit Function
    
    If Not FileExists(fpath) Then Exit Function
    
    For Each f In libFiles
        If LCase(f) = LCase(fpath) Then Exit Function
    Next

    libFiles.Add fpath
    AddLibFile = True
    
End Function

Public Function GetCallStack() As Collection
    If Not CanIBeActiveInstance(Me) Then Exit Function
    If Not running Then GoTo fail
    If duk Is Nothing Then GoTo fail
    If Not duk.isDebugging Then GoTo fail
    If InStr(1, lblStatus.Caption, "Paused") < 1 Then GoTo fail
    Set GetCallStack = SyncGetCallStack()
    Exit Function
fail:
    Set GetCallStack = New Collection
End Function

Friend Property Get context() As Long
    If duk Is Nothing Then
        context = 0
    Else
        context = duk.context
    End If
End Property

Friend Property Get duktape() As CDukTape
    Set duktape = duk
End Property



Friend Sub SetStatus(state As dbgStates)

    m_DbgState = dsPaused
    RaiseEvent StateChanged(state)
    
    Dim msg()
    msg = Array("Running", "Idle", "Paused", "Running")
    
    If state = dsRunning Then
        'to eliminate flicker from fast changes when debugging..
        tmrSetStatus.Enabled = True
    Else
        lblStatus.Caption = "Status: " & msg(state)
        tmrSetStatus.Enabled = False
    End If
    
End Sub

Function LoadFile(fpath As String) As Boolean
    If Not FileExists(fpath) Then Exit Function
    curFile = fpath
    LoadFile = scivb.LoadFile(fpath)
End Function

Friend Sub SyncUI()
       
    Dim curline As Long
    
    If Len(Status.FileName) = 0 Or Status.FileName <> curFile Then Exit Sub
    
    ClearLastLineMarkers
    
    curline = Status.lineNumber - 1
    scivb.SetMarker curline, 1
    scivb.SetMarker curline, 3
    lastEIP = curline
    
    scivb.GotoLine curline
    scivb.SetFocus
    UserControl.Refresh
    DoEvents

End Sub
 


Friend Sub ClearLastLineMarkers()
    Dim startPos As Long, endPos As Long

    scivb.DeleteMarker lastEIP, 1 'remove the yellow arrow
    scivb.DeleteMarker lastEIP, 3 'remove the yellow line backcolor

    'force a refresh of the specified line or it might not catch it..
    startPos = scivb.PositionFromLine(lastEIP)
    endPos = scivb.PositionFromLine(lastEIP + 1)
    scivb.DirectSCI.Colourise startPos, endPos
    
End Sub

Friend Sub duk_dbgOut(msg As String)
    RaiseEvent dbgOut(msg)
End Sub

Friend Sub duk_Error(ByVal line As Long, ByVal desc As String)
    RaiseEvent dukErr(line, desc)
End Sub

Friend Sub duk_printOut(msg As String)
    RaiseEvent printOut(msg)
End Sub



Private Sub scivb_AutoCompleteEvent(className As String)
    Dim prev As String
    Dim it As CIntellisenseItem
    
    prev = scivb.PreviousWord
    
    For Each it In intellisense
        If it.objName = className Or prev = it.objName Then
            scivb.ShowAutoComplete it.methods
            Exit Sub
        End If
    Next
            
End Sub

Private Sub scivb_DoubleClick()
    Dim word As String
    
    word = scivb.CurrentWord
    If Len(word) < 20 Then
        lblInfo.Caption = "  " & scivb.hilightWord(word, , vbBinaryCompare) & " instances of '" & word & " ' found"
    End If
    
End Sub

Private Sub scivb_MouseUp(Button As Integer, Shift As Integer, X As Long, Y As Long)
    On Error Resume Next
    
    Dim sel As String
 
    sel = scivb.SelText
    If Len(sel) > 0 And Len(sel) < 20 Then
        lblInfo.Caption = "  " & scivb.hilightWord(sel, , vbBinaryCompare) & " instances of '" & sel & " ' found"
    End If
    
End Sub

Private Sub scivb_KeyDown(KeyCode As Long, Shift As Long)

    Dim curline As Long
    
    If Not CanIBeActiveInstance(Me) Then Exit Sub
    
    'Debug.Print KeyCode & " " & Shift
    Select Case KeyCode
        Case vbKeyF2:
                      If Len(curFile) = 0 Then curFile = GetFreeFileName(Environ("temp"), ".js")
                      curline = scivb.CurrentLine
                      ToggleBreakPoint curFile, curline, scivb.GetLineText(curline), Me
                      
        Case vbKeyF5: If running Then SendDebuggerCmd dc_Resume Else ExecuteScript True
        Case vbKeyF7: SendDebuggerCmd dc_stepInto
        Case vbKeyF8: SendDebuggerCmd dc_StepOver
        Case vbKeyF9: SendDebuggerCmd dc_stepout
    End Select

End Sub

Private Sub tbarDebug_ButtonClick(ByVal Button As MSComctlLib.Button)
    Dim curline As Long
    Dim txt As String
    
    If Not CanIBeActiveInstance(Me) Then
        RaiseEvent printOut("Only one instance can be debugging at a time.")
        Exit Sub
    End If
    
    Select Case Button.key
        Case "Run":               debugClear: If running Then SendDebuggerCmd dc_Resume Else ExecuteScript
        Case "Start Debugger":    debugClear: If running Then SendDebuggerCmd dc_Resume Else ExecuteScript True
        Case "Step In":           SendDebuggerCmd dc_stepInto
        Case "Step Over":         SendDebuggerCmd dc_StepOver
        Case "Step Out":          SendDebuggerCmd dc_stepout
        Case "Clear All Breakpoints": RemoveAllBreakpoints Me
        Case "Break":                 SyncPauseExecution

        Case "Run to Cursor":
                                  curline = scivb.CurrentLine
                                  txt = scivb.GetLineText(curline)
                                  If Not isExecutableLine(txt) Then
                                        doOutput "Can not run to cursor: not executable line"
                                  Else
                                        Status.StepToLine = curline + 1
                                        SendDebuggerCmd dc_stepInto
                                  End If
                                  
        Case "Toggle Breakpoint":
                                  If Len(curFile) = 0 Then curFile = GetFreeFileName(Environ("temp"), ".js")
                                  curline = scivb.CurrentLine
                                  ToggleBreakPoint curFile, curline, scivb.GetLineText(curline), Me
                                    
        Case "Stop":
                                  debugPrint "stop isDebugging: " & duk.isDebugging
                                  If duk.isDebugging Then
                                        userStop = True
                                        duk.Timeout = -1
                                        SendDebuggerCmd dc_stepInto
                                  Else
                                        DukOp opd_ForceQuit
                                  End If
                                  
                        
        
    End Select
    
End Sub



'this is important dont forget it!
Private Sub ownerForm_Unload(Cancel As Integer)
    If Me.isRunning Then
        MsgBox "You must stop execution before closing this form"
        Cancel = 1
        Exit Sub
    End If
End Sub

Private Sub ExecuteScript(Optional withDebugger As Boolean)
 
    Dim rv, f
    Dim o As CCachedObj
    Dim c As New Collection
    
    On Error GoTo cleanup
    
    If isControlActive() Then
        MsgBox "Another debugger instance is already running", vbInformation
        Exit Sub
    End If
    
    If Len(curFile) = 0 Then curFile = GetFreeFileName(Environ("temp"), ".js")
    
    SetStatus dsStarted
    running = True
    scivb.LockEditor
    SetToolBarIcons
    lblStatus = "Status: " & IIf(withDebugger, "Debugging...", "Running...")
    
    userStop = False
    Set duk = New CDukTape
    Set ActiveUserControl = Me
    Set RecvBuffer = New CWriteBuffer 'this resets our flags like firstMessage and bpInitilized...
    duk.userCOMDir = userCOMDir
    
    For Each o In objCache
         duk.AddObject o.obj, o.name
    Next
    
    For Each f In libFiles
        duk.AddFile f
    Next

    If withDebugger Then
        duk.Timeout = 0
        duk.DebugAttach
    Else
        duk.Timeout = m_timeout '7000 'set to 0 to disabled
    End If
     
    If Len(curFile) = 0 Then curFile = GetFreeFileName(Environ("temp"), ".js")
    WriteFile curFile, scivb.Text
    rv = duk.AddFile(curFile)
    
cleanup:
     
    If withDebugger Then duk.DebugAttach False
    If duk.hadError And Not userStop Then doOutput duk.LastError
    duk.Reset 'remove any live COM object references
    Set duk = Nothing
    
    SetStatus dsIdle 'these could call form_load again if closing down..
    running = False
    SetToolBarIcons
    ClearLastLineMarkers
    scivb.LockEditor False
    Set ActiveUserControl = Nothing
    
End Sub

Private Sub SetToolBarIcons(Optional forceDisable As Boolean = False)
    Dim b As Button
    
    If forceDisable Then
        For Each b In tbarDebug.Buttons
            b.Enabled = False
        Next
        Set tbarDebug.ImageList = Nothing
        Set tbarDebug.ImageList = ilToolbars_Disabled
        Exit Sub
    End If
    
    Set tbarDebug.ImageList = Nothing
    Set tbarDebug.ImageList = IIf(running, ilToolbar, ilToolbars_Disabled)
    
    For Each b In tbarDebug.Buttons
        If Len(b.key) > 0 Then
            b.Image = b.key
            b.ToolTipText = b.key
            If b.key <> "Run" And b.key <> "Start Debugger" And InStr(b.key, "Breakpoint") < 1 Then
                b.Enabled = running
            End If
        End If
    Next
    
End Sub


 
Private Sub UserControl_Initialize()
    SetToolBarIcons
    m_timeout = 0 'now we can force quit scripts any time so just disable by default
    DBG_EXT_HIDE_ERROR = True
    'DBG_PRINT_EXTERNAL = True 'can set at runtime with remote=1 in duk> prompt
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
    'only do this once at runtime startup (this will also fire at design time in IDE)
    'we dont want to run a subclass while in IDE
    If Ambient.UserMode And Not isInitilized Then
        scivb.DirectSCI.HideSelection False
        scivb.DirectSCI.MarkerDefine 2, SC_MARK_CIRCLE
        scivb.DirectSCI.MarkerSetFore 2, vbRed 'set breakpoint color
        scivb.DirectSCI.MarkerSetBack 2, vbRed
    
        scivb.DirectSCI.MarkerDefine 1, SC_MARK_ARROW
        scivb.DirectSCI.MarkerSetFore 1, vbBlack 'current eip
        scivb.DirectSCI.MarkerSetBack 1, vbYellow
    
        scivb.DirectSCI.MarkerDefine 3, SC_MARK_BACKGROUND
        scivb.DirectSCI.MarkerSetFore 3, vbBlack 'current eip
        scivb.DirectSCI.MarkerSetBack 3, vbYellow
    
        scivb.DirectSCI.AutoCSetIgnoreCase True
        scivb.DisplayCallTips = True
        scivb.ReadOnly = False
        isInitilized = True
        'this is used to block the user from closing the form while debugger is running. and unsubclass..
        On Error Resume Next
        Set ownerForm = UserControl.Parent
        If Err.Number <> 0 Then MsgBox "Could not Hook parent form Unload event is a form the parent container? you may crash on close.."
     End If
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    With scivb
        .Width = UserControl.Width - .Left - 200
        .Height = UserControl.Height - .Top - 600
        fraCmd.Move .Left, .Top + .Height + 20, .Width
        txtCmd.Width = fraCmd.Width - txtCmd.Left - 100
    End With
End Sub

Private Sub tmrSetStatus_Timer()
    'this is just to eliminate flicker when single stepping
    'it was switching back between paused/running super fast and annoying..so if they are single stepping
    'we will wait..and if its still running in 700ms then we will update the label..
    If running Then lblStatus.Caption = "Status: Running"
    tmrSetStatus.Enabled = False
End Sub


''we use a timer for this to give them a chance to click on the calltip to edit the variable..
Private Sub tmrHideCallTip_Timer()
    If scivb.isMouseOverCallTip() Then Exit Sub
    tmrHideCallTip.Enabled = False
    scivb.StopCallTip
End Sub
 
Private Sub scivb_MarginClick(lline As Long, Position As Long, margin As Long, modifiers As Long)
    'Debug.Print "MarginClick: line,pos,margin,modifiers", lLine, Position, margin, modifiers
    ToggleBreakPoint curFile, lline, scivb.GetLineText(lline), Me
End Sub

Private Sub scivb_MouseDwellEnd(lline As Long, Position As Long)
   If running Then tmrHideCallTip.Enabled = True
End Sub

Private Sub scivb_MouseDwellStart(lline As Long, Position As Long)
    

    Dim txt As String
    Dim curWord As String
    Dim cv As CVariable
    Dim eow As Long
    Dim b As Long, lstart As Long, relativeEow As Long
    
    If Not CanIBeActiveInstance(Me) Then Exit Sub
    
    If running Then
         curWord = scivb.WordUnderMouse(Position)
         If Len(curWord) = 0 Then Exit Sub
         
         'this next block allows to do lookups on array values. while we could support
         'looking up indexes calculated with embedded function calls, we are going to
         'avoid it since it may have unexpected side effects. This also blocks math that
         'includes () for operator preecedence though...kiss principle..im ok with it.
         txt = scivb.GetLineText(lline)
         eow = scivb.DirectSCI.WordEndPosition(Position, True) + 1
         lstart = scivb.PositionFromLine(lline)
         relativeEow = eow - lstart
         
         If relativeEow > 0 Then
            txt = Trim(Mid(txt, relativeEow))
            If VBA.Left(txt, 1) = "[" Then
               b = InStr(txt, "]")
               If b > 2 Then
                   txt = Mid(txt, 2, b - 2)
                   If InStr(txt, "(") < 1 Then
                       curWord = curWord & "[" & txt & "]"
                   End If
               End If
            End If
         End If
         '------------------------------------------------------------------------------
         
         If InStr(curWord, "[") > 0 Then
            Set cv = SyncEval(curWord)
            If cv.varType = "undefined" Then Exit Sub
            If Len(cv.Value) = 0 Then Exit Sub
         Else
            Set cv = SyncGetVarValue(curWord)
         End If
         
         If cv.varType <> DUK_VAR_NOT_FOUND Then
            scivb.SelStart = Position 'so call tip shows right under it..
            scivb.SelLength = 0
            txt = cv.Value
            If Len(txt) = 0 Then
                txt = cv.varType
            ElseIf Len(txt) > 25 Then
                txt = Mid(txt, 1, 20) & "..."
            End If
            If cv.varType = "string" Then txt = """" & txt & """"
            scivb.ShowCallTip curWord & " = " & txt
         End If
        
    End If


End Sub

Private Sub txtCmd_KeyPress(KeyAscii As Integer)
    
    Dim v As CVariable
    
    If Not CanIBeActiveInstance(Me) Then Exit Sub
    
    If KeyAscii <> 13 Then Exit Sub 'wait for user to press return key
    KeyAscii = 0 'eat the keypress to prevent vb from doing a msgbeep
    
    If txtCmd.Text = "remote=1" Then
        DBG_PRINT_EXTERNAL = True
        DukOp opd_remoteDebug, , 1
        txtCmd.Text = Empty
        Exit Sub
    ElseIf txtCmd.Text = "remote=0" Then
        DBG_PRINT_EXTERNAL = False
        DukOp opd_remoteDebug, , 0
        txtCmd.Text = Empty
        Exit Sub
    End If
    
    If Left(txtCmd.Text, 1) = "." Or txtCmd.Text = "?" Then
        handleDebuggerCmdLine txtCmd
        txtCmd = Empty
        Exit Sub
    End If
    
    If Not running Then Exit Sub
    If Len(txtCmd.Text) = 0 Then Exit Sub
    
    Set v = SyncEval(txtCmd.Text)
    If v.varType = "undefined" Then Exit Sub
    If Len(v.Value) = 0 Then Exit Sub
    doOutput v.Value
    
End Sub

Sub handleDebuggerCmdLine(cmdLine As String)
    
    On Error Resume Next
    
    Dim args() As String
    Dim ret() As String
    Dim o As CCachedObj
    Dim b As CBreakpoint
    Dim cmd As String
    Dim X
    
    args = Split(cmdLine, " ")
    cmd = LCase(args(0))
    
    If cmd = "?" Then
        push ret, "Command list: .cls .objs .libs .bl .timeout"
        push ret, "You can also use this area to run javascript and set variables."
        push ret, "You can not debug function calls made from here."
    End If
    
    If cmd = ".cls" Then
        RaiseEvent dbgOut("cls")
        Exit Sub
    End If
    
    If Left(cmd, 8) = ".objs" Then
        If objCache.count = 0 Then
            push ret, "No scriptable objects have been added by host"
        Else
            For Each o In objCache
                push ret, o.name & " (" & TypeName(o.obj) & ")"
            Next
        End If
    End If
            
    
    If cmd = ".libs" Then
        If libFiles.count = 0 Then
            push ret, "No libraries script files have been added by host"
        Else
            For Each X In libFiles
                push ret, X
            Next
        End If
    End If
    
    If cmd = ".bl" Then
        If modBreakpoints.breakpoints.count = 0 Then
            push ret, "No breakpoints set"
        Else
            For Each b In modBreakpoints.breakpoints
                push ret, b.Stats & vbCrLf & String(20, "-") & vbCrLf
            Next
        End If
    End If
    
    If cmd = ".timeout" Then
        X = CLng(args(1))
        If Err.Number = 0 Then
            Me.Timeout = CLng(X)
            push ret, "Timeout set to " & X & " seconds"
        Else
            push ret, "Error setting timeout: " & Err.Description
        End If
    End If
        
    RaiseEvent printOut(Join(ret, vbCrLf))
    
End Sub


'for convience but really just use the sci property to get full access. (withevents even)
Property Get Text() As String
    Text = scivb.Text
End Property

Property Let Text(v As String)
    scivb.Text = v
End Property

Property Get isRunning() As Boolean
    isRunning = running
End Property


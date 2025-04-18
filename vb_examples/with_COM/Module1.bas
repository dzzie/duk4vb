Attribute VB_Name = "mDuk"
Public hDukLib As Long
Public libRefCnt As Long 'used when running in IDE...

Public Declare Function DukCreate Lib "Duk4VB.dll" () As Long
Public Declare Function AddFile Lib "Duk4VB.dll" (ByVal ctx As Long, ByVal jsFile As String) As Long
Public Declare Function Eval Lib "Duk4VB.dll" (ByVal ctx As Long, ByVal js As String) As Long
Public Declare Function DukPushNewJSClass Lib "Duk4VB.dll" (ByVal ctx As Long, ByVal jsClassName As String, ByVal hInst As Long) As Long 'returns 0/-1
Public Declare Sub SetCallBacks Lib "Duk4VB.dll" (ByVal msgProc As Long, ByVal dbgCmdProc As Long, ByVal hostResolverProc As Long, ByVal lineInputfunc As Long, Optional ByVal dbgWrite As Long)
Public Declare Function DukOp Lib "Duk4VB.dll" (ByVal operation As opDuk, Optional ByVal ctx As Long = 0, Optional ByVal arg1 As Long, Optional ByVal sArg As String) As Long

'misc windows api..
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Public Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Any, Source As Any, ByVal length As Long)
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Declare Function lstrlen Lib "kernel32.dll" Alias "lstrlenA" (ByVal lpString As Long) As Long

Enum cb_type
    cb_output = 0
    cb_Refresh = 1
    cb_Fatal = 2
    'cb_engine = 3
    cb_error = 4
    cb_ReleaseObj = 5
    cb_StringReturn = 6
    cb_alert = 8
    cb_timeoutCheck = 9
End Enum

Enum opDuk
    opd_PushUndef = 0
    opd_PushNum = 1
    opd_PushStr = 2
    opd_GetInt = 3
    opd_IsNullUndef = 4
    opd_GetString = 5
    opd_Destroy = 6
    opd_LastString = 7
    opd_ScriptTimeout = 8
    opd_PushBool = 13 '0=false
End Enum

Public LastStringReturn As String


Function InitDukLib(Optional ByVal explicitPathToDll As String) As Boolean
    
    If Len(explicitPathToDll) = 0 Then
        explicitPathToDll = App.path
        If Not FileExists(explicitPathToDll & "\duk4vb.dll") Then
            explicitPathToDll = GetParentFolder(explicitPathToDll)
            If Not FileExists(explicitPathToDll & "\duk4vb.dll") Then
                explicitPathToDll = GetParentFolder(explicitPathToDll)
                If Not FileExists(explicitPathToDll & "\duk4vb.dll") Then explicitPathToDll = GetParentFolder(explicitPathToDll)
            End If
        End If
        If Not FileExists(explicitPathToDll & "\duk4vb.dll") Then
            explicitPathToDll = Empty
        Else
            explicitPathToDll = explicitPathToDll & "\duk4vb.dll"
        End If
    End If

    If FileExists(explicitPathToDll) Then
        hDukLib = LoadLibrary(explicitPathToDll) 'to ensure the ide finds the dll
        If hDukLib = 0 Then Exit Function
    End If
    
    'this can still work..but now its up to the runtime to find the dll..if not the app will terminate
    SetCallBacks AddressOf vb_stdout, 0, AddressOf HostResolver, AddressOf VbLineInput
    InitDukLib = True
    
End Function

Function GetLastString() As String
    Dim rv As Long
    rv = DukOp(opd_LastString)
    If rv = 0 Then Exit Function
    GetLastString = StringFromPointer(rv)
End Function

Function AddObject(o As Object, name As String) As Boolean
    On Error GoTo hell
    objs.Add o, name
    AddObject = True
    Exit Function
hell:
End Function

Function GetArgAsString(ctx As Long, index As Long) As String
    
    'an invalid index here would trigger a script error and aborting the eval call..weird.. <---
    'as long as the native function is added with expected arg count, and you dont surpass it your ok
    'even if the js function ommitted args in its call, empty ones will just be retrieved as 'undefined'
    
    Dim ptr As Long
    ptr = DukOp(opd_GetString, ctx, index)
    
    If ptr <> 0 Then
        GetArgAsString = StringFromPointer(ptr)
    End If
    
End Function
 




'callback functions
'------------------------------
Public Function vb_stdout(ByVal t As cb_type, ByVal lpMsg As Long) As Long

    Dim msg As String
    
    'timout not implemented check ocx build
    
    If t = cb_Fatal Then
    
        MsgBox "A fatal error has occured in Duktape the application " & vbCrLf & _
               "is unstable now please save your work and exit." & vbCrLf & vbCrLf & _
               "The specific error message was: " & StringFromPointer(lpMsg), vbCritical, "Fatal Error"
        
        While Forms.Count > 0
            DoEvents
            Sleep 10
        Wend
        
        FreeLibrary hDukLib
        End
        
    End If
    
    If t = cb_Refresh Then
        DoEvents
        Sleep 3
        Exit Function
    End If
    
    If lpMsg = 0 Then Exit Function
    
    msg = StringFromPointer(lpMsg)
    
    Select Case t
        Case cb_StringReturn: LastStringReturn = msg
        Case cb_ReleaseObj: ReleaseObj CLng(msg)
        Case cb_output, cb_error:  MsgBox msg, vbInformation, "Script Output"
        Case cb_alert: MsgBox msg, vbInformation, "Script Alert"
    End Select
    
End Function


Public Function VbLineInput(ByVal buf As Long, ByVal ctx As Long) As Long
    Dim b() As Byte
    Dim retVal As String
    VbLineInput = 0 'return value default..
    
    Dim text As String
    Dim def As String
    
    text = StringFromPointer(buf)
    def = GetArgAsString(ctx, 1)
    
    retVal = InputBox(text, "Script Basic Line Input", def)
    
    If Len(retVal) = 0 Then
        DukOp opd_PushUndef, ctx
        Exit Function
    Else
        DukOp opd_PushStr, ctx, 0, retVal
    End If
        
  
End Function






'utility functions..
'--------------------------------------------------------------------


''http://www.vbforums.com/showthread.php?405366-RESOLVED-Using-CallByName-with-variable-number-of-arguments
'Public Function CallByNameEx(obj As Object, ProcName As String, CallType As VbCallType, Optional vArgsArray As Variant, Optional isObj As Boolean = False)
'
'        Dim oTLI As object 'New TLIApplication
'        Dim ProcID As Long
'        Dim numArgs As Long
'        Dim i As Long
'        Dim v()
'
'        On Error GoTo Handler
'
'        Set oTLI = CreateObject("TLI.TLIApplication")
'        ProcID = oTLI.InvokeID(obj, ProcName)
'
'        If Not IsArray(vArgsArray) Or AryIsEmpty(vArgsArray) Then
'            dbg "CallByName: ", obj, ProcName, isObj
'            If isObj Then
'                Set CallByNameEx = oTLI.InvokeHook(obj, ProcID, CallType)
'            Else
'                CallByNameEx = oTLI.InvokeHook(obj, ProcID, CallType)
'            End If
'        Else
'            numArgs = UBound(vArgsArray)
'            dbg "CallByName: ", obj, ProcName, isObj, Join(vArgsArray, ", ")
'            ReDim v(numArgs)
'            For i = 0 To numArgs
'                v(i) = vArgsArray(numArgs - i)
'            Next i
'            If isObj Then
'                Set CallByNameEx = oTLI.InvokeHookArray(obj, ProcID, CallType, v)
'            Else
'                CallByNameEx = oTLI.InvokeHookArray(obj, ProcID, CallType, v)
'            End If
'        End If
'
'    Exit Function
'
'Handler:
'        dbg "Error in CallByNameEx: ", Err.Number, Err.Description
'End Function

'listbox.additem ..even if the v(0) is String..its adding it as a strptr pointer..(must be taking as a long unless i wrap outter in cstr() fuck you..
'this is stupid..but tli.invokehook doesnt always work where the built in one does (listbox.additem)
'and it adds another external dependancy..so screw it..
Public Function CallByNameEx(obj As Object, ProcName As String, CallType As VbCallType, Optional v As Variant, Optional isObj As Boolean = False)

        Dim ProcID As Long
        Dim numArgs As Long

        On Error GoTo Handler
        
        'callbyName has some weird nuances..apparently v(0) as variant is
        'not the same as a as variant even if both contain a string..
        Dim a, b, c, d, e, f, g, h, i, j
    
        If Not IsArray(v) Or AryIsEmpty(v) Then
            dbg "CallByName: ", obj, ProcName, isObj
            If isObj Then
                Set CallByNameEx = CallByName(obj, ProcName, CallType)
            Else
                CallByNameEx = CallByName(obj, ProcName, CallType)
            End If
        Else
            numArgs = UBound(v)
            
            If numArgs > 9 Then
                MsgBox "CallByNameEx does not support more than 10 args.. method: " & ProcName, vbCritical
            End If
            
            dbg "CallByName: ", obj, ProcName, isObj, Join(v, ", ")
            
            If numArgs >= 0 Then a = v(0)
            If numArgs >= 1 Then b = v(1)
            If numArgs >= 2 Then c = v(2)
            If numArgs >= 3 Then d = v(3)
            If numArgs >= 4 Then e = v(4)
            If numArgs >= 5 Then f = v(5)
            If numArgs >= 6 Then g = v(6)
            If numArgs >= 7 Then h = v(7)
            If numArgs >= 8 Then i = v(8)
            If numArgs >= 9 Then j = v(9)
            
            If isObj Then
                Select Case numArgs
                    Case 0: Set CallByNameEx = CallByName(obj, ProcName, CallType, a)
                    Case 1: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b)
                    Case 2: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c)
                    Case 3: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d)
                    Case 4: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e)
                    Case 5: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f)
                    Case 6: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g)
                    Case 7: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g, h)
                    Case 8: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g, h, i)
                    Case 9: Set CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g, h, i, j)
                End Select
            Else
                Select Case numArgs
                    Case 0:  CallByNameEx = CallByName(obj, ProcName, CallType, a)
                    Case 1:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b)
                    Case 2:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c)
                    Case 3:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d)
                    Case 4:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e)
                    Case 5:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f)
                    Case 6:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g)
                    Case 7:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g, h)
                    Case 8:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g, h, i)
                    Case 9:  CallByNameEx = CallByName(obj, ProcName, CallType, a, b, c, d, e, f, g, h, i, j)
                End Select
            End If
        End If

    Exit Function

Handler:
        dbg "Error in CallByNameEx: ", Err.Number, Err.Description
End Function




Function StringFromPointer(buf As Long) As String
    Dim sz As Long
    Dim tmp As String
    Dim b() As Byte
    
    If buf = 0 Then Exit Function
       
    sz = lstrlen(buf)
    If sz = 0 Then Exit Function
    
    ReDim b(sz)
    CopyMemory b(0), ByVal buf, sz
    tmp = StrConv(b, vbUnicode)
    If Right(tmp, 1) = Chr(0) Then tmp = Left(tmp, Len(tmp) - 1)
    
    StringFromPointer = tmp
 
End Function

Sub dbg(prefix As String, ParamArray args())
    Dim a, tmp As String
    
    tmp = prefix
    
    For Each a In args
        If IsNumeric(a) Then
            tmp = tmp & Hex(a) & ", "
        ElseIf IsObject(a) Then
            tmp = tmp & "Obj: " & TypeName(a) & ", "
        Else
            tmp = tmp & a & ", "
        End If
    Next
    
    Form1.List1.AddItem tmp
    Debug.Print tmp
    
End Sub

Function GetParentFolder(path) As String
    On Error Resume Next
    If Len(path) = 0 Then Exit Function
    tmp = Split(path, "\")
    ub = tmp(UBound(tmp))
    GetParentFolder = Replace(Join(tmp, "\"), "\" & ub, "")
End Function

'Function FileExists(path) As Boolean
'  On Error Resume Next
'  If Len(path) = 0 Then Exit Function
'  If Dir(path, vbHidden Or vbNormal Or vbReadOnly Or vbSystem) <> "" Then FileExists = True
'End Function

Sub push(ary, value) 'this modifies parent ary object
    On Error GoTo init
    x = UBound(ary) '<-throws Error If Not initalized
    ReDim Preserve ary(UBound(ary) + 1)
    ary(UBound(ary)) = value
    Exit Sub
init:     ReDim ary(0): ary(0) = value
End Sub



Function IsIde() As Boolean
' Brad Martinez  http://www.mvps.org/ccrp
    On Error GoTo out
    Debug.Print 1 / 0
out: IsIde = Err
End Function


Function AryIsEmpty(ary) As Boolean
  On Error GoTo oops
    i = UBound(ary)  '<- throws error if not initalized
    AryIsEmpty = False
  Exit Function
oops: AryIsEmpty = True
End Function

Function KeyExistsInCollection(c As Collection, val As String) As Boolean
    On Error Resume Next
    Dim t
    t = c(val)
    If Err.Number = 0 Then
        KeyExistsInCollection = True
        Exit Function
    Else
        Err.Clear
        Set t = c(val) 'maybe its an object collection?
        KeyExistsInCollection = (Err.Number = 0)
    End If
End Function

Function c2s(c As Collection) As String
    Dim x, y
    If c.Count = 0 Then Exit Function
    For Each x In c
        y = y & x & ", "
    Next
    y = Mid(y, 1, Len(y) - 2)
    c2s = y
End Function

Function FolderExists(path As String) As Boolean
  On Error GoTo hell
  Dim tmp As String
  tmp = path & "\"
  If Len(tmp) = 1 Then Exit Function
  If Dir(tmp, vbDirectory) <> "" Then FolderExists = True
  Exit Function
hell:
    FolderExists = False
End Function

Function FileExists(path As String) As Boolean
  On Error GoTo hell
    
  If Len(path) = 0 Then Exit Function
  If Right(path, 1) = "\" Then Exit Function
  If Dir(path, vbHidden Or vbNormal Or vbReadOnly Or vbSystem) <> "" Then FileExists = True
  
  Exit Function
hell: FileExists = False
End Function

Function ReadFile(filename) As Variant
  Dim f As Long
  Dim temp As Variant
  f = FreeFile
  temp = ""
   Open filename For Binary As #f        ' Open file.(can be text or image)
     temp = Input(FileLen(filename), #f) ' Get entire Files data
   Close #f
   ReadFile = temp
End Function

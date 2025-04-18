Attribute VB_Name = "mDuk"
'Author: David Zimmer <dzzie@yahoo.com>
'Site: Sandsprite.com
'License: http://opensource.org/licenses/MIT

Option Explicit
Public hDukLib As Long
Public libRefCnt As Long 'used when running in IDE...
Public startTime As Long

Public Declare Function DukCreate Lib "Duk4VB.dll" () As Long
Public Declare Function AddFile Lib "Duk4VB.dll" (ByVal ctx As Long, ByVal jsFile As String) As Long
Public Declare Function Eval Lib "Duk4VB.dll" (ByVal ctx As Long, ByVal js As String) As Long
Public Declare Function DukPushNewJSClass Lib "Duk4VB.dll" (ByVal ctx As Long, ByVal jsClassName As String, ByVal hInst As Long) As Long 'returns 0/-1
Public Declare Sub SetCallBacks Lib "Duk4VB.dll" (ByVal msgProc As Long, ByVal dbgCmdProc As Long, ByVal cb_HostResolverProc As Long, ByVal lineInputfunc As Long, ByVal debugWritefunc As Long)
Public Declare Function DukOp Lib "Duk4VB.dll" (ByVal operation As opDuk, Optional ByVal ctx As Long = 0, Optional ByVal arg1 As Long, Optional ByVal sArg As String) As Long

'misc windows api..
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Public Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Any, Source As Any, ByVal length As Long)
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Declare Function lstrlen Lib "kernel32.dll" Alias "lstrlenA" (ByVal lpString As Long) As Long
Declare Function GetTickCount Lib "kernel32" () As Long

'call back message types we receive from duktape in cb_stdout
Enum cb_type
    cb_output = 0
    cb_Refresh = 1
    cb_Fatal = 2
    'cb_engine = 3
    cb_error = 4
    cb_ReleaseObj = 5
    cb_StringReturn = 6
    cb_debugger = 7
    cb_alert = 8
    cb_timeoutCheck = 9
End Enum

'DukOp declare operation codes..
Enum opDuk
    opd_PushUndef = 0
    opd_PushNum = 1
    opd_PushStr = 2
    opd_GetInt = 3
    opd_IsNullUndef = 4 'unused apparently..dont even rememebr adding it! probably for the COM stuff which isnt in this project..
    opd_GetString = 5
    opd_Destroy = 6
    opd_LastString = 7
    opd_ScriptTimeout = 8
    opd_debugAttach = 9
    opd_dbgCoOp = 10
    opd_dbgTriggerRead = 12
    opd_PushBool = 13 '0=false
End Enum

'commands we create to send to duktape
Enum Debug_Commands
    dc_NotSet = 0
    dc_break = 2
    dc_stepInto = 3
    dc_stepout = 4
    dc_StepOver = 5
    dc_Resume = 9
    dc_GetLocals = 10
    dc_GetVar = 11
    dc_SetBreakpoint = 12
    dc_delBreakpoint = 13
    dc_GetCallStack = 14
    dc_Eval = 15
End Enum


Global Const STATUS_NOTIFICATION = &H1
Global Const PRINT_NOTIFICATION = &H2
Global Const ALERT_NOTIFICATION = &H3
Global Const LOG_NOTIFICATION = &H4

Global Const BASIC_INFO_REQ = &H10
Global Const TRIGGER_STATUS_REQ = &H11
Global Const PAUSE_REQ = &H12
Global Const RESUME_REQ = &H13
Global Const STEP_INTO_REQ = &H14
Global Const STEP_OVER_REQ = &H15
Global Const STEP_OUT_REQ = &H16
Global Const LIST_BREAK_REQ = &H17
Global Const ADD_BREAK_REQ = &H18
Global Const DEL_BREAK_REQ = &H19
Global Const GET_VAR_REQ = &H1A
Global Const PUT_VAR_REQ = &H1B
Global Const GET_CALL_STACK_REQ = &H1C
Global Const GET_LOCALS_REQ = &H1D
Global Const EVAL_REQ = &H1E
Global Const DETACH_REQ = &H1F
Global Const DUMP_HEAP_REQ = &H20
    
Global Const DUK_DBG_MARKER_EOM = 0
Global Const DUK_DBG_MARKER_REQUEST = 1
Global Const DUK_DBG_MARKER_REPLY = 2
Global Const DUK_DBG_MARKER_ERROR = 3
Global Const DUK_DBG_MARKER_NOTIFY = 4

Global Const DUK_DBG_CMD_STATUS = &H1
Global Const DUK_DBG_CMD_PRINT = &H2
Global Const DUK_DBG_CMD_ALERT = &H3
Global Const DUK_DBG_CMD_LOG = &H4

Global Const DUK_DBG_ERR_UNKNOWN = &H0
Global Const DUK_DBG_ERR_UNSUPPORTED = &H1
Global Const DUK_DBG_ERR_TOOMANY = &H2
Global Const DUK_DBG_ERR_NOTFOUND = &H3
Global Const DUK_VAR_NOT_FOUND = "DUK_VAR_NOT_FOUND"

Global running As Boolean
Public LastStringReturn As String
Public readyToReturn As Boolean
Public LastCommand As Debug_Commands
Public replyReceived As Boolean

Public ActiveUserControl As ucDukDbg
Public ActiveDukTapeClass As CDukTape

Public RespBuffer As New CResponseBuffer
Public RecvBuffer As New CWriteBuffer
Public Status As New CStatus

'because we have to work across callbacks we need some module level variables..
Private tmpVar       As CVariable
Public tmpBreakPoint As CBreakpoint  'used in modBreakpoints so public
Private tmpCol       As Collection
Private ctlCount As Long

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
    SetCallBacks AddressOf cb_stdout, _
                 AddressOf cb_GetDbgCmd, _
                 AddressOf cb_HostResolver, _
                 AddressOf cb_prompt, _
                 AddressOf cb_DebugDataIncoming
                 
    InitDukLib = True
    
End Function

Function isControlActive() As Boolean
    If ActiveUserControl Is Nothing Then Exit Function
    'If ActiveUserControl.Closing Then Exit Function
    isControlActive = True
End Function

Function CanIBeActiveInstance(ctl As ucDukDbg) As Boolean
    If ActiveUserControl Is Nothing Then CanIBeActiveInstance = True
    If ObjPtr(ActiveUserControl) = ObjPtr(ctl) Then CanIBeActiveInstance = True
End Function

Function AmIActive(ctl As ucDukDbg) As Boolean
    If ActiveUserControl Is Nothing Then Exit Function
    If ObjPtr(ActiveUserControl) = ObjPtr(ctl) Then AmIActive = True
End Function

'Function MonitorInstances(ctl As ucDukDbg, Optional isClosing As Boolean = False)
'
'    Dim duk As CDukTape
'
'    ctlCount = ctlCount + IIf(isClosing, -1, 1)
'
'    If isClosing And ObjPtr(ActiveUserControl) = ObjPtr(ctl) Then
'        'check if running?
''        If running Then
''            If Not ctl.duktape Is Nothing Then
''                ctl.duktape.Timeout = 1
''                forceShutDown = True
''                SendDebuggerCmd dc_stepInto
''                If ctl.duktape.isDebugging Then ctl.duktape.DebugAttach False
''            End If
''        End If
'        Set ActiveUserControl = Nothing
'        running = False
'    End If
'
'    If ctlCount = 0 Then
'      'do we need to do any library teardown?
'      'zero out callbacks?
'    End If
'
'End Function

'debugger is just starting up first message already received..
Sub On_DebuggerInilitize()
    Status.StepToLine = -1
    InitDebuggerBpx ActiveUserControl
End Sub

Private Sub On_DebuggerTerminate()
    
    Set Status = New CStatus 'clear all fields for next session
    
    Dim b As CBreakpoint
    For Each b In breakpoints
        b.isSet = False
    Next
    
End Sub

'call back: this is used for script to host app object integration..

'see notes in DukOp for opd_dbgTriggerRead
Function SyncGetVarValue(name As String) As CVariable
    Set tmpVar = New CVariable
    tmpVar.name = name
    LastCommand = dc_GetVar
    replyReceived = False
    RespBuffer.ConstructMessage dc_GetVar, name
    DukOp opd_dbgTriggerRead, ActiveUserControl.context
    Set SyncGetVarValue = tmpVar
End Function

Function SyncSetBreakPoint(b As CBreakpoint) As Boolean
    Set tmpBreakPoint = b
    LastCommand = dc_SetBreakpoint
    replyReceived = False
    RespBuffer.ConstructMessage dc_SetBreakpoint, b.FileName, b.lineNo + 1
    DukOp opd_dbgTriggerRead, ActiveUserControl.context
    SyncSetBreakPoint = CBool(Len(b.errText) = 0)
End Function

Function SyncDelBreakPoint(b As CBreakpoint) As Boolean
    Set tmpBreakPoint = b
    b.errText = Empty
    LastCommand = dc_delBreakpoint
    replyReceived = False
    RespBuffer.ConstructMessage dc_delBreakpoint, b.index
    DukOp opd_dbgTriggerRead, ActiveUserControl.context
    SyncDelBreakPoint = CBool(Len(b.errText) = 0)
End Function

Function SyncGetCallStack() As Collection 'of cCallStack
    Set tmpCol = New Collection
    LastCommand = dc_GetCallStack
    replyReceived = False
    RespBuffer.ConstructMessage dc_GetCallStack
    DukOp opd_dbgTriggerRead, ActiveUserControl.context
    Set SyncGetCallStack = tmpCol
End Function

Function SyncEval(js As String) As CVariable
    Set tmpVar = New CVariable
    tmpVar.name = "eval result"
    LastCommand = dc_Eval
    replyReceived = False
    RespBuffer.ConstructMessage dc_Eval, js
    DukOp opd_dbgTriggerRead, ActiveUserControl.context
    Set SyncEval = tmpVar
End Function

Sub SyncPauseExecution()
    LastCommand = dc_break
    replyReceived = False
    RespBuffer.ConstructMessage dc_break
    DukOp opd_dbgTriggerRead, ActiveUserControl.context
End Sub

Sub SyncStep(dc As Debug_Commands)
    If dc = dc_stepInto Or dc = dc_stepout Or dc = dc_StepOver Then
        LastCommand = dc
        replyReceived = False
        RespBuffer.ConstructMessage dc
        DukOp opd_dbgTriggerRead, ActiveUserControl.context
    End If
End Sub



'Public Sub LoadCallStack()
'
'    Dim li As ListItem
'    Dim c As cCallStack
'    Dim callStack As Collection
'
'    status.callStackLoaded = True
'    Set callStack = SyncGetCallStack()
'
'    'columns line, function, file
'    Form1.lvCallStack.ListItems.Clear
'    For Each c In callStack
'        Set li = Form1.lvCallStack.ListItems.Add(, , c.lineNo)
'        li.SubItems(1) = c.func
'        li.SubItems(2) = FileNameFromPath(c.fpath)
'    Next
'
'End Sub

'if its a critical command to know was sent check return, if just BS like user step who cares..
Public Function SendDebuggerCmd(cmd As Debug_Commands, Optional arg1, Optional arg2) As Boolean
    
    If Not running Then Exit Function
    If ActiveUserControl Is Nothing Then Exit Function
    
    If Not replyReceived Then
        'we are still waiting for last commands response..maybe user hit step step step to fast?
        dbg "SendDebuggerCmd still waiting for reply exiting"
        Exit Function
    End If
    
    ActiveUserControl.ClearLastLineMarkers
    replyReceived = False
    
    LastCommand = cmd
    If Not RespBuffer.ConstructMessage(cmd, arg1, arg2) Then
        dbg "Failed to construct message for " & cmd
    Else
        readyToReturn = True
        SendDebuggerCmd = True
    End If
    
End Function

Function GetLastString() As String
    Dim rv As Long
    rv = DukOp(opd_LastString)
    If rv = 0 Then Exit Function
    GetLastString = StringFromPointer(rv)
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
 




'callback: debugger misc message stream
'------------------------------
Public Function cb_stdout(ByVal t As cb_type, ByVal lpMsg As Long) As Long

    Dim msg As String, timeOutMS As Long, curTicks As Long
    
    If t = cb_timeoutCheck Then
    
        If Not ActiveDukTapeClass Is Nothing Then
            timeOutMS = ActiveDukTapeClass.Timeout
            If timeOutMS > 0 Then
                curTicks = GetTickCount
                If (curTicks - startTime) > timeOutMS Then
                    cb_stdout = 1
                    Exit Function
                End If
            End If
        End If

        cb_stdout = 0
        DoEvents
        Sleep 2
        Exit Function
        
    End If
    
    If t = cb_Fatal Then
    
        MsgBox "A fatal error has occured in Duktape the application " & vbCrLf & _
               "is unstable now please save your work and exit." & vbCrLf & vbCrLf & _
               "The specific error message was: " & StringFromPointer(lpMsg), vbCritical, "Fatal Error"
        
        While 1
            DoEvents
            Sleep 10
        Wend

    End If
    
    If t = cb_Refresh Then
        DoEvents
        Sleep 2
        Exit Function
    End If
    
    If lpMsg = 0 Then Exit Function
    
    'this callback can be used from CDukTape standalone not just debugger specific..
    'If Not isControlActive() Then Exit Sub
    
    msg = StringFromPointer(lpMsg)
    
    Select Case t
        Case cb_StringReturn: LastStringReturn = msg
        Case cb_ReleaseObj: ReleaseObj CLng(msg)
        Case cb_output: doOutput msg: dbg "Output received: " & msg
        Case cb_error:  dbg "Script Error: " & msg
        Case cb_alert: MsgBox msg, vbInformation, "Script Alert"
        Case cb_debugger:
                If msg = "Debugger-Detached" Then
                    running = False
                    dbg "Debugger detached!"
                    On_DebuggerTerminate
                End If
                
    End Select
    
End Function

'unix to dos then output to active class..
Public Function doOutput(msg)
    Dim tmp As String
    tmp = Replace(msg, vbCr, Empty)
    tmp = Replace(tmp, vbLf, Chr(5))
    tmp = Replace(tmp, Chr(5), vbCrLf)
    
    On Error Resume Next
    If Not ActiveDukTapeClass Is Nothing Then
        ActiveDukTapeClass.doPrintOut tmp
    ElseIf Not ActiveUserControl Is Nothing Then
        ActiveUserControl.duk_printOut tmp
    Else
        Debug.Print "debug.print: No active control/class? output: " & msg
    End If
    
End Function

'call back: prompt function implementation
Public Function cb_prompt(ByVal buf As Long, ByVal ctx As Long) As Long
    Dim b() As Byte
    Dim retVal As String
    cb_prompt = 0 'return value default..
    
    Dim Text As String
    Dim def As String
    
    Text = StringFromPointer(buf)
    def = GetArgAsString(ctx, 1)
    
    retVal = InputBox(Text, "Script Basic Line Input", def)
    
    If Len(retVal) = 0 Then
        DukOp opd_PushUndef, ctx
        Exit Function
    Else
        DukOp opd_PushStr, ctx, 0, retVal
    End If
        
  
End Function

'call back: debugger is requesting a command to operate on..
'           vb blocks until user enters command..
'           this is the duktape read data routine
Public Function cb_GetDbgCmd(ByVal buf As Long, ByVal sz As Long) As Long
    
    Dim i As Long
    Dim cmd_length As Long
    Dim b() As Byte
    
        If Not isControlActive() Then Exit Function
        
topLine:
        If Not RespBuffer.isEmpty Then
            
            If RespBuffer.GetBuf(sz, b) Then
                CopyMemory ByVal buf, ByVal VarPtr(b(0)), sz
                cb_GetDbgCmd = sz
            End If
            
            Exit Function
        End If
        
        If Not RecvBuffer.firstMessage Then
            If Not RecvBuffer.breakPointsInitilized Then 'only once at debugger start
                RecvBuffer.breakPointsInitilized = True
                On_DebuggerInilitize
            ElseIf Not Status.callStackLoaded Then 'every time we step/bp
                
                If Len(Status.FileName) > 0 And Status.FileName <> ActiveUserControl.CurrentFile Then
                    'my personal preference is to only debug current file user sees..
                    'for me any other js is lib files I add as glue and dont want to bother them with..
                    SendDebuggerCmd dc_stepout
                    GoTo topLine
                End If
                
                If Status.lastLineNo = Status.lineNumber And LastCommand = dc_stepout Then
                    'must be above case + var assignment of return value..
                    SendDebuggerCmd dc_StepOver
                    GoTo topLine
                End If
                                
                If Status.StepToLine <> -1 Then
                    'this is the run to cursor implementation
                    If Status.lineNumber = Status.StepToLine Then
                        Status.StepToLine = -1
                    Else
                        SendDebuggerCmd dc_stepInto
                        GoTo topLine
                    End If
                End If

                'LoadCallStack
                'LoadVariables disabled see comments at bottom of module.
            End If
            
        End If
        
        'doOutput "STATUS_NOTIFICATION Line: " & status.lineNumber & " LastLine: " & status.lastLineNo & " pc: " & status.pc & " File: " & status.fileName
        ActiveUserControl.SyncUI
        
        'we block here until the UI sets the readyToReturn = true
        'this is not a CPU hog, and form remains responsive to user actions..
        readyToReturn = False
        ActiveUserControl.SetStatus dsPaused
        While Not readyToReturn
            DoEvents
            Sleep 20
            i = i + 1
            
            If RespBuffer.isEmpty And Not isControlActive() Then Exit Function
            
            If running = False Then 'we have a detach
                Exit Function
            End If
            
            If i = 500 Then
                If isControlActive() Then
                    DukOp opd_dbgCoOp, ActiveUserControl.context
                End If
                i = 0
            End If
            
        Wend
        If isControlActive() Then ActiveUserControl.SetStatus dsRunning
        
        If Not RespBuffer.isEmpty Then
            
            If RespBuffer.GetBuf(sz, b) Then
                CopyMemory ByVal buf, ByVal VarPtr(b(0)), sz
                cb_GetDbgCmd = sz
            End If
            
            Exit Function
        End If
        
        
End Function

'call back: debugger is sending our interface data,
'           this happens in multiple stages until a single EOM byte is received (00)
'           this is the duktape write data routine
Public Function cb_DebugDataIncoming(ByVal buf As Long, ByVal sz As Long) As Long

    If buf = 0 Or sz = 0 Then Exit Function 'shouldnt happen...
    If Not isControlActive() Then Exit Function
    
    Dim b() As Byte
    ReDim b(sz - 1) 'b is 0 based,
    CopyMemory b(0), ByVal buf, sz
    'Debug.Print bHexDump(b)
     
    RecvBuffer.WriteBuf b()
    cb_DebugDataIncoming = sz
End Function

'called by RecvBuff when a full message has been received..
Public Function On_FullMessageReceived()
    
    Dim b As Byte
    Dim i As Long
        
    If Not isControlActive() Then Exit Function
    
    With RecvBuffer
    
        'doOutput "MessageReceived: " & .BytesLeft & " bytes"
        
        b = .ReadByte()
        
        If .firstMessage Then
            'I actually have a small bug in my parser..i didnt realize that the initial
            'debugger attach message is terminated by a LF only..so the buffer right now
            'actually contains two messages..the dbg attach message, and a status notification
            'message. So we will just check the protocol version, then gobble up the rest of the
            'message to get to the status message and then process that..oops!
            If b <> Asc(1) Then MsgBox "Bad debugger protocol version!"
            '<protocolversion> <SP (0x20)> <additional text, no LF> <LF (0x0a)>
            '1 <DUK_VERSION> <DUK_GIT_DESCRIBE> <target string> <LF>
            '1 10099 v1.0.0-254-g2459e88 duk command built from Duktape repo
            Do
                b = .ReadByte
            Loop While b <> &HA
            b = .ReadByte()
        End If
        
        Select Case b
            Case DUK_DBG_MARKER_NOTIFY:  HandleNotify
            
            Case DUK_DBG_MARKER_REQUEST: .DebugDump ("Request")
            
            'ERR <int: error code> <str: error message or empty string> EOM
            '0x00    Unknown or unspecified error
            '0x01    Unsupported command
            '0x02    Too many (e.g. too many breakpoints, cannot add new)
            '0x03    Not found (e.g. invalid breakpoint index)
            Case DUK_DBG_MARKER_ERROR:
                     
                     Select Case LastCommand
                        Case dc_SetBreakpoint:
                                                .ReadInt 'discard errorcode
                                                tmpBreakPoint.errText = .ReadString
                        Case dc_delBreakpoint:
                                                .ReadInt 'discard errorcode
                                                tmpBreakPoint.errText = .ReadString
                        Case Else:
                            .DebugDump ("Error")
                    End Select
            
            Case DUK_DBG_MARKER_REPLY:
            
                    If .BytesLeft = 0 Then
                        'Debug.Print "Success Reply"
                        Exit Function
                    End If
                    
                    'the reply is specific to the last command we issued..
                    Select Case LastCommand
                        Case dc_GetVar, dc_Eval: HandleGetVar
                        Case dc_SetBreakpoint:   tmpBreakPoint.index = .ReadInt 'REP <int: breakpoint index> EOM
                        Case dc_GetCallStack:    HandleCallStack
                        'Case dc_GetLocals:       HandleGetLocals
                        Case Else:
                            .DebugDump ("Reply last cmd: " & LastCommand)
                    End Select
                    
        End Select
        
    End With
    
End Function
                                                                                        
Function HandleNotify()
    Dim fname As String, func As String
    Dim msg As Long, state As Long, lno As Long, pc As Long
    
    With RecvBuffer
    
        If .BytesLeft < 4 Then Exit Function '??
        msg = .ReadInt
        
        Select Case msg
                   'NFY <int: 1> <int: state> <str: filename> <str: funcname> <int: linenumber> <int: pc> EOM
              Case STATUS_NOTIFICATION:
                    Status.lastLineNo = Status.lineNumber
                    Status.state = .ReadInt
                    Status.FileName = .ReadString
                    Status.curFunc = .ReadString
                    Status.lineNumber = .ReadInt
                    Status.pc = .ReadInt
                    Status.callStackLoaded = False
                    
                    'for debugging
                    'doOutput "STATUS_NOTIFICATION Line: " & status.lineNumber & " LastLine: " & status.lastLineNo & " pc: " & status.pc & " File: " & status.fileName
                    
                    'next one for debugging the C code since UI will be frozen (vb IDE keeps window WM_PAINT working at breakpoints which is SOOOOO handy!)
                    'MsgBox "STATUS_NOTIFICATION Line: " & status.lineNumber & "LastLine: " & status.lastLineNo & " pc: " & status.pc
                                    
                   'NFY <int: 2> <str: message> EOM - String output redirected from the print() function.
              Case PRINT_NOTIFICATION: .DebugDump "Print Notify"
              
                   'NFY <int: 3> <str: message> EOM - String output redirected from the alert() function.
              Case ALERT_NOTIFICATION: .DebugDump "Alert Notify"
              
                   'NFY <int: 4> <int: log level> <str: message> EOM - Logger output redirected from Duktape logger calls.
              Case LOG_NOTIFICATION:
                    .DebugDump "Log Notify"
        End Select
        
    End With
    
    
End Function



Function HandleGetVar() 'maybe better named read_tval
    
    Dim b As Byte
    Dim found As Long
    Dim hadErr As Long
    
    If LastCommand = dc_GetVar Then
        'REP <int: 0/1, found> <tval: value> EOM
        found = RecvBuffer.ReadInt
        If found = 0 Then
            tmpVar.varType = DUK_VAR_NOT_FOUND
            Exit Function
        End If
    ElseIf LastCommand = dc_Eval Then
        'REP <int: 0=success, 1=error> <tval: value> EOM
        'on error tval will contain the error message as string
        hadErr = RecvBuffer.ReadInt
    ElseIf LastCommand = dc_GetLocals Then
        'we dont have to do anything any prfixes already called..using as a read_tval call..
    Else
        dbg "in HandleGetVar for unknown reason?"
        Exit Function
    End If
    
        
    With tmpVar
        b = RecvBuffer.ReadByte
        Select Case b
                
            Case &H10: '0x10 <int32>    integer     4-byte integer, signed 32-bit integer in network order follows initial byte
                    .varType = "integer"
                    .value = RecvBuffer.ReadInt(False)
            
            Case &H11: '0x11 <uint32> <data>    string  4-byte string, unsigned 32-bit string length in network order and string data follows initial byte
                    .varType = "string"
                    .value = RecvBuffer.ReadString(False)
            
            Case &H16: .varType = "undefined"                 '0x16    undefined   Ecmascript "undefined"
            Case &H17: .varType = "null"                      '0x17    null    Ecmascript "null"
            Case &H18: .varType = "boolean": .value = True    '0x18    true    Ecmascript "true"
            Case &H19:  .varType = "boolean": .value = False  '0x19    false   Ecmascript "false"
            Case &H1A: .varType = "double":                   '0x1a <8 bytes>  number  IEEE double (network endian)
                       .value = RecvBuffer.ReadDouble()
                       
            '0x1b <uint8> <uint8> <data>     object  Class number, pointer length, and pointer data (network endian)
            Case &H1B: .varType = "object": .value = "[Object]"
            
            '0x1c <uint8> <data>     pointer     Pointer length, pointer data (network endian)
            '0x1d <uint16> <uint8> <data>    lightfunc   Lightfunc flags, pointer length, pointer data (network endian)
            '0x1e <uint8> <data>     heapptr     Pointer to a heap object (used by DumpHeap, network endian)
            
            '0x13 <uint32> <data>    buffer  4-byte buffer, unsigned 32-bit buffer length in network order and buffer data follows initial byte
            '0x14 <uint16> <data>    buffer  2-byte buffer, unsigned 16-bit buffer length in network order and buffer data follows initial byte
            
            
            'these ones we dont have to worry about..(not using packed protocol)
                '0x12 <uint16> <data>    string  2-byte string, unsigned 16-bit string length in network order and string data follows initial byte
                '0x60...0x7f <data>  string  String with length [0,31], string length is IB - 0x60, data follows
                '0x80...0xbf     integer     Integer [0,63], integer value is IB - 0x80
                '0xc0...0xff <uint8>     integer     Integer [0,16383], integer value is ((IB - 0xc0) << 8) + followup_byte
            
            'unused:
                '0x15    unused  Represents the internal "undefined unused" type which used to e.g. mark unused (unmapped) array entries
                '0x1f reserved
                '0x20...0x5f     reserved
            
            Case Else: .varType = "VarType: 0x" & Hex(b)
        End Select
        
    End With

End Function


Function HandleCallStack()
    'REP [ <str: fileName> <str: funcName> <int: lineNumber> <int: pc> ]* EOM
    
    Dim c As cCallStack
    
    With RecvBuffer
        Do While 1
            Set c = New cCallStack
            c.fpath = .ReadString
            c.func = .ReadString
            c.lineNo = .ReadInt
            c.pc = .ReadInt
            tmpCol.Add c
            If .BytesLeft = 0 Then Exit Do
        Loop
    End With
    
End Function


'so the getlocals command has some limitations that make it not worth using for me:
' 1) only works for local variables once in a function
' 2) only works if the script explicitly prefixes the variable with var
' 3) to support this..my HandleGetVar would have to fully support every tval type which it doesnt
'     the above limitations and gui complexity/space a var list window would take up and how easy
'     it is to mouse over a variable to get its value on demand or use the command window to get it
'     I am just going to eliminate getlocals support because of these limitations..
'Function HandleGetLocals()
'    'note documentation is wrong second argument is actually a tval
'    'REP [ <str: varName> <str: varValue> ]* EOM
'
'    Dim c As CVariable
'
'    With RecvBuffer
'        Do While 1
'            Set tmpVar = New CVariable
'            Set c = New CVariable
'            c.name = .ReadString
'            HandleGetVar 'read tval into tmpVar
'            c.Value = tmpVar.Value
'            c.varType = tmpVar.varType
'            tmpCol.Add c
'            If .BytesLeft = 0 Then Exit Do
'        Loop
'    End With
'
'End Function
'
'Sub LoadVariables()
'
'    Dim li As ListItem
'    Dim v As CVariable
'    Dim vars As Collection
'
'    Form1.lvVars.ListItems.Clear
'    Set vars = SyncGetLocals()
'
'    'columns: name,value (type not returned)
'    For Each v In vars
'        Set li = Form1.lvVars.ListItems.Add(, , v.name)
'        li.SubItems(1) = v.Value
'    Next
'
'End Sub
'
'Function SyncGetLocals() As Collection 'of cvariable
'    Set tmpCol = New Collection
'    LastCommand = dc_GetLocals
'    replyReceived = False
'    RespBuffer.ConstructMessage dc_GetLocals
'    DukOp opd_dbgTriggerRead, ActiveDebuggerClass.Context
'    Set SyncGetLocals = tmpCol
'End Function




VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{2668C1EA-1D34-42E2-B89F-6B92F3FF627B}#5.0#0"; "scivb2.ocx"
Begin VB.Form Form1 
   Caption         =   "DukTape JS Debugger Example"
   ClientHeight    =   9795
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   13845
   LinkTopic       =   "Form1"
   ScaleHeight     =   9795
   ScaleWidth      =   13845
   StartUpPosition =   2  'CenterScreen
   Begin sci2.SciSimple scivb 
      Height          =   5775
      Left            =   90
      TabIndex        =   9
      Top             =   765
      Width           =   13695
      _ExtentX        =   24156
      _ExtentY        =   10186
   End
   Begin VB.Timer tmrSetStatus 
      Enabled         =   0   'False
      Interval        =   700
      Left            =   9900
      Top             =   180
   End
   Begin VB.Frame fraCmd 
      Height          =   600
      Left            =   225
      TabIndex        =   6
      Top             =   8685
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
         TabIndex        =   8
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
         TabIndex        =   7
         Top             =   225
         Width           =   510
      End
   End
   Begin MSComctlLib.ListView lvLog 
      Height          =   1050
      Left            =   1575
      TabIndex        =   5
      Top             =   6795
      Width           =   1860
      _ExtentX        =   3281
      _ExtentY        =   1852
      View            =   3
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      NumItems        =   1
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "Message"
         Object.Width           =   1411
      EndProperty
   End
   Begin VB.TextBox txtOut 
      Height          =   1185
      Left            =   5670
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   4
      Top             =   6705
      Width           =   3165
   End
   Begin MSComctlLib.ListView lvCallStack 
      Height          =   1185
      Left            =   8955
      TabIndex        =   3
      Top             =   6795
      Width           =   1815
      _ExtentX        =   3201
      _ExtentY        =   2090
      View            =   3
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      NumItems        =   3
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "Line"
         Object.Width           =   1235
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "Function"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   2
         Text            =   "File"
         Object.Width           =   2540
      EndProperty
   End
   Begin VB.Timer tmrHideCallTip 
      Enabled         =   0   'False
      Interval        =   600
      Left            =   8010
      Top             =   135
   End
   Begin MSComctlLib.ImageList ilToolbars_Disabled 
      Left            =   9225
      Top             =   45
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
            Picture         =   "Form1.frx":0000
            Key             =   "Run"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":010C
            Key             =   "Break"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0218
            Key             =   "Run to Cursor"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0324
            Key             =   "Clear All Breakpoints"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":042E
            Key             =   "Step Over"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":053A
            Key             =   "Step Out"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0646
            Key             =   "Step In"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0752
            Key             =   "Stop"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":085E
            Key             =   "Start Debugger"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0968
            Key             =   "Toggle Breakpoint"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList ilToolbar 
      Left            =   8550
      Top             =   45
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
            Picture         =   "Form1.frx":0A72
            Key             =   "Run"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0B7E
            Key             =   "Start Debugger"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0C88
            Key             =   "Break"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0D92
            Key             =   "Stop"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0E9C
            Key             =   "Toggle Breakpoint"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0FA6
            Key             =   "Clear All Breakpoints"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":10B0
            Key             =   "Step In"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":11BA
            Key             =   "Step Over"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":12C4
            Key             =   "Step Out"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":13CE
            Key             =   "Run to Cursor"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":14D8
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.Toolbar tbarDebug 
      Height          =   330
      Left            =   135
      TabIndex        =   0
      Top             =   270
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
   Begin MSComctlLib.TabStrip ts 
      Height          =   3120
      Left            =   90
      TabIndex        =   2
      Top             =   6570
      Width           =   13695
      _ExtentX        =   24156
      _ExtentY        =   5503
      Placement       =   1
      _Version        =   393216
      BeginProperty Tabs {1EFB6598-857C-11D1-B16A-00C0F0283628} 
         NumTabs         =   3
         BeginProperty Tab1 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Output"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab2 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "CallStack"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab3 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Log"
            ImageVarType    =   2
         EndProperty
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Label lblStatus 
      Caption         =   "Status: Idle"
      Height          =   375
      Left            =   4095
      TabIndex        =   1
      Top             =   270
      Width           =   4560
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Author: David Zimmer <dzzie@yahoo.com>
'Site: Sandsprite.com
'License: http://opensource.org/licenses/MIT

Dim duk As CDukTape
 
Const SC_MARK_CIRCLE = 0
Const SC_MARK_ARROW = 2
Const SC_MARK_BACKGROUND = 22
'http://www.scintilla.org/aprilw/SciLexer.bas
 
Public lastEIP As Long
Public curFile As String
Private userStop As Boolean

Public Sub SyncUI()
       
    Dim curline As Long
    
    If Len(status.fileName) = 0 Or status.fileName <> curFile Then Exit Sub
    
    ClearLastLineMarkers
    
    curline = status.lineNumber - 1
    scivb.SetMarker curline, 1
    scivb.SetMarker curline, 3
    lastEIP = curline
    
    scivb.GotoLine curline
    scivb.SetFocus
    Me.Refresh
    DoEvents

End Sub
 


Public Sub ClearLastLineMarkers()
    Dim startPos As Long, endPos As Long

    scivb.DeleteMarker lastEIP, 1 'remove the yellow arrow
    scivb.DeleteMarker lastEIP, 3 'remove the yellow line backcolor

    'force a refresh of the specified line or it might not catch it..
    startPos = scivb.PositionFromLine(lastEIP)
    endPos = scivb.PositionFromLine(lastEIP + 1)
    scivb.DirectSCI.Colourise startPos, endPos
    
End Sub



Private Sub Command1_Click()
duk.Reset
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If Not duk Is Nothing Then
        duk.Timeout = -1
        forceShutDown = True
        SendDebuggerCmd dc_stepInto
        If duk.isDebugging Then duk.DebugAttach False
        Set duk = Nothing
    End If
End Sub

Private Sub lvLog_DblClick()
    If lvLog.SelectedItem Is Nothing Then Exit Sub
    MsgBox lvLog.SelectedItem.Tag, vbInformation
End Sub

Private Sub scivb_KeyDown(KeyCode As Long, Shift As Long)

    Dim curline As Long
    
    'Debug.Print KeyCode & " " & Shift
    Select Case KeyCode
        Case vbKeyF2: curline = scivb.CurrentLine
                      ToggleBreakPoint curFile, curline, scivb.GetLineText(curline)
                      
        Case vbKeyF5: If running Then SendDebuggerCmd dc_Resume Else ExecuteScript True
        Case vbKeyF7: SendDebuggerCmd dc_stepInto
        Case vbKeyF8: SendDebuggerCmd dc_StepOver
        Case vbKeyF9: SendDebuggerCmd dc_stepout
    End Select

End Sub

Private Sub tbarDebug_ButtonClick(ByVal Button As MSComctlLib.Button)
    Dim curline As Long
    Dim txt As String
    
    On Error Resume Next
    
    Select Case Button.Key
        Case "Run":               If running Then SendDebuggerCmd dc_Resume Else ExecuteScript
        Case "Start Debugger":    If running Then SendDebuggerCmd dc_Resume Else ExecuteScript True
        Case "Step In":           SendDebuggerCmd dc_stepInto
        Case "Step Over":         SendDebuggerCmd dc_StepOver
        Case "Step Out":          SendDebuggerCmd dc_stepout
        Case "Clear All Breakpoints": RemoveAllBreakpoints
        Case "Break":                 SyncPauseExecution

        Case "Run to Cursor":
                                  curline = scivb.CurrentLine
                                  txt = scivb.GetLineText(curline)
                                  If Not isExecutableLine(txt) Then
                                        doOutput "Can not run to cursor: not executable line"
                                  Else
                                        status.stepToLine = curline + 1
                                        SendDebuggerCmd dc_stepInto
                                  End If
                                  
        Case "Toggle Breakpoint":
                                  curline = scivb.CurrentLine
                                  ToggleBreakPoint curFile, curline, scivb.GetLineText(curline)
                                    
        Case "Stop":
                                  userStop = True
                                  duk.Timeout = -1
                                  SendDebuggerCmd dc_stepInto
                        
        
    End Select
    
End Sub

Private Sub ExecuteScript(Optional withDebugger As Boolean)
 
    Dim rv

    running = True
    SetToolBarIcons
    lblStatus = "Status: " & IIf(withDebugger, "Debugging...", "Running...")
    txtOut.Text = Empty
    lvLog.ListItems.Clear
    lvCallStack.ListItems.Clear
    
    userStop = False
    Set duk = New CDukTape
    Set RecvBuffer = New CWriteBuffer 'this resets our flags like firstMessage and bpInitilized...
    
    If Not duk.AddFile(App.path & "\lib.js") Then
        doOutput "lib.js: " & duk.LastError
        Exit Sub
    End If
    
    If withDebugger Then
        duk.Timeout = 0
        duk.DebugAttach
    Else
        duk.Timeout = 7 'set to 0 to disabled
    End If
     
    WriteFile curFile, scivb.Text
    rv = duk.AddFile(curFile)
    
    If Not duk Is Nothing Then 'form closing?
         If withDebugger Then duk.DebugAttach False
        
         If duk.hadError Then
             If Not userStop Then
                doOutput duk.LastError
             End If
         End If
         
         Set duk = Nothing
         ClearLastLineMarkers
         lblStatus = "Status: Idle" 'these would call form_load again if closing down..
         running = False
         SetToolBarIcons
    
    End If
    
End Sub

Private Sub SetToolBarIcons()
    Dim b As Button
    
    Set tbarDebug.ImageList = Nothing
    Set tbarDebug.ImageList = IIf(running, ilToolbar, ilToolbars_Disabled)
    
    For Each b In tbarDebug.Buttons
        If Len(b.Key) > 0 Then
            b.Image = b.Key
            b.ToolTipText = b.Key
            If b.Key <> "Run" And b.Key <> "Start Debugger" And InStr(b.Key, "Breakpoint") < 1 Then
                b.Enabled = running
            End If
        End If
    Next
    
End Sub

Private Sub Form_Load()

    SetToolBarIcons
    lvCallStack.Visible = False
    lvLog.Visible = False
    
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
    Call scivb.LoadCallTips(App.path & "\dependancies\calltips.txt")
    scivb.ReadOnly = False
    
    'scivb.Text = Replace(Replace("function b(c){\n\treturn c++\n}\na=0;\na = b(a)\na=b(a)", "\n", vbCrLf), "\t", vbTab)
    scivb.LoadFile App.path & "\test.js"
    curFile = App.path & "\test.js"
 
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    With scivb
        .Width = Me.Width - .Left - 200
        ts.Width = .Width
        txtOut.Width = .Width - 200
        ts.Top = Me.Height - ts.Height - 800
        .Height = Me.Height - .Top - ts.Height - 1000
        With lvLog
            .Move ts.Left + 100, ts.Top + 150, ts.Width - 200, ts.Height - 500
            lvCallStack.Move .Left, .Top, .Width, .Height
            txtOut.Move .Left, .Top, .Width, .Height - fraCmd.Height - 100
            fraCmd.Move .Left, txtOut.Top + txtOut.Height + 20, .Width
            txtCmd.Width = fraCmd.Width - txtCmd.Left - 100
        End With
        SetLastColumnWidth lvCallStack
        SetLastColumnWidth lvLog
    End With
End Sub

Private Sub SetLastColumnWidth(lv As ListView)
    lv.ColumnHeaders(lv.ColumnHeaders.Count).Width = lv.Width - lv.ColumnHeaders(lv.ColumnHeaders.Count).Left - 100
End Sub

Private Sub tmrSetStatus_Timer()
    'this is just to eliminate flicker when single stepping
    'it was switching back between paused/running super fast and annoying..so if they are single stepping
    'we will wait..and if its still running in 700ms then we will update the label..
    If running Then lblStatus.Caption = "Status: Running"
    tmrSetStatus.Enabled = False
End Sub

Private Sub ts_Click()
    Dim i As Long
    i = ts.SelectedItem.index
    txtOut.Visible = IIf(i = 1, True, False)
    lvCallStack.Visible = IIf(i = 2, True, False)
    lvLog.Visible = IIf(i = 3, True, False)
    fraCmd.Visible = txtOut.Visible
End Sub

''we use a timer for this to give them a chance to click on the calltip to edit the variable..
Private Sub tmrHideCallTip_Timer()
    If scivb.isMouseOverCallTip() Then Exit Sub
    tmrHideCallTip.Enabled = False
    scivb.StopCallTip
    Set selVariable = Nothing
End Sub
 
Private Sub scivb_MarginClick(lline As Long, Position As Long, margin As Long, modifiers As Long)
    'Debug.Print "MarginClick: line,pos,margin,modifiers", lLine, Position, margin, modifiers
    ToggleBreakPoint curFile, lline, scivb.GetLineText(lline)
End Sub

Private Sub scivb_MouseDwellEnd(lline As Long, Position As Long)
   If running Then tmrHideCallTip.Enabled = True
End Sub

Private Sub scivb_MouseDwellStart(lline As Long, Position As Long)

    Dim txt As String
    Dim curWord As String
    Dim cv As CVariable
    
    If running Then
         curWord = scivb.WordUnderMouse(Position)
         If Len(curWord) = 0 Then Exit Sub
         Set cv = SyncGetVarValue(curWord)
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
    
    If KeyAscii <> 13 Then Exit Sub 'wait for user to press return key
    KeyAscii = 0 'eat the keypress to prevent vb from doing a msgbeep
    
    If txtCmd.Text = "cls" Then
        txtOut.Text = Empty
        Exit Sub
    End If
    
    If Not running Then Exit Sub
    If Len(txtCmd.Text) = 0 Then Exit Sub
    
    Set v = SyncEval(txtCmd.Text)
    If v.varType = "undefined" Then Exit Sub
    If Len(v.Value) = 0 Then Exit Sub
    doOutput v.Value
    
End Sub

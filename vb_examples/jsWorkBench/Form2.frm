VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{047848A0-21DD-421D-951E-B4B1F3E1718D}#85.0#0"; "dukDbg.ocx"
Begin VB.Form Form2 
   Caption         =   "jsThing - http://sandsprite.com"
   ClientHeight    =   8310
   ClientLeft      =   165
   ClientTop       =   1020
   ClientWidth     =   14355
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form2"
   ScaleHeight     =   8310
   ScaleWidth      =   14355
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrFormatting 
      Enabled         =   0   'False
      Interval        =   300
      Left            =   13950
      Top             =   0
   End
   Begin dukDbg.ucDukDbg DukDbg 
      Height          =   6360
      Left            =   2610
      TabIndex        =   8
      Top             =   135
      Width           =   11670
      _ExtentX        =   20585
      _ExtentY        =   11218
   End
   Begin MSComctlLib.ListView lv2 
      Height          =   2670
      Left            =   30
      TabIndex        =   3
      TabStop         =   0   'False
      Top             =   5580
      Width           =   2295
      _ExtentX        =   4048
      _ExtentY        =   4710
      View            =   3
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      GridLines       =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   2
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "msg"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "data"
         Object.Width           =   2540
      EndProperty
   End
   Begin VB.Frame splitter 
      BackColor       =   &H00808080&
      Height          =   75
      Left            =   2610
      MousePointer    =   7  'Size N S
      TabIndex        =   1
      Top             =   6615
      Width           =   11895
   End
   Begin MSComctlLib.ListView lv 
      Height          =   2775
      Left            =   45
      TabIndex        =   0
      TabStop         =   0   'False
      Top             =   90
      Width           =   2295
      _ExtentX        =   4048
      _ExtentY        =   4895
      View            =   3
      LabelEdit       =   1
      MultiSelect     =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      NumItems        =   1
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "Saved Scripts"
         Object.Width           =   2540
      EndProperty
   End
   Begin RichTextLib.RichTextBox txtOut 
      Height          =   1455
      Left            =   2535
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   6780
      Width           =   11760
      _ExtentX        =   20743
      _ExtentY        =   2566
      _Version        =   393217
      HideSelection   =   0   'False
      ScrollBars      =   2
      TextRTF         =   $"Form2.frx":0000
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin MSComctlLib.ListView lvFunc 
      Height          =   2670
      Left            =   45
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   2880
      Width           =   2295
      _ExtentX        =   4048
      _ExtentY        =   4710
      View            =   3
      LabelEdit       =   1
      MultiSelect     =   -1  'True
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      FullRowSelect   =   -1  'True
      GridLines       =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   2
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "Functions"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "loc"
         Object.Width           =   2540
      EndProperty
   End
   Begin VB.Label lblClipboard 
      AutoSize        =   -1  'True
      Caption         =   "^"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   15.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   345
      Index           =   2
      Left            =   2340
      TabIndex        =   5
      ToolTipText     =   "Copy output to script pane"
      Top             =   6750
      Width           =   195
   End
   Begin VB.Label lblClipboard 
      AutoSize        =   -1  'True
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "System"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   240
      Index           =   1
      Left            =   2385
      TabIndex        =   7
      ToolTipText     =   "copy output to clipboard"
      Top             =   7290
      Width           =   120
   End
   Begin VB.Label lblClipboard 
      AutoSize        =   -1  'True
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "System"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   240
      Index           =   0
      Left            =   2385
      TabIndex        =   6
      ToolTipText     =   "Copy Script to Clipboard"
      Top             =   720
      Width           =   120
   End
   Begin VB.Menu mnuMainLoadFile 
      Caption         =   "Load"
      Begin VB.Menu mnuLoadFile 
         Caption         =   "Load JavaScript"
      End
      Begin VB.Menu mnuLoadShellcode 
         Caption         =   "Load Shellcode"
      End
   End
   Begin VB.Menu mnuBeautify 
      Caption         =   "Format_JS"
   End
   Begin VB.Menu mnuUnescapeSelection 
      Caption         =   "Unescape_Selection (F6)"
   End
   Begin VB.Menu mnuManualEscapes 
      Caption         =   "Manual_Escapes"
      Begin VB.Menu mnuManualEscape 
         Caption         =   "HexString Unescape (Preserve White Space)"
         Index           =   0
      End
      Begin VB.Menu mnuManualEscape 
         Caption         =   "HexString Unescape (Strip White Space)"
         Index           =   1
      End
      Begin VB.Menu mnuManualEscape 
         Caption         =   "% Unescape"
         Index           =   2
      End
      Begin VB.Menu mnuManualEscape 
         Caption         =   "\x Unescape"
         Index           =   3
      End
      Begin VB.Menu mnuManualEscape 
         Caption         =   "\n Unescape"
         Index           =   4
      End
      Begin VB.Menu mnuManualEscape 
         Caption         =   "# Unescape"
         Index           =   5
      End
      Begin VB.Menu mnuManualEscape 
         Caption         =   "Octal UnEscape"
         Index           =   6
      End
      Begin VB.Menu mnuAddPercentoHexString 
         Caption         =   "Add % to HexString"
      End
      Begin VB.Menu mnuVarPrefix 
         Caption         =   "Prefix Sel Lines with var"
      End
      Begin VB.Menu mnuUniAsciiToHex 
         Caption         =   "\u00 -> \x"
      End
      Begin VB.Menu mnuHex2Unicode 
         Caption         =   "HexString to %u encoded"
      End
   End
   Begin VB.Menu mnuSimplifySelection 
      Caption         =   "Simplify_Selection_Quotes"
   End
   Begin VB.Menu mnuShellcodeUI 
      Caption         =   "Shellcode_Analysis"
      Begin VB.Menu mnuLaunchSclog 
         Caption         =   "scDbg ( libEmu - Emulation )"
         Index           =   1
      End
      Begin VB.Menu mnuScSigs 
         Caption         =   "scSigs  (Sig Check + libemu Unpack)"
      End
      Begin VB.Menu mnuXorBruteForce 
         Caption         =   "Xor BruteForcer"
      End
      Begin VB.Menu mnuspacer99 
         Caption         =   "-"
      End
      Begin VB.Menu mnuSaveShellcode 
         Caption         =   "Save Bytes to file"
      End
      Begin VB.Menu mnuSc2ExeMain 
         Caption         =   "Shellcode 2 Exe"
      End
      Begin VB.Menu mnuShellcode2Exe 
         Caption         =   "Simple Husk (sc 0x401000)"
         Index           =   0
         Visible         =   0   'False
      End
      Begin VB.Menu mnuShellcode2Exe 
         Caption         =   "Simple Husk w/Wsa Startup  (sc 0x401020)"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuShellcode2Exe 
         Caption         =   "New Husk (supports cmdline + 6k sc buf)"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuSend2IDA 
         Caption         =   "Disassemble in IDA"
      End
   End
   Begin VB.Menu mnuReplace 
      Caption         =   "Find/Replace"
   End
   Begin VB.Menu mnuDeobTools 
      Caption         =   "Deobsfuscation Tools"
      Begin VB.Menu mnuBasicRefactor 
         Caption         =   "Basic Refactor"
      End
      Begin VB.Menu mnuStripInlineDecoderCalls 
         Caption         =   "Strip Inline Decoder Calls"
      End
      Begin VB.Menu mnuReplaceHexAscii 
         Caption         =   "Replace Hex Ascii Strings"
      End
      Begin VB.Menu mnuProcessActionScript 
         Caption         =   "Process ActionScript"
      End
      Begin VB.Menu mnuStrAryReplace 
         Caption         =   "String Array Elem Replace"
      End
   End
   Begin VB.Menu mnuPopup 
      Caption         =   "mnuPopup"
      Begin VB.Menu mnuSaveAll 
         Caption         =   "Save All"
      End
      Begin VB.Menu mnuSaveToFile 
         Caption         =   "Save to file"
      End
      Begin VB.Menu mnuSpacer5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCopyToJs 
         Caption         =   "Copy to JS TextBox"
      End
      Begin VB.Menu mnuCopyToLower 
         Caption         =   "Copy To Lower TextBox"
      End
      Begin VB.Menu mnuSpacer4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuRenameScript 
         Caption         =   "Rename Entry"
      End
      Begin VB.Menu mnuRemoveEntry 
         Caption         =   "Remove Entry"
      End
      Begin VB.Menu mnuClearAll 
         Caption         =   "Clear All"
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "Options"
      Begin VB.Menu mnuGotoLine 
         Caption         =   "Goto Line"
      End
      Begin VB.Menu mnuCopyFuncsNames 
         Caption         =   "Copy Func Names"
      End
      Begin VB.Menu mnuSciVerInfo 
         Caption         =   "Scintinilla Version"
      End
      Begin VB.Menu mnuShowHelp 
         Caption         =   "Show Help"
      End
      Begin VB.Menu mnuWordWrap 
         Caption         =   "Word Wrap"
      End
      Begin VB.Menu mnuCodeFolding 
         Caption         =   "Code Folding"
      End
      Begin VB.Menu mnuCollapseAll 
         Caption         =   "Collapse/Expand All"
      End
      Begin VB.Menu mnuIndentGuide 
         Caption         =   "Show Indent Guides"
      End
   End
   Begin VB.Menu mnuPopup3 
      Caption         =   "mnuPopup3"
      Begin VB.Menu mnuCopyAllDatalv2 
         Caption         =   "Copy All w/Data"
      End
   End
   Begin VB.Menu mnuPopupFuncs 
      Caption         =   "mnuPopupFuncs"
      Begin VB.Menu mnuFunctionScan 
         Caption         =   "Rescan   (F5)"
      End
      Begin VB.Menu mnuRenameFunc 
         Caption         =   "Rename  (N)"
      End
      Begin VB.Menu mnuExtractFunc 
         Caption         =   "Extract"
      End
      Begin VB.Menu mnuCopyFuncNames 
         Caption         =   "Copy All Names"
      End
      Begin VB.Menu mnuSeqRenameFuncs 
         Caption         =   "Sequential Rename All"
      End
      Begin VB.Menu mnuHighLightAllRefs 
         Caption         =   "Highlight All References"
      End
      Begin VB.Menu mnuFindFuncRefs 
         Caption         =   "Find All References  (R)"
      End
      Begin VB.Menu mnuFindFuncDependancies 
         Caption         =   "Function Dependancies"
      End
      Begin VB.Menu mnuCopyRenameMap 
         Caption         =   "Copy Rename Map"
      End
      Begin VB.Menu mnuGraphTo 
         Caption         =   "Graph Xrefs To  (X)"
      End
      Begin VB.Menu mnuiGraphFrom 
         Caption         =   "Graph Xrefs From (Z)"
      End
   End
   Begin VB.Menu mnuMainPopup 
      Caption         =   "mnuMainPopup"
      Begin VB.Menu mnuMain 
         Caption         =   "Jump To"
         Index           =   0
      End
      Begin VB.Menu mnuMain 
         Caption         =   "Find Refs"
         Index           =   1
      End
      Begin VB.Menu mnuMain 
         Caption         =   "Graph To"
         Index           =   2
      End
      Begin VB.Menu mnuMain 
         Caption         =   "Graph From"
         Index           =   3
      End
      Begin VB.Menu mnuMain 
         Caption         =   "Copy"
         Index           =   4
      End
      Begin VB.Menu mnuMain 
         Caption         =   "Paste"
         Index           =   5
      End
      Begin VB.Menu mnuMain 
         Caption         =   "To Hex"
         Index           =   6
      End
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Author:   dzzie@yahoo.com
'Site:     http://sandsprite.com

Option Explicit

Dim WithEvents duk As CDukTape
Attribute duk.VB_VarHelpID = -1

Public txtJS As sci2.SciSimple
Attribute txtJS.VB_VarHelpID = -1

Dim dlg As New CCmnDlg
Dim toolbox As New CScriptFunctions

'splitter code taken from sample by Bruce Fast, submitted to the public domain. thanks!

'todo: we could add our listview to the toolbox class so scripts can pull
'saved script variables directly from there via something like
' savedVar1 = tb.lv.listitems(index).tag
'even if i am the only one who would use that :P

Private Capturing As Boolean
Private Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long
Private Declare Function ReleaseCapture Lib "user32" () As Long
Private Declare Function GetKeyState Lib "user32" (ByVal nVirtKey As Long) As Integer
Private Const VK_LBUTTON = &H1

Private Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Private Declare Function ScreenToClient Lib "user32" (ByVal hwnd As Long, lpPoint As POINTAPI) As Long
Private Type POINTAPI
        x As Long
        Y As Long
End Type

Private objsAdded As Boolean
Dim USING_MYMAIN As Boolean
Private Declare Function SetFilePointer Lib "kernel32" (ByVal hFile As Long, ByVal lDistanceToMove As Long, lpDistanceToMoveHigh As Long, ByVal dwMoveMethod As Long) As Long
Dim renames() As String

Dim navPoints() As Long
Dim curTopLine As Long
Dim ignoreNavPoint As Boolean

Const UNK_FUNC = "????"

Public Function StandardizeLineBreaks(ByVal x)
    x = Replace(x, vbCrLf, Chr(5))
    x = Replace(x, vbCr, Chr(5))
    x = Replace(x, vbLf, Chr(5))
    StandardizeLineBreaks = Replace(x, Chr(5), vbCrLf)
End Function

 
Private Sub duk_dbgOut(msg As String)
    Debug.Print "DukDbg: " & msg
End Sub

Private Sub duk_Error(ByVal line As Long, ByVal desc As String)

    Debug.Print "DukError: " & desc
    
    On Error Resume Next
    Dim pos As Long
    
    txtJS.GotoLine line - 1
    pos = txtJS.PositionFromLine(line - 1)
    txtJS.SelStart = pos
    txtJS.SelLength = Len(txtJS.GetLineText(line - 1))
    
    txtOut.Text = "Time: " & Now & vbCrLf & "Line: " & line & vbCrLf & " Error: " & desc
    txtOut.Text = txtOut.Text & vbCrLf & "Source: " & txtJS.GetLineText(line - 1)  'vbsci specific
    lv2.ListItems.Add , , "Error: " & txtOut.Text
    
End Sub

Private Sub DukDbg_dukErr(line As Long, msg As String)
    duk_Error line, msg
End Sub

Private Sub DukDbg_printOut(msg As String)
    toolbox.t msg
    
'    Dim leng As Long
'    Dim includeCRLF As Boolean
'
'    leng = Len(Form1.txtOut.Text)
'
'    If leng > 0 And right(tmp, 2) <> vbCrLf Then includeCRLF = True
'
'    txtOut.SelLength = 0
'    txtOut = txtOut & IIf(includeCRLF, vbCrLf, "") & tmp
'    txtOut.SelStart = leng + 2
'
End Sub

 
Private Sub DukDbg_StateChanged(state As DukDbg.dbgStates)
    If state = dsStarted Then
        lv2.ListItems.Clear
        txtOut.Text = Empty
    End If
End Sub

Private Sub lv2_ItemClick(ByVal Item As MSComctlLib.ListItem)
    On Error Resume Next
    If Len(Item.SubItems(1)) > 0 Then
        txtOut.Text = Item.SubItems(1)
    Else
        txtOut.Text = Item.Text
    End If
End Sub

Private Sub lv2_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    If Button = 2 Then PopupMenu mnuPopup3
End Sub

Private Sub lvFunc_ColumnClick(ByVal ColumnHeader As MSComctlLib.ColumnHeader)
    LV_ColumnSort lvFunc, ColumnHeader
End Sub

Private Sub lvFunc_DblClick()
    On Error Resume Next
    If Not lvFunc.SelectedItem Is Nothing Then
         'txtJS.GotoLine lvFunc.SelectedItem.tag
         'push navPoints, CLng(lvFunc.SelectedItem.tag)
         txtJS.FirstVisibleLine = CLng(lvFunc.SelectedItem.Tag)
         txtJS.SelectLine
         txtJS.SetFocus
    End If
End Sub

Private Sub lvFunc_KeyDown(KeyCode As Integer, Shift As Integer)
    'MsgBox KeyCode
    If KeyCode = 116 Then 'f5
        mnuFunctionScan_Click
    End If
End Sub

Private Sub lvFunc_KeyPress(KeyAscii As Integer)
    'MsgBox KeyAscii
    If KeyAscii = Asc("N") Or KeyAscii = Asc("n") Then
        mnuRenameFunc_Click
        KeyAscii = 0
    End If
    If KeyAscii = Asc("R") Or KeyAscii = Asc("r") Then
       mnuFindFuncRefs_Click
       KeyAscii = 0
    End If
    If KeyAscii = Asc("X") Or KeyAscii = Asc("x") Then
       mnuGraphTo_Click
       KeyAscii = 0
    End If
    If KeyAscii = Asc("Z") Or KeyAscii = Asc("z") Then
       mnuiGraphFrom_Click
       KeyAscii = 0
    End If
End Sub

Private Sub lvFunc_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    If Button = 2 Then PopupMenu mnuPopupFuncs
End Sub

Private Sub mnuCodeFolding_Click()
    mnuCodeFolding.Checked = Not mnuCodeFolding.Checked
    txtJS.Folding = mnuCodeFolding.Checked
End Sub

Private Sub mnuCollapseAll_Click()
    If mnuCodeFolding.Checked = False Then mnuCodeFolding_Click
    txtJS.FoldAll
End Sub

Private Sub mnuCopyAllDatalv2_Click()
    On Error Resume Next
    Dim li As ListItem
    Dim ret As String
    
    For Each li In lv2.ListItems
        If Len(li.SubItems(1)) > 0 Then
            ret = ret & li.SubItems(1) & vbCrLf
        End If
    Next
    Clipboard.Clear
    Clipboard.SetText ret
    MsgBox Len(ret) & " bytes copied", vbInformation
End Sub

Private Sub mnuCopyFuncNames_Click()
    Dim li As ListItem
    Dim tmp
    For Each li In lvFunc.ListItems
        tmp = tmp & li.Text & vbCrLf
    Next
    Clipboard.Clear
    Clipboard.SetText tmp
    MsgBox Len(tmp) & " bytes copied", vbInformation
End Sub

Private Sub mnuCopyFuncsNames_Click()
    On Error Resume Next
    Dim x, Y, tmp
    
    x = Split(txtJS.Text, vbCrLf)
    For Each Y In x
        If InStr(Y, "function") > 0 Then
            tmp = tmp & Y & vbCrLf
        End If
    Next
    tmp = Replace(tmp, vbTab, Empty)
    Clipboard.Clear
    Clipboard.SetText tmp
    MsgBox UBound(Split(tmp, vbCrLf)) & " lines copied to clipboard"
End Sub

Private Sub mnuCopyRenameMap_Click()
    Clipboard.Clear
    Clipboard.SetText Join(renames, vbCrLf)
End Sub

Private Sub mnuCopyToJs_Click()
    On Error Resume Next
    If lv.SelectedItem Is Nothing Then Exit Sub
    txtJS.Text = lv.SelectedItem.Tag
    Erase renames
End Sub

Private Sub mnuCopyToLower_Click()
    On Error Resume Next
    If lv.SelectedItem Is Nothing Then Exit Sub
    txtOut.Text = lv.SelectedItem.Tag
End Sub

Public Function ExtractFunction(ByVal startLine As Long, Optional ByRef foundEnd, Optional includeSpacer As Boolean = True, Optional linesOfCode As Long) As String
    Dim data, tmp() As String, j, x, il As Long, indent As Long
    
    indent = GetIndentLevel(startLine)
    data = IIf(includeSpacer, vbCrLf & vbCrLf, Empty)
    j = startLine
    foundEnd = False
    
    Do
        x = txtJS.GetLineText(j)
        
        If Len(x) > 2 Then
            If right(x, 2) = vbCrLf Then x = Mid(x, 1, Len(x) - 2)
        End If
        
        If Len(x) > 1 Then
            If right(x, 1) = vbLf Then x = Mid(x, 1, Len(x) - 1)
        End If
                
        il = GetIndentLevel(, x)
        data = data & x & vbCrLf
        
        If il = indent Then x = Trim(Replace(x, vbTab, ""))
        
        If VBA.left(x, 1) = "}" Then
            foundEnd = True
            Exit Do
        End If
        
        j = j + 1
        If j > txtJS.TotalLines Then 'GetLineText failing on very last line?
            foundEnd = True
            Exit Do
        End If
        
        DoEvents
    Loop While Not foundEnd
    
    linesOfCode = j - startLine
    ExtractFunction = data & IIf(includeSpacer, vbCrLf & vbCrLf, Empty)
    
End Function

Private Sub mnuExpandAll_Click()
    If mnuCodeFolding.Checked = True Then mnuCodeFolding_Click
End Sub

Private Sub mnuExploitScan_Click()

End Sub

Private Sub mnuExtractFunc_Click()
    On Error Resume Next
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    Dim li As ListItem
    Dim data As String, tmp As String
    Dim foundEnd As Boolean
    For Each li In lvFunc.ListItems
        If li.Selected Then
            data = data & ExtractFunction(CLng(li.Tag), foundEnd)
            If Not foundEnd Then Exit Sub
        End If
    Next
    tmp = fso.GetFreeFileName(Environ("temp"))
    fso.writeFile tmp, data & vbCrLf & vbCrLf
    Shell "notepad.exe " & tmp, vbNormalFocus
End Sub

Private Sub mnuFindFuncDependancies_Click()
    On Error Resume Next
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    Dim li As ListItem
    Dim data As String, startfunc, report, tmp
    Dim foundEnd As Boolean
    Dim func() As String
     
    startfunc = lvFunc.SelectedItem.Text
    
    data = ExtractFunction(CLng(lvFunc.SelectedItem.Tag), foundEnd)
    
    For Each li In lvFunc.ListItems
        If li.Text <> startfunc Then li.Selected = False
        If InStr(data, li.Text & "(") > 0 And li.Text <> startfunc Then
            push func, li.Text
            li.Selected = True
        End If
    Next
    
    report = "Non Recursive function references found within: " & startfunc & vbCrLf & vbCrLf
    report = report & vbTab & Join(func, vbCrLf & vbTab)
    
    tmp = fso.GetFreeFileName(Environ("temp"))
    fso.writeFile tmp, report
    Shell "notepad.exe " & tmp, vbNormalFocus
    
End Sub

Private Sub mnuFindFuncRefs_Click()
    On Error Resume Next
    Dim find As String
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    find = lvFunc.SelectedItem.Text
    If Len(find) = 0 Then Exit Sub
    Dim f As Object
    Set f = txtJS.ShowFindReplace
    f.Text1 = find
    f.cmdFindAll_Click
End Sub

Public Sub mnuFunctionScan_Click()
    
    'kinda quick and dirty function scan, assumes you already ran format js
    'wokrs on standard js, js with functions embedded within funcs, and jquery type classes.
    
    On Error Resume Next
    
    Dim li As ListItem
    Dim loc As Long, i As Long, x As String, func, a, b
    
    lvFunc.ListItems.Clear

    For i = 0 To txtJS.DirectSCI.GetLineCount
        x = txtJS.GetLineText(i)
        func = Empty
        
        a = InStr(x, " = function(")
        b = InStr(x, ": function(")
        
        If a > 0 Then
            func = Trim(Mid(x, 1, a))
            func = Trim(Replace(func, vbTab, Empty))
        ElseIf b > 0 Then
            func = Trim(Mid(x, 1, b - 1))
            func = Trim(Replace(func, vbTab, Empty))
        ElseIf InStr(x, "function") > 0 And InStr(x, "(") > 0 And InStr(x, "{") > 0 Then
            a = InStr(x, "(")
            b = InStrRev(x, " ", a)
            func = Trim(Mid(x, b, a - b))
        End If
        
        If AnyofTheseInstr(func, "[,(") Then func = UNK_FUNC
        
        If Len(func) > 0 Then
            Set li = lvFunc.ListItems.Add(, , func)
            ExtractFunction i, , False, loc
            li.SubItems(1) = VBA.right("    " & loc, 5) 'for sorting
            li.Tag = i
        End If
        
        DoEvents
        
    Next
    
End Sub

Function GetIndentLevel(Optional lineNo = 0, Optional strText = Empty) As Long
    On Error Resume Next
    Dim cnt As Long, x
    
    If Len(strText) > 0 Then
        x = strText
    Else
        x = txtJS.GetLineText(lineNo)
    End If
    
    While VBA.left(x, 1) = vbTab
        cnt = cnt + 1
        x = Mid(x, 2)
    Wend
    
    GetIndentLevel = cnt
    
End Function

Private Sub mnuGotoLine_Click()
    On Error Resume Next
    txtJS.ShowGoto
End Sub

Private Sub mnuGraphTo_Click()
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    Dim f As New frmFuncGraph
    f.GraphTo lvFunc.SelectedItem.Text
End Sub

Private Sub mnuHex2Unicode_Click()
    On Error Resume Next
    Dim x, a, b, ret, i
    
    x = Replace(txtJS.SelText, vbCrLf, Empty)
    x = Replace(x, Chr(0), "")
    If Len(x) = 0 Then
        MsgBox "Nothing selected!"
        Exit Sub
    End If
    While Len(x) Mod 2 <> 0
        x = x & "0"
    Wend
    For i = 1 To Len(x) Step 4
       a = Mid(x, i, 2)
       b = Mid(x, i + 2, 2)
       ret = ret & "%u" & b & a
    Next
    If right(ret, 2) = "%u" Then ret = Mid(ret, 1, Len(ret) - 2)
    txtJS.SelText = ret
End Sub

Private Sub mnuHighLightAllRefs_Click()
    On Error Resume Next
    Dim find As String
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    find = lvFunc.SelectedItem.Text
    If Len(find) = 0 Then Exit Sub
    Me.Caption = "  " & txtJS.hilightWord(find, , vbBinaryCompare) & " instances of " & find & " found"
End Sub

Private Sub mnuiGraphFrom_Click()
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    Dim f As New frmFuncGraph
    f.GraphFrom lvFunc.SelectedItem.Text
End Sub

Private Sub mnuIndentGuide_Click()
    mnuIndentGuide.Checked = Not mnuIndentGuide.Checked
    txtJS.ShowIndentationGuide = mnuIndentGuide.Checked
End Sub

Public Sub mnuLoadShellcode_Click()
    Dim f As String, x
    dlg.SetCustomFilter "Shellcode File (*.sc)", "*.sc"
    f = dlg.OpenDialog(CustomFilter, "", "Open file")
    If Len(f) = 0 Then Exit Sub
    If Not fso.FileExists(f) Then Exit Sub
    x = HexDump(fso.ReadFile(f), 1)
    txtJS.Text = AddPercentToHexString(x)
    txtJS.SelectAll
End Sub

Private Sub mnuMain_Click(Index As Integer)
     
    On Error Resume Next
    Dim f As Object
    Dim hexstring As String
    
    Select Case Index
        Case 0: lvFunc_DblClick
        Case 1:
                If Len(txtJS.CurrentWord) > 0 Then
                    If isWordFunctionName(txtJS.CurrentWord) Then
                        mnuFindFuncRefs_Click   'func already selected by mouse up event code
                    Else
                        Set f = txtJS.ShowFindReplace
                        f.Text1 = txtJS.CurrentWord
                        f.cmdFindAll_Click
                    End If
                End If
                
        Case 2: mnuGraphTo_Click
        Case 3: mnuiGraphFrom_Click
        Case 4:
                If txtJS.SelLength > 0 Then
                    txtJS.Copy
                Else
                    Clipboard.Clear
                    Clipboard.SetText txtJS.CurrentWord
                End If
                
        Case 5: txtJS.Paste
        Case 6:
                hexstring = LongHex(CDbl(txtJS.CurrentWord))
                If Err.Number = 0 Then txtJS.ReplaceAll txtJS.CurrentWord, "0x" & hexstring
    End Select
    
End Sub

'thanks wof: http://forums.codeguru.com/showthread.php?477230-Converting-large-decimal-numbers-to-Hex
Function LongHex(ByVal Number As Double) As String
  Dim i#, r#, h$
  Do
     i = Int(Number / 16)
     r = Number - i * 16
     h = Hex$(r) + h
     Number = i
  Loop While Number > 0
  LongHex = h$
End Function

Private Sub mnuOutputToClip_Click()
    lblClipboard_Click 1
End Sub

Private Sub mnuOutputToScript_Click()
    lblClipboard_Click 2
End Sub

Private Sub mnuProcessActionScript_Click()
    On Error Resume Next
    Dim tmp
    tmp = ProcessActionScript(txtJS.Text)
    If Len(tmp) = 0 Then
        MsgBox "Had an error processing the text, assumes output from as3 sourcerer", vbInformation
    Else
        txtJS.Text = tmp
        mnuBeautify_Click
    End If
End Sub

 

Private Sub mnuRenameFunc_Click()

    On Error Resume Next
    Dim fl As Long, oldname, NewName
    Dim li As ListItem
    
    If lvFunc.SelectedItem Is Nothing Then Exit Sub
    
    fl = txtJS.FirstVisibleLine 'this can be buggy...
    Debug.Print "Top line: " & fl
    
    oldname = lvFunc.SelectedItem.Text
    If oldname = UNK_FUNC Then
        MsgBox "Can not rename unknown function", vbInformation
        Exit Sub
    End If
    
    NewName = InputBox("Enter new name for " & oldname, , oldname)
    If Len(NewName) = 0 Then Exit Sub
    
    For Each li In lvFunc.ListItems
        If li.Text = NewName Then
            MsgBox "This name is already taken"
            Exit Sub
        End If
    Next
    
    If InStr(txtJS.Text, NewName) > 0 Then
        MsgBox "This string is already found in the current script please make unique"
        Exit Sub
    End If
    
    push renames, oldname & " -> " & NewName
    txtJS.Text = Replace(txtJS.Text, oldname & "(", NewName & "(")
    txtJS.FirstVisibleLine = fl
    
    'MsgBox txtJS.SCI.ReplaceAll(CStr(oldname), CStr(NewName), True) 'buggy...
    
    lvFunc.SelectedItem.Text = NewName
    
End Sub

Private Sub mnuReplaceHexAscii_Click()
    Dim r As New RegExp
    Dim m As match
    Dim tmp As String
    Dim b() As Byte
    Dim i As Long
    Dim a As Byte
    Dim Z As Byte
    Dim zero As Byte
    Dim nine As Byte
    Dim mm
    Dim isOk As Boolean
    Dim topLine As Long
    
    On Error Resume Next
    
    topLine = Form2.txtJS.FirstVisibleLine
    mnuUniAsciiToHex_Click
    
    a = Asc("A")
    Z = Asc("Z")
    zero = Asc("0")
    nine = Asc("9")
    
    r.Global = True
    r.IgnoreCase = True
    r.Pattern = "(\\[xX][0-9a-fA-F]+)+" 'find all \x__\x__ strings

    Set mm = r.Execute(txtJS.Text)
    
    For Each m In mm
        
        'Debug.Print m.Value
        tmp = js_unescape(m.value)
        If Len(tmp) > 0 Then
            b() = StrConv(UCase(tmp), vbFromUnicode, LANG_US)
            For i = 0 To UBound(b)
                isOk = False
                If b(i) = 20 Or b(i) = 9 Then isOk = True
                If b(i) >= a And b(i) <= Z Then isOk = True
                If b(i) >= zero And b(i) <= nine Then isOk = True
                If Not isOk Then
                    If AnyofTheseInstr(Chr(b(i)), "~`!@#$%^&*()_+-={}[]|\;:<>,./?""'", vbBinaryCompare, "") Then isOk = True
                End If
                If Not isOk Then
                    Exit For
                End If
            Next
            If i = UBound(b) + 1 And isOk Then
                If InStr(tmp, """") > 0 Then tmp = Replace(tmp, """", "\x22") 'so we dont break js quoted strings..
                If InStr(tmp, "'") > 0 Then tmp = Replace(tmp, "'", "\x27")
                txtJS.ReplaceAll m.value, tmp, True
            End If
        End If
        
    Next
    
    Form2.txtJS.FirstVisibleLine = topLine
    
End Sub

Private Sub mnuSc2ExeMain_Click()
    'built in shellcode 2 exe removed because to many AV vendors complain about the husks..not sure how many people use them.
    On Error Resume Next
    Dim h1 As String, h2 As String, x As Long
    h1 = App.path & "\simple_husk.dat"
    h2 = App.path & "\husk.dat"
    If fso.FileExists(h1) And fso.FileExists(h2) Then
        'they have manually installed the husks so we will let them use it..(aka let me use them anyway..)
        x = CLng(InputBox(Replace("Enter which husk index you would like to use:\n\n1: basic husk\n2:advanced husk", "\n", vbCrLf)))
        If Err.Number <> 0 Then Exit Sub
        If x = 0 Or x > 2 Then Exit Sub
        If x = 1 Then x = 0 'use simple husk no wsa
        Shellcode2Exe (x)
    Else
        Shell "cmd /c start http://sandsprite.com/shellcode_2_exe.php"
    End If
End Sub

Function Shellcode2Exe(Index As Long)
    '0 = simple no wsa, 1 = simple w/wsa, 2 = adv husk
    
    On Error Resume Next
    
    Dim pth As String
    Dim f As Long
    Dim Shellcode() As Byte
    Dim husk() As Byte
    Dim hFile As String
    Dim simple_husk As Boolean
    Dim x, i
    
    x = txtJS.SelText
    
    If Len(x) = 0 Then
        MsgBox "No text selected", vbInformation
        Exit Function
    End If
    
    
    'If MsgBox("Do you want to use the simple husk?", vbYesNo + vbQuestion) = vbYes Then
    ' simple_husk = True
    'End If
    
    simple_husk = True
    If Index = 2 Then simple_husk = False
    
    hFile = App.path & IIf(simple_husk, "\simple_husk.dat", "\husk.dat")
    If Not fso.FileExists(hFile) Then
        MsgBox "Husk.exe container was not found did your AV eat it?", vbInformation
        Exit Function
    End If
    
    hFile = fso.ReadFile(hFile)
    
    If simple_husk Then
        hFile = HexStringUnescape(hFile)
        husk() = StrConv(hFile, vbFromUnicode, LANG_US)
        For i = 0 To UBound(husk): husk(i) = husk(i) Xor &H77: Next
    Else
        'husk() = StrConv(hFile, vbFromUnicode, LANG_US)
        hFile = HexStringUnescape(hFile)
        husk() = StrConv(hFile, vbFromUnicode, LANG_US)
        For i = 0 To UBound(husk): husk(i) = husk(i) Xor &H77: Next
    End If
    
    x = PrepareShellcode(x)
    Shellcode() = StrConv(x, vbFromUnicode, LANG_US)
    
    If simple_husk And UBound(Shellcode) > &H1A49 Then
        MsgBox "Shellcode is larger than buffer in husk..may cause errors"
    End If
    
    If Not simple_husk And UBound(Shellcode) > 6000 Then
        MsgBox "Shellcode is larger than buffer in husk..may cause errors"
    End If
    
    pth = dlg.SaveDialog(AllFiles, "shellcode.exe_", , "Save Shellcode Executable As")
    If Len(pth) = 0 Then Exit Function
    
    If Err.Number <> 0 Then
        MsgBox Err.Description
        Exit Function
    End If
    
    f = FreeFile
    Open pth For Binary As f
    Put f, , husk
    
    Dim offset As Long
    
    Select Case Index
        Case 0: offset = &H1000
        Case 1: offset = &H1020
        Case 2: offset = &HC000
    End Select
    
    Dim b As Byte
    If offset = &HC000 Then 'negative fuckers
        Seek f, &H7000
        For i = 0 To &H5000 'this is some stupid shit...
            Get f, , b
        Next
        Put f, , Shellcode()
    Else
        Put f, offset + 1, Shellcode()
    End If
    
    Close
    
    If Err.Number = 0 Then
        MsgBox "File generated successfully...", vbInformation
    Else
        MsgBox Err.Description
    End If
    
End Function

Private Sub mnuSciVerInfo_Click()
     txtJS.ShowAbout
End Sub

Private Sub mnuScriptToClip_Click()
    lblClipboard_Click 0
End Sub

Private Sub mnuSend2IDA_Click()
    Dim h As String, x, scf
    Const def = "C:\Program Files\IDA\idag.exe"
    
    On Error Resume Next
    
    If Len(txtJS.SelText) = 0 Then
        MsgBox "You must first select the shellcode to extract in the script window."
        Exit Sub
    End If
    
    h = GetMySetting("idapath")
    If Len(h) = 0 And fso.FileExists(def) Then h = def 'use default if found
        
    If Len(h) = 0 Or Not fso.FileExists(h) Then
        If MsgBox("You have not yet configured the path to IDA install. select it now?", vbYesNo) = vbNo Then Exit Sub
        h = dlg.OpenDialog(exeFiles, , "Select IDA")
        If fso.FileExists(h) Then
            SaveMySetting "idapath", h
        Else
            Exit Sub
        End If
    End If
    
    x = txtJS.SelText
    x = PrepareShellcode(x) 'does not handle just hex blobs like 9090 other formats supported though with multiescape
    
    scf = App.path & "\tmp.sc"
    If fso.FileExists(scf) Then Kill scf
    fso.writeFile scf, x
    
    Shell h & " """ & scf & """", vbNormalFocus
    
    If Err.Number <> 0 Then MsgBox Err.Description
    
End Sub

Private Sub mnuSeqRenameFuncs_Click()
    
    On Error Resume Next
    Dim fl As Long
    Dim li As ListItem
    Dim li2 As ListItem
    Dim i As Long
    Dim reGens As Long
    Dim ignoreSelected As Boolean
    Dim r As VbMsgBoxResult
    Dim oldname, NewName
    
    i = 1
    fl = txtJS.FirstVisibleLine 'this can be buggy...
    
    r = MsgBox("Ignore Selected functions? ", vbYesNoCancel)
    If r = vbCancel Then Exit Sub
    If r = vbYes Then ignoreSelected = True
    
    For Each li In lvFunc.ListItems
        
        If li.Selected And ignoreSelected Then GoTo nextone
        
        oldname = li.Text
        If oldname = UNK_FUNC Then GoTo nextone
        
reGenerate:
        NewName = "func_" & IIf(i < 10, "0" & i, i)
        i = i + 1
        
        If reGens > 500 Then
            MsgBox "Sorry I had a problem regenerating names in use?", vbInformation
            Exit For
        End If
        
        'see if name already exists..
        For Each li2 In lvFunc.ListItems
            If li2.Text = NewName Then
                reGens = reGens + 1
                GoTo reGenerate
            End If
        Next
        
        'does new name already exist?
        If InStr(txtJS.Text, NewName) > 0 Then
            reGens = reGens + 1
            GoTo reGenerate
        End If
        
        txtJS.Text = Replace(txtJS.Text, oldname, NewName)
        li.Text = NewName
nextone:
        
    Next
    
    txtJS.FirstVisibleLine = fl
    
End Sub

Private Sub mnuShowHelp_Click()
    'toolbox.Help
End Sub

Private Sub mnuStrAryReplace_Click()
    frmAryReplace.Show
End Sub

Private Sub mnuStripInlineDecoderCalls_Click()
    frmInlineDecoderCalls.Show
End Sub

Private Sub mnuUniAsciiToHex_Click()
    txtJS.Text = Replace(txtJS.Text, "\u00", "\x", , , vbTextCompare)
    txtJS.Text = Replace(txtJS.Text, "%u00", "\x", , , vbTextCompare)
End Sub

Private Sub mnuVarPrefix_Click()

    On Error Resume Next
    Dim x, tmp, i
    
    If txtJS.SelLength = 0 Then
        MsgBox "This is used for the refactor form, used to add var to the beginning of all lines selected.", vbInformation
        Exit Sub
    End If
    
    x = txtJS.SelText
    tmp = Split(x, vbCrLf)
    For i = 0 To UBound(tmp)
        tmp(i) = "var " & tmp(i)
    Next
        
    If Err.Number = 0 Then txtJS.SelText = Join(tmp, vbCrLf)


End Sub

Private Sub mnuWordWrap_Click()
    mnuWordWrap.Checked = Not mnuWordWrap.Checked
    txtJS.WordWrap = mnuWordWrap.Checked
End Sub

Private Sub mnuXorBruteForce_Click()
    On Error Resume Next
    Dim infile As String
    Dim x, base
    
    If Len(txtJS.SelText) = 0 Then
        If MsgBox("No Shellcode was selected, do you want to open a file to scan?", vbYesNo) = vbNo Then Exit Sub
        infile = dlg.OpenDialog(AllFiles, , "Open File to Xor Scan")
        If Len(infile) = 0 Then Exit Sub
        x = fso.ReadFile(infile)
    Else
        x = txtJS.SelText
        'does not handle just hex blobs like 9090 other formats supported though with multiescape
        x = PrepareShellcode(x)
    End If
    
    base = App.path & "\xor_bruteforcer"
    If fso.FileExists(base & "\sample.sc") Then Kill base & "\sample.sc"
    
    If Not fso.FileExists(base & "\xorbrute.exe") Then
        MsgBox "Could not find xorbrute.exe in xor_bruteforcer sub directory", vbInformation
        Exit Sub
    End If
    
    fso.writeFile base & "\sample.sc", x
    Shell """" & base & "\xorbrute.exe"" " & base & "\sample.sc", vbNormalFocus
    
End Sub

'splitter code
'------------------------------------------------
Private Sub splitter_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim a1&

    If Button = 1 Then 'The mouse is down
        If Capturing = False Then
            splitter.ZOrder
            SetCapture splitter.hwnd
            Capturing = True
        End If
        With splitter
            a1 = .Top + Y
            If MoveOk(a1) Then
                .Top = a1
            End If
        End With
    End If
End Sub

Private Sub splitter_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    If Capturing Then
        ReleaseCapture
        Capturing = False
        DoMove
    End If
End Sub


Private Sub DoMove()
    On Error Resume Next
    Dim tw As Integer 'Twips Width
    Dim th As Integer 'Twips Height
    tw = Screen.TwipsPerPixelX
    th = Screen.TwipsPerPixelY
    Const buf = 30
    txtOut.Top = splitter.Top + splitter.Height + buf
    txtOut.Height = Me.Height - txtOut.Top - (th * 60)
    DukDbg.Height = splitter.Top - DukDbg.Top - buf
    lblClipboard(2).Top = txtOut.Top
    lblClipboard(1).Top = lblClipboard(2).Top + 400
End Sub


Private Function MoveOk(Y&) As Boolean  'Put in any limiters you desire
    MoveOk = False
    If Y > 2500 And Y < Me.Height - 1500 Then
        MoveOk = True
    End If
End Function

'------------------------------------------------
'end splitter code

'Private Sub cmdRun_Click()
'    On Error Resume Next
'
'    Dim user_lib As String
'    Dim main_wrapper As String
'
'    user_lib = App.path & "\userlib.js"
'    main_wrapper = App.path & "\myMain.js"
'
'    toolbox.ResetAlertCount
'
'    USING_MYMAIN = False
'    If fso.FileExists(user_lib) And fso.FileExists(main_wrapper) Then USING_MYMAIN = True
'
'    If chkNoResest.value = 0 Or objsAdded = False Then
'        lv2.ListItems.Clear
'        objsAdded = True
'        sc.Reset
'
'        If Not USING_MYMAIN Then 'old default support for basic objects..do we need to keep this?
'            toolbox.DebugLog "Not using userlib or myMain falling back to internal toolbox based support for Adobe Objects"
'            sc.AddObject "info", toolbox    'fall back in case userlib or main fuckup...
'            sc.AddObject "media", toolbox
'            sc.AddObject "spell", toolbox
'            sc.AddObject "util", toolbox
'            sc.AddObject "this", toolbox
'            sc.AddObject "Collab", toolbox
'        End If
'
'        sc.AddObject "tb", toolbox
'        sc.AddObject "toolbox", toolbox
'        sc.AddObject "txtOut", txtOut
'
'        If fso.FileExists(user_lib) Then
'            sc.AddCode fso.ReadFile(user_lib)
'        End If
'
'    End If
'
'
'    'sc.Timeout = IIf(chkDebug.value = 1, -1, 10000) ' we should verify a JIT script debugger is setup on system..
'
'    If USING_MYMAIN = True Then
'        main_wrapper = fso.ReadFile(main_wrapper)
'        'main_wrapper = Replace(main_wrapper, "//real script here", IIf(chkDebug.value = 1, "debugger" & vbCrLf, "") & txtjs.text)
'        main_wrapper = Replace(main_wrapper, "//real script here", txtjs.Text)
'        sc.AddCode main_wrapper
'    Else
'        sc.AddCode IIf(chkDebug.value = 1, "debugger" & vbCrLf, "") & txtjs.Text
'    End If
'
'End Sub



 

Private Sub Form_Load()
    On Error Resume Next
    
    Set txtJS = DukDbg.sci
    If txtJS Is Nothing Then MsgBox "Failed to set txtjs!"
    
    'DukDbg.AddLibFile "C:\Documents and Settings\david\Desktop\pre.js"
    
    mnuPopup.Visible = False
    mnuPopup3.Visible = False
    mnuPopupFuncs.Visible = False
    mnuMainPopup.Visible = False
    txtJS.ContextMenu = False
    
    mnuWordWrap.Checked = IIf(GetMySetting("WordWrap", 1) = 1, True, False)
    mnuIndentGuide.Checked = IIf(GetMySetting("IndentGuide", 0) = 1, True, False)
    mnuCodeFolding.Checked = IIf(GetMySetting("CodeFolding", 0) = 1, True, False)
   
    txtJS.WordWrap = mnuWordWrap.Checked
    txtJS.ShowIndentationGuide = mnuIndentGuide.Checked
    txtJS.Folding = mnuCodeFolding.Checked
    txtJS.DisplayCallTips = True
   
    lvFunc.ColumnHeaders(1).Width = lv.Width - 800
    lv.ColumnHeaders(1).Width = lv.Width - 200
    FormPos Me, True
    splitter.Top = GetMySetting("SplitterTop", splitter.Top)
    Form_Resize
    DoMove
   
    Dim jsapi As String
    jsapi = App.path & "\scripts\js_api.txt"
    If fso.FileExists(jsapi) Then
        Dim apiLoaded As Long
        apiLoaded = txtJS.LoadCallTips(jsapi)
        Debug.Print "JSApi loaded: " & apiLoaded & " path: " & jsapi
    End If
    
    jsapi = App.path & "\scripts\jsunpack_pre.js"
    If fso.FileExists(jsapi) Then DukDbg.AddLibFile jsapi
    
    DukDbg.userCOMDir = App.path & "\scripts\COM\"
    If DukDbg.AddObject(toolbox, "tb") Then
        DukDbg.AddIntellisense "tb", "GetListviewData DebugLog alert t Save2Clipboard GetClipboard writeFile HexDump ReadFile unescape HexString2Bytes EscapeHexString"
    Else
        Debug.Print "Failed to add toolbox class?"
    End If
        
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    Dim tw As Integer 'Twips Width
    Dim th As Integer 'Twips Height
    tw = Screen.TwipsPerPixelX
    th = Screen.TwipsPerPixelY
    
    txtOut.Width = Me.Width - txtOut.left - (tw * 20) '300
    DukDbg.Width = txtOut.Width
    txtOut.Height = Me.Height - txtOut.Top - (th * 60) '750
    
    lv2.Top = Me.Height - lv2.Height - (th * 60) '750
    lvFunc.Height = Me.Height - lv2.Height - lvFunc.Top - (th * 60)
    'lvFunc.Top = lv2.Top - lvFunc.height '- (th * 60)   '750
    'lv.height = lvFunc.Top - lv.Top - (tw * 10)  '25
    
    'lv.Height = Me.Height - lv.Top - 700
    splitter.Width = txtJS.Width
    
    If Me.WindowState = vbMinimized Then Exit Sub
    DoMove
     
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    FormPos Me, True, True
    SaveMySetting "SplitterTop", splitter.Top
    SaveMySetting "WordWrap", IIf(mnuWordWrap.Checked, 1, 0)
    SaveMySetting "IndentGuide", IIf(mnuIndentGuide.Checked, 1, 0)
    SaveMySetting "CodeFolding", IIf(mnuCodeFolding.Checked, 1, 0)
End Sub


Public Sub SaveToListView(data As String, Optional nameAs As String)
    Dim li As ListItem
    On Error Resume Next
    If Len(nameAs) = 0 Then nameAs = (lv.ListItems.count + 1) & " len - " & Len(txtJS.Text)
    Set li = lv.ListItems.Add(, , nameAs)
    li.Tag = data
    li.ToolTipText = data
End Sub

Private Sub lblClipboard_Click(Index As Integer)
    
    Dim li As ListItem
    On Error Resume Next
    
    Select Case Index
        Case 0: SaveToListView txtJS.Text
        Case 1: SaveToListView txtOut.Text
        Case 2:
            txtJS.Text = txtOut.Text
            txtOut.Text = Empty
            Erase renames
    End Select
    
End Sub

Private Sub lv_DblClick()
    If lv.SelectedItem Is Nothing Then Exit Sub
    txtJS.Text = lv.SelectedItem.Tag
    mnuFunctionScan_Click
End Sub


Private Sub lv_KeyDown(KeyCode As Integer, Shift As Integer)
    
    'MsgBox KeyCode & " " & Shift
    'Exit Sub
    
    Dim li As ListItem
    Dim i As Long, x
    
    If KeyCode = 82 And Shift = 2 Then 'ctrl-r - rename
        For Each li In lv.ListItems
            If li.Selected Then
                x = InputBox("Rename entry " & li.Text, , li.Text)
                If Len(x) > 0 Then li.Text = x
            End If
        Next
    End If
    
    If KeyCode = 65 And Shift = 2 Then 'ctrl-a - select all
        For Each li In lv.ListItems
            li.Selected = True
        Next
    End If
    
    If KeyCode = 73 And Shift = 2 Then 'ctrl-i - invert selection
        For Each li In lv.ListItems
            li.Selected = Not li.Selected
        Next
    End If
    
    If KeyCode = 68 And Shift = 2 Then 'ctrl-d - delete selected
        If MsgBox("Are you sure you want to deleted the selected entries?", vbYesNo) = vbYes Then
            For i = lv.ListItems.count To 1 Step -1
                If li.Selected = True Then
                    lv.ListItems.Remove i
                End If
            Next
        End If
    End If
    
    If KeyCode = 78 And Shift = 2 Then 'ctrl-n -select none
        For Each li In lv.ListItems
            li.Selected = False
        Next
    End If
    
End Sub


Private Sub lv_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    If Button = 2 Then PopupMenu mnuPopup
End Sub

Private Sub mnuAddPercentoHexString_Click()
    
    On Error Resume Next
    Dim t As String
    Dim b() As Byte
    Dim c As String, i As Long
    
    t = txtJS.SelText
    If Len(t) = 0 Then
        MsgBox "No selection", vbInformation
        Exit Sub
    End If
    
    t = HexStringUnescape(t, True)
    b() = StrConv(t, vbFromUnicode, LANG_US)
    t = Empty
    
    For i = 0 To UBound(b)
        c = Hex(b(i))
        If Len(c) = 1 Then c = "0" & c
        t = t & "%" & c
    Next
    
    txtJS.SelText = t
    
End Sub

Private Sub mnuBasicRefactor_Click()
    
    On Error GoTo hell
    Const debugMode As Boolean = True
    
    If InStr(1, txtJS.Text, vbCrLf & "}" & vbCrLf) < 1 Then
        MsgBox "This assumes you already ran Format_Javascript", vbInformation
        'Exit Sub
    End If
    
    frmRefactor.LoadFunctions txtJS.Text, debugMode
     
    Exit Sub
hell:
       MsgBox "Error in mnuBasicRefactor_Click() original was saved to left list probably have to restore from there", vbInformation
    
End Sub



Private Sub mnuBeautify_Click()
    
    On Error Resume Next
    Dim js As String
    Dim c As New Collection
    Dim rv
    
    'txtjs.text = "a=0;if(a){a++;}else{a++;}a=0;a=0"
    
    Set duk = New CDukTape
    tmrFormatting.Enabled = True
    If Not duk.AddObject(txtJS, "textbox") Then
        Exit Sub
    End If
    
    If Not duk.AddFile(App.path & "\scripts\beautify.js") Then
        MsgBox "Could not add beautify.js Error: " & duk.LastError
        Exit Sub
    End If
    
    rv = duk.eval("js_beautify(textbox.Text, {indent_size: 1, indent_char: '\t'}).split('\n').join('\r\n');")
    
    If duk.hadError Then
        MsgBox "Error running beautify: " & duk.LastError
    Else
        txtJS.Text = rv
    End If
    
    tmrFormatting.Enabled = False
    
    mnuFunctionScan_Click

End Sub

Private Sub mnuClearAll_Click()
    
    If MsgBox("Are you sure you want to delete all items?", vbYesNo) = vbYes Then
        lv.ListItems.Clear
    End If
        
End Sub

'Private Sub mnuExploitScan_Click()
'
'    Dim report() As String
'    Dim p() As String
'    Dim exploits()
'    Dim offset As Long
'
'    On Error Resume Next
'
'    exploits = Form1.GetExploits()
'
'    For i = 0 To UBound(exploits)
'            p() = Split(exploits(i), "=")
'            If ContainsExploit(txtjs.text, p(1), offset) Then
'                push report, "Exploit " & p(0) & " - " & p(1) & " - found in main textbox"
'                txtjs.selstart = offset - 1
'                'txtjs.sellength = Len(p(1))
'
'                'vbsci specific
'                report(UBound(report)) = report(UBound(report)) & " Line: " & txtJS.CurrentLine
'
'                'txtJS.SelColor = vbBlue
'                'txtJS.SelBold = True
'                'txtjs.sellength = 0
'
'            End If
'    Next
'
'    If InStr(txtjs.text, "import") > 0 Then  'its an as3 sourcer decompilation of a flash file?
'        push report, vbCrLf & "Flash CVE Scan: " & vbCrLf & flash_as_cveScan(txtjs.text)
'    End If
'
'    If AryIsEmpty(report) Then
'        MsgBox "No exploits found", vbInformation
'        Exit Sub
'    End If
'
'    push report, vbCrLf & "Other exploits may be hidden w/ obsfuscation"
'    push report, "May also be used innocently."
'
'    txtOut.Text = Join(report, vbCrLf)
'
'    'Dim tmp As String
'    'tmp = fso.GetFreeFileName(Environ("temp"))
'    'fso.WriteFile tmp, Join(report, vbCrLf)
'    'Shell "notepad.exe """ & tmp & """", vbNormalFocus
'
'
'End Sub

 

Private Sub mnuLaunchSclog_Click(Index As Integer)
    
    'If Len(txtjs.seltext) = 0 Then
    '    MsgBox "You must first select the shellcode to extract in the script window."
    '    Exit Sub
    'End If
    Dim x
    
    x = txtJS.SelText
    
    'does not handle just hex blobs like 9090 other formats supported though with multiescape
    
    x = PrepareShellcode(x)
    
    If Index = 0 Then
        'frmSclog.InitInterface CStr(x)
    Else
        frmScTest.InitInterface CStr(x)
    End If
    
End Sub

Private Sub mnuScSigs_Click()
    On Error Resume Next
    
    'If Len(txtjs.seltext) = 0 Then
    '    MsgBox "You must first select the shellcode to extract in the script window."
    '    Exit Sub
    'End If
    
    Dim x, base
    
    x = txtJS.SelText
    
    'does not handle just hex blobs like 9090 other formats supported though with multiescape
    
    x = PrepareShellcode(x)
    
    base = App.path & "\libemu"
    If fso.FileExists(base & "\sample.sc") Then Kill base & "\sample.sc"
    
    If Not fso.FileExists(base & "\scsigs.exe") Then
        MsgBox "Could not find scsigs.exe in libemu directory", vbInformation
        Exit Sub
    End If
    
    fso.writeFile base & "\sample.sc", x
    Shell """" & base & "\scsigs.exe"" " & base & "\sample.sc", vbNormalFocus
    
End Sub


Private Function PrepareShellcode(x) As String
    Dim orglen
    
    If InStr(x, """") > 0 Then x = Replace(x, """", Empty)
    If InStr(x, vbCrLf) > 0 Then x = Replace(x, vbCrLf, Empty)
    If InStr(x, vbCr) > 0 Then x = Replace(x, vbCr, Empty)
    If InStr(x, vbLf) > 0 Then x = Replace(x, vbLf, Empty)
    If InStr(x, vbTab) > 0 Then x = Replace(x, vbTab, Empty)
    If InStr(x, "+") > 0 Then x = Replace(x, "+", Empty)
    If InStr(x, "'") > 0 Then x = Replace(x, "'", Empty)
    If InStr(x, " ") > 0 Then x = Replace(x, " ", Empty)
    
    If InStr(x, "\u") > 0 Then x = Replace(x, "\u", "%u") 'new to me 6.24.13
    
    orglen = Len(x)
    x = MultiEscape(x)
    If Len(x) = orglen Then 'nothing happened, is it a hexchar string?
        x = HexStringUnescape(x, , , True)
    End If
    PrepareShellcode = x
    
End Function

Public Sub mnuLoadFile_Click()
    Dim f As String
    dlg.SetCustomFilter "Javascript File (*.js)", "*.js"
    f = dlg.OpenDialog(CustomFilter, "", "Open file")
    If Len(f) = 0 Then Exit Sub
    If Not fso.FileExists(f) Then Exit Sub
    txtJS.Text = fso.ReadFile(f)
    mnuFunctionScan_Click
    Erase renames
End Sub

Private Sub mnuManualEscape_Click(Index As Integer)
    
    Dim t As String
    Dim ss As Long
    On Error Resume Next
    
    t = txtJS.SelText
    ss = txtJS.SelStart
    
    If Len(t) = 0 Then
        MsgBox "No text selected", vbInformation
        Exit Sub
    End If
    
    Select Case Index
        Case 0: t = HexStringUnescape(t)
        Case 1: t = HexStringUnescape(t, True)
        Case 2: t = unescape(t)
        Case 3: t = js_unescape(t)
        Case 4: t = nl_unescape(t)
        Case 5: t = pound_unescape(t)
        Case 6: t = octal_unescape(t)
    End Select
    
    txtJS.SelText = t
    txtJS.SelStart = ss
    txtJS.SelLength = Len(t)
    
End Sub

Private Sub mnuRemoveEntry_Click()
    
    If lv.SelectedItem Is Nothing Then Exit Sub
    lv.ListItems.Remove lv.SelectedItem.Index
    
End Sub

Private Sub mnuRenameScript_Click()
    Dim x
    
    If lv.SelectedItem Is Nothing Then Exit Sub
    
    x = InputBox("Rename script", , lv.SelectedItem.Text)
    If Len(x) = 0 Then Exit Sub
    lv.SelectedItem.Text = x
    
End Sub

Private Sub mnuReplace_Click()
    On Error Resume Next
    txtJS.ShowFindReplace
End Sub


Private Sub mnuSaveAll_Click()
    
    Dim li As ListItem
    Dim f As String
    On Error GoTo hell
    
    f = dlg.FolderDialog()
    If Len(f) = 0 Then Exit Sub
    
    For Each li In lv.ListItems
        fso.writeFile f & "\script_" & li.Index & ".js", li.Tag
    Next
    
    Exit Sub
hell:     MsgBox Err.Description
    
End Sub

Private Sub mnuSaveShellcode_Click()
    Dim x
    
    x = txtJS.SelText
    If Len(x) = 0 Then
        MsgBox "No text selected", vbInformation
        Exit Sub
    End If
    
    x = PrepareShellcode(x)
    
    Dim pth As String
    pth = dlg.SaveDialog(AllFiles, RecommendedName("bytes.sc"), , "Save shellcode as")
    If Len(pth) = 0 Then Exit Sub
    
    fso.writeFile pth, x
    
End Sub

Private Function RecommendedName(Optional defaultName) As String
    On Error Resume Next
    RecommendedName = defaultName
End Function

Private Sub mnuSaveToFile_Click()

    Dim li As ListItem
    Dim f As String
    On Error GoTo hell
    
    If lv.SelectedItem Is Nothing Then Exit Sub
    
    f = dlg.SaveDialog(AllFiles, "script_" & lv.SelectedItem.Index & ".js", "", "Save file")
    If Len(f) = 0 Then Exit Sub
    
    fso.writeFile f, lv.SelectedItem.Tag
    
    Exit Sub
hell:     MsgBox Err.Description
End Sub

Private Sub mnuSimplifySelection_Click()
    Dim x, ss
    
    '"p"+"ar"+"ent"+""
    'effata+'A'+'B'+'C'+cruore+'E'+eumque+effata
    On Error Resume Next
    ss = txtJS.SelStart
    x = txtJS.SelText
    x = Replace(x, "'", """") 'unify
    x = Replace(x, """ + """, Empty) 'remove " + "
    x = Replace(x, "+""""", Empty) 'remove + empty
    x = Replace(x, """+""", Empty) 'remove "+"
    
    txtJS.SelText = x
    txtJS.SelStart = ss
    txtJS.SelLength = Len(x)
    txtJS.SetFocus

End Sub

Private Sub mnuUnescapeSelection_Click()
    
    On Error Resume Next
    
    If txtJS.SelLength = 0 Then Exit Sub
    Dim t As Object
    
    Set t = txtJS
    
    If InStr(t.SelText, "%u") > 0 Then
        t.SelText = unescape(t.SelText)
    End If
    
    If InStr(t.SelText, "#") > 0 Then
        t.SelText = pound_unescape(t.SelText)
    End If
    
    If InStr(t.SelText, "\x") > 0 Then
        t.SelText = js_unescape(t.SelText)
    End If
    
    If InStr(t.SelText, "\n") > 0 Then
        t.SelText = nl_unescape(t.SelText)
    End If
    
   MsgBox "todo:mnuUnescapeSelection"
    'sc2.Reset
    'sc2.AddObject "txtJs", txtJS, True
    'sc2.AddCode "txtjs.seltext = unescape(txtjs.seltext)"
    
    
    
End Sub



Private Sub tmrFormatting_Timer()
    DoEvents
    Sleep 10
End Sub

Private Sub lvFunc_ItemClick(ByVal Item As MSComctlLib.ListItem)
    Dim word As String
    word = Item.Text
    Me.Caption = "  " & txtJS.hilightWord(word, , vbBinaryCompare) & " instances of '" & word & " ' found"
End Sub

Function isWordFunctionName(word As String) As Boolean
    Dim li As ListItem
    For Each li In lvFunc.ListItems
        If li.Text = word Then
            isWordFunctionName = True
            Exit For
        End If
    Next
End Function

Function selectFunction(name As String)
    Dim li As ListItem
    For Each li In lvFunc.ListItems
        If li.Text = name Then li.Selected = True Else li.Selected = False
    Next
End Function

Private Sub txtjs_KeyDown(KeyCode As Long, Shift As Long)
    If KeyCode = 117 Then 'f6
        If txtJS.SelLength > 0 Then mnuUnescapeSelection_Click
    End If
End Sub

Private Sub txtJS_MouseUp(Button As Integer, Shift As Integer, x As Long, Y As Long)
    
    On Error Resume Next
    
    Dim word As String 'word mouse is currently over..
    Dim isFuncName As Boolean

    If Button = 2 Then
        word = txtJS.CurrentWord
        'Me.Caption = word
        isFuncName = isWordFunctionName(word)
        If isFuncName Then selectFunction word
        mnuMain(0).Visible = isFuncName 'jump to
        mnuMain(2).Visible = isFuncName 'graph to
        mnuMain(3).Visible = isFuncName 'graph from
        mnuMain(6).Visible = IsNumeric(word)
        PopupMenu mnuMainPopup
    End If
    
End Sub

'everytime first visible line changes..
Private Sub txtjs_LineChanged(Position As Long)

On Error Resume Next

    Dim p As POINTAPI, txtJSScrollBarPos As Long, tmpPoints() As Long, i As Long
    
    'dont track position changes if they are scrolling the editor window
    'detected as having mouse over scroll bar region and position changing..
    If GetCursorPos(p) <> 0 Then
        If ScreenToClient(Me.hwnd, p) <> 0 Then
            txtJSScrollBarPos = (txtJS.left + txtJS.Width) / 15
            'Me.Caption = p.x & " " & txtJSScrollBarPos
            If Abs(txtJSScrollBarPos - p.x) < 40 Then Exit Sub
        End If
    End If
        
    'this means they just hit escape to nav back so ignore this change too..
    If ignoreNavPoint Then
        ignoreNavPoint = False
        Exit Sub
    End If
    
    If Abs(curTopLine - Position) > 10 Then
        
        push navPoints, curTopLine
        curTopLine = Position
        'Debug.Print Now & "new pos saved: " & Position
        
        If UBound(navPoints) > 70 Then
            'Debug.Print "compacting navpoints.."
            navPoints = SaveTopXElements(navPoints, 45)
        End If
        
    End If
    
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
    Dim lastPos As Long
    If KeyAscii = 27 Then 'escape key pressed
        lastPos = pop(navPoints)
        If lastPos <> 0 Then
            ignoreNavPoint = True
            'Debug.Print "nav back to: " & lastPos
            'txtJS.GotoLine lastPos
            txtJS.FirstVisibleLine = lastPos
        End If
    End If
End Sub

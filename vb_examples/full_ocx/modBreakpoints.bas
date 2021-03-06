Attribute VB_Name = "modBreakpoints"
'Author: David Zimmer <dzzie@yahoo.com>
'Site: Sandsprite.com
'License: http://opensource.org/licenses/MIT

Option Explicit
Public breakpoints As New Collection
 
'here are the behaviors of duktape for setting breakpoints:
'    bp on an empty line will stop on next line
'    bp on a function (){ start will break on next line
'    bp on a comment or multiline comment will break at next line
'    bp on a function close brace will set, but never hit. <--
'    bp on close brace of a if state breaks on next line
'    bp on a single line function with multiple statements will hit once (step into only steps once and all bypass)

'this next function is used for run to line functionality..some basic tests..
Function isExecutableLine(sourceText As String) As Boolean
    Dim tmp As String
    On Error Resume Next
    
    tmp = LCase(sourceText)
    tmp = Trim(Replace(tmp, vbTab, Empty))
    tmp = Replace(tmp, vbCr, Empty)
    tmp = Replace(tmp, vbLf, Empty)
    If Len(tmp) = 0 Then GoTo fail
    If tmp = "}" Then GoTo fail  'is end function/end if
    If Left(tmp, 1) = "/" Then GoTo fail 'is comment

    isExecutableLine = True
Exit Function
fail: isExecutableLine = False
End Function

Public Function BreakPointExists(FileName As String, lineNo As Long, ctl As ucDukDbg, Optional b As CBreakpoint, Optional colIndex As Long) As Boolean

    On Error Resume Next
    colIndex = 1
    For Each b In breakpoints
        If b.lineNo = lineNo And b.FileName = FileName And b.owner = ObjPtr(ctl) Then
            BreakPointExists = True
            Exit Function
        End If
        colIndex = colIndex + 1
    Next
    
    colIndex = 0
    
End Function

Public Sub ToggleBreakPoint(FileName As String, lineNo As Long, sourceText As String, ctl As ucDukDbg)
  
    If BreakPointExists(FileName, lineNo, ctl) Then
        RemoveBreakpoint FileName, lineNo, ctl
    Else
        SetBreakpoint FileName, lineNo, sourceText, ctl
    End If

End Sub

'file name is case sensitive!
'sooo we need a live context to actually set breakpoints..but we can store them
'at design time, and then on initial debugger startup make sure to cycle through set breakpoints to initial set..
Public Function SetBreakpoint(ByVal FileName As String, lineNo As Long, ByVal sourceText As String, ctl As ucDukDbg) As Boolean
    Dim b As CBreakpoint
    
    If BreakPointExists(FileName, lineNo, ctl, b) Then
        b.sourceText = sourceText 'just in case it changed..
        SetBreakpoint = True
        Exit Function
    End If
    
    Set b = New CBreakpoint
    
    With b
        .FileName = FileName
        .lineNo = lineNo
        .sourceText = sourceText
        .owner = ObjPtr(ctl)
    End With
    
    If running Then
        If Not SyncSetBreakPoint(b) Then
            dbg "Failed to set breakpoint: " & b.Stats
            Exit Function
        Else
            b.isSet = True
        End If
    End If

    breakpoints.Add b
    If ctl.CurrentFile = FileName Then ctl.sci.SetMarker lineNo
    SetBreakpoint = True
    
    
End Function

Public Sub RemoveBreakpoint(FileName As String, lineNo As Long, ctl As ucDukDbg)
    Dim b As CBreakpoint
    Dim colIndex As Long
    Dim cur_b As CBreakpoint
    
    If Not BreakPointExists(FileName, lineNo, ctl, b, colIndex) Then Exit Sub
    
    If running Then
        If Not SyncDelBreakPoint(b) Then
            dbg "Failed to delete bp from duktape?: " & b.Stats
            Exit Sub
        End If
        
        'we have to compact our duktape bp indexes - technically we should call relist...
        'todo: test this against multiple instances of form..(bp index is set per duk heap)
        For Each cur_b In breakpoints
            If ObjPtr(ctl) = b.owner Then
                If cur_b.index > b.index Then
                    cur_b.index = cur_b.index - 1
                End If
            End If
        Next
    End If
        
    If ctl.CurrentFile = FileName Then ctl.sci.DeleteMarker lineNo
    
    breakpoints.Remove colIndex
    
End Sub
'
'Sub ClearUIBreakpoints()
'    Dim b As CBreakpoint
'    For Each b In breakpoints
'        frmMain.scivb.DeleteMarker b.lineNo
'    Next
'End Sub
'
Sub RemoveAllBreakpoints(ctl As ucDukDbg)
    Dim b As CBreakpoint
    For Each b In breakpoints
        If b.owner = ObjPtr(ctl) Then RemoveBreakpoint b.FileName, b.lineNo, ctl
    Next
End Sub

 

'called on debugger startup when first message received..
'assumes only single source file ansd is still current..! todo:
Sub InitDebuggerBpx(ctl As ucDukDbg)
    Dim b As CBreakpoint
    For Each b In breakpoints
        If Not b.isSet Then
            If ctl.CurrentFile = b.FileName And b.owner = ObjPtr(ctl) Then
                If b.sourceText = ctl.sci.GetLineText(b.lineNo) Then
                     If Not SyncSetBreakPoint(b) Then
                        dbg "InitDebuggerBpx: Failed to set bp" & b.Stats
                     End If
                     If Len(b.errText) > 0 Then dbg b.Stats
                End If
            Else
                If Not SyncSetBreakPoint(b) Then
                    dbg "Failed to set breakpoint: " & b.Stats
                End If
            End If
        End If
    Next
End Sub



<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl" language="VBScript"><xsl:template match="/"><TABLE cellpadding="0" cellspacing="0"><xsl:for-each select="ServiceData/BankStatements">
				<xsl:script language="VBScript"><![CDATA[
  Dim TotalItemsCounter:  
  TotalItemsCounter = 0:  
  Function AddToItemsCounter  
    TotalItemsCounter = TotalItemsCounter + 1: 
  End Function: 
    
  Function GetItemNumber() :
    GetItemNumber = TotalItemsCounter: 
  End Function: 
    
  Function isLastNode(oNode) : 
    If oNode.nextSibling is nothing then 
      isLastNode = false:
    else 
      isLastNode = true:
    End If :
  End Function

  Dim bNoDataFound:  
  bNoDataFound = True:  
  Function RaiseFlagDataFound : 
    bNoDataFound = False:  
  End Function:
  
  Function NoDataWasFound : 
    NoDataWasFound = bNoDataFound :  
  End Function

  Dim bPreSubmit:  
  bPreSubmit = False:  
  Function RaiseFlagPreSubmit : 
    bNoDataFound = False:  
    bPreSubmit = True:  
  End Function:
  
  Function NotInPreSubmit : 
    NotInPreSubmit = not bPreSubmit :  
  End Function

  Function BodyImgSwitch(sSwitchStatus): 
    Dim oSrcImage:  
    Dim oNewImage:  
    set oNewImage = Nothing:  
    set oSrcImage = window.event.srcElement:  
    select case ucase(sSwitchStatus):  
      case "OVER":  
        set oNewImage = document.all(oSrcImage.id & "_R"):  
      case "OUT":  
        set oNewImage = document.all(oSrcImage.id & "_S"):  
      case "DOWN":  
        set oNewImage = document.all(oSrcImage.id & "_P"):  
      case "UP":  
        set oNewImage = document.all(oSrcImage.id & "_S"):  
    end select:  
    if not oNewImage is nothing then  
      oSrcImage.src = oNewImage.src:  
    end if:  
  End Function

  Function ItemHasChildNodes (oNode,sChildrenNames) 
    Dim sChild: 
    Dim bChildExists: 
    bChildExists = false: 
    for each sChild in split(sChildrenNames,",")
      If not oNode.selectSingleNode(".//" & sChild & "") is nothing then
        bChildExists = true: 
      End If:
    next:
    ItemHasChildNodes = bChildExists :
  End Function
]]></xsl:script><xsl:if test="@bFirstPage"/><xsl:if test="context()[0]"><xsl:if test="@bRenderScripts"><SCRIPT language="VBScript"><xsl:comment><![CDATA[
Sub ShowHideDataTableLines():
  On Error Resume Next:
  Dim objChildline:
  Dim objRow:
  Dim iCountRows:
  Dim i:
  Dim showHideLineId:
  Dim bOpenTable:
  Dim sSameType:
  Set objRow = window.event.srcElement:
  While UCase(objRow.tagName) <> "TR"
    Set objRow = objRow.parentNode:
  Wend:
  If objRow.openClose = "OPEN" Then
    objRow.openClose = "CLOSE":
    window.event.srcElement.src = "../../images/but_colapse_table.gif":
    bOpenTable = true:
  Else
    objRow.openClose = "OPEN":
    window.event.srcElement.src = "../../images/but_expend_table.gif":
    bOpenTable = false:
  End If:
  Set objChildline = objRow.nextSibling:
  i = 1:
  iCountRows = 0:
  If Not objChildline Is Nothing Then
    sSameType = objChildline.INLINE:
    While Not objChildline Is Nothing And i = 1 And sSameType <> "Exit"
       If objChildline.Id = objRow.Id Then
         i = 0:
       Else
        If UCase(objChildline.INLINE) = "FALSE" Then
          CopyDataTableContents objChildline.Id, CInt(objChildline.TARGETID), bOpenTable:
        Else
          If lcase(objChildline.Style.display) = "none" Then
            objChildline.Style.display = "block":
            If objChildline.all(objChildline.Id & "_SOURCE").style.display = "none" then
              objChildline.all(objChildline.Id & "_SOURCE").style.display = "block":
            End If:
          Else
            objChildline.Style.display = "none":
          End If:
        End If:
        Set objChildline = objChildline.nextSibling:
        If sSameType <> objChildline.INLINE Then sSameType = "Exit":
      End If:
    Wend:
  End If:
End Sub:
]]></xsl:comment></SCRIPT><SCRIPT language="VBScript"><xsl:comment><![CDATA[
  Sub SortTableByColumn(sForEach, sNewSort): 
    Dim oScript: 
    For Each oScript In document.scripts: 
      If instr(Ucase(oScript.innerHtml),"SORTCOLUMNSINDATA") > 0 then 
        SortColumnsInData sForEach, sNewSort:
      End If:
    next:
  End Sub:
]]></xsl:comment></SCRIPT><SCRIPT language="VBScript"><xsl:comment><![CDATA[
  Sub CollapseTable(): 
  Dim srcItem
  Dim destItem
  Dim masterRowId
  Set srcItem = window.event.srcElement
  While UCase(srcItem.tagName) <> "TR"
    Set srcItem = srcItem.parentNode
  Wend
  Set destItem = srcItem.nextSibling
  Do While Not destItem Is Nothing
    If masterRowId = "" And destItem.dataRow = "TRUE" Then
      masterRowId = destItem.Id
    End If
     If masterRowId <> "" And destItem.dataRow = "TRUE" Then
      If masterRowId <> destItem.Id Then
        destItem.Style.display = "none"
      Else
        If srcItem.openClose = "OPEN" Then
          destItem.Style.display = "none"
          destItem.openClose = "CLOSE"
        Else
          destItem.Style.display = ""
          destItem.openClose = "OPEN"
        End If
      End If
    Else
      If srcItem.openClose = "OPEN" Then
        destItem.Style.display = "none"
        destItem.openClose = "CLOSE"
      Else
        destItem.Style.display = ""
        destItem.openClose = "OPEN":
      End If
    End If
    Set destItem = destItem.nextSibling
  Loop
  If srcItem.openClose = "OPEN" Then
    srcItem.openClose = "CLOSE"
    window.event.srcElement.src = "../../images/but_expend_table.gif"
  Else
    srcItem.openClose = "OPEN"
    window.event.srcElement.src = "../../images/but_colapse_table.gif"
  End If
End Sub
]]></xsl:comment></SCRIPT><SCRIPT language="VBScript"><xsl:comment><![CDATA[
Sub myChangeTo(sColor): 
  Dim objRow: 
  Dim invCell:
  Set objRow = window.event.srcElement: 
  While UCase(objRow.tagName) <> "TR"
    Set objRow = objRow.parentNode:
  Wend:
  For Each invCell In objRow.cells:
    invCell.Style.backgroundcolor = sColor
  Next:
  window.event.cancelBubble = true:
End Sub: 
]]></xsl:comment></SCRIPT><SCRIPT language="VBScript"><xsl:comment><![CDATA[
Sub myChangeBack:
  Dim objRow: 
  Dim invCell:
  Set objRow = window.event.srcElement: 
  While UCase(objRow.tagName) <> "TR"
    Set objRow = objRow.parentNode:
  Wend:
  For Each invCell In objRow.cells:
    invCell.Style.backgroundcolor = ""
  Next:
  window.event.cancelBubble = true:
End Sub:

]]></xsl:comment></SCRIPT><SCRIPT language="VBScript"><xsl:comment><![CDATA[
Sub CopyDataTableContents(srcTableId, iTargetNo, bOpenTable):
  Dim srcObj:
  Dim invCell:
  Dim invRow:
  Dim destObj:
  Dim destTableId:
  Dim srcObjRow:
  Dim destObjRow:
  Dim i:
  Dim srcTable:
  Dim prevRow:
  Dim chkItem:
 
  Set srcObj = window.event.srcElement:
  While srcObj.nodeName <> "TR"
    Set srcObj = srcObj.parentNode:
    set srcTable = srcObj:
  Wend:
 
  while srcTable.nodeName <> "TABLE"
    Set srcTable = srcTable.parentNode:
  Wend:
 
  For Each prevRow In srcTable.rows
    If prevRow.Id = srcObj.Id Then
      Set invCell = prevRow.cells(0):
      If invCell.Style.Filter = "invert" Then
        prevRow.openClose = "OPEN":
        For Each invCell In prevRow.cells
          invCell.Style.Filter = "clear":
          For Each chkItem In invCell.children
            If chkItem.children.Length > 0 Then
              If chkItem.children(0).nodeName = "IMG" Then
                If chkItem.children(0).className = "COLLAPSE" Then
                  chkItem.children(0).src = "../../images/but_expend_table.gif":
                End If:
              End If:
            End If:
          Next:
        Next:
        Exit For:
      End If:
    End If:
  Next:
 
  Set destObj = srcObj:
  For Each invCell In srcObj.cells:
    If bOpenTable Then
      invCell.Style.Filter = "invert":
    Else
      invCell.Style.Filter = "clear":
    End If:
  Next:
  Dim orgId
  orgId = srcObj.Id
  Do While Not srcObj.nextSibling Is Nothing And srcObj.nextSibling.Id <> orgId
    If Not srcObj.nextSibling.All(srcTableId & "_SOURCE") Is Nothing Then
      If TypeName(srcObj.nextSibling.All(srcTableId & "_SOURCE")) = "DispHTMLElementCollection" Then
        Set srcObj = srcObj.nextSibling.All(srcTableId & "_SOURCE")(0):
        Exit Do
      Else
        Set srcObj = srcObj.nextSibling.All(srcTableId & "_SOURCE"):
        Exit Do
      End If:
    Else
      Set srcObj = srcObj.nextSibling
    End If:
  Loop
  destTableId = srcTableId & "_TARGET":
  If TypeName(destObj.All(destTableId)) <> "HTMLTable" Then
    If TypeName(document.All(destTableId)) = "DispHTMLElementCollection" Then
      Set destObj = document.All(destTableId)(iTargetNo):
    Else
      Set destObj = document.All(destTableId):
    End If:
  Else
    Set destObj = destObj.All(destTableId):
    If destObj.rows.length = 1 Then
      If destObj.rows(0).DATAROW = "TRUE" Then
         bCopyData = False:
      End If:
    Else:
      If destObj.rows.length = 0 Then
        bCopyData = True:
      Else
        For i = 0 To destObj.rows.length - 1
          If destObj.rows(i) Is Nothing Then Exit For:
            If destObj.rows(i).DATAROW = "TRUE" Then
              destObj.deleteRow (i):
              i = i - 1:
            End If:
        Next:
        bCopyData = False:
      End If:
    End If:
  End If:
  For i = 0 To destObj.rows.length - 1
    If destObj.rows(i) Is Nothing Then Exit For:
    If destObj.rows(i).DATAROW = "TRUE" Then
      destObj.deleteRow (i):
      i = i - 1:
    End If:
  Next:
  If UCase(destObj.Style.display) = "NONE" Then
    destObj.Style.display = "block"
  Else
    If UCase(destObj.DISPLAY_NONE) = "FALSE" Then
      destObj.Style.display = "none"
    End If
  End If
  If bOpenTable Then
    For Each srcObjRow In srcObj.rows
      If srcObjRow.DATAROW = "TRUE" Then
        Set destObjRow = srcObjRow.cloneNode(True):
        destObj.firstChild.appendChild destObjRow:
      End If:
    Next:
  End If:
End Sub
]]></xsl:comment></SCRIPT></xsl:if></xsl:if><xsl:eval>RaiseFlagDataFound</xsl:eval><TR CHECK_IF_EMPTY="BODY" OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_12" INLINE="FALSE">
					<TD ID="ITEM_13" border="#FFFFFF">
						<DIV align="CENTER">
							<TABLE ID="ITEM_146" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
									<TD>
										<TABLE ID="ITEM_86" DISPLAY_NONE="TRUE" background="DATAMALL_PICTURE:9A050179D4104A05A22922C781ABF59F" width="650" height="100" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_87" INLINE="FALSE">
													<TD ID="ITEM_88" align="LEFT" valign="TOP" width="86" height="100" style=" CURSOR:DEFAULT; ">
														<DIV align="LEFT">
															<TABLE ID="ITEM_89" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
																	<TD>
																		<TABLE ID="ITEM_139" DISPLAY_NONE="TRUE" width="35" height="46" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_140" INLINE="FALSE">
																					<TD ID="ITEM_141" align="LEFT" valign="BOTTOM" width="35" height="46" style=" CURSOR:DEFAULT; ">
																						<DIV align="RIGHT" valign="baseline"><IMG ID="ITEM_412" SRC="DATAMALL_PICTURE:49A5F417359E4D7EAD8CF8DDF0189259" style=" CURSOR:DEFAULT; "/><IMG style="display:none" ID="ITEM_412_S" SRC="DATAMALL_PICTURE:49A5F417359E4D7EAD8CF8DDF0189259"/></DIV></TD></TR></TBODY></TABLE></TD></TR></TABLE>
														</DIV></TD>
													<TD ID="ITEM_91" align="LEFT" valign="BOTTOM" width="564" height="100" style=" CURSOR:DEFAULT; ">
														<DIV align="LEFT">
															<TABLE ID="ITEM_92" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
																	<TD>
																		<TABLE ID="ITEM_95" DISPLAY_NONE="TRUE" width="250" height="73" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_96" INLINE="FALSE">
																					<TD ID="ITEM_97" align="LEFT" width="250" height="19" style=" CURSOR:DEFAULT; ">
																						<DIV align="LEFT">
																							<TABLE ID="ITEM_98" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD>
																										<SPAN style=" FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; FONT-WEIGHT:bold; " ID="ITEM_109" valign="baseline" dir=""><xsl:value-of select="PersonalDetailes/FirstName/text()" disable-output-escaping="yes"/></SPAN>
																										<SPAN style=" FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_111" valign="baseline" dir="LTR">&amp;nbsp;&amp;nbsp;</SPAN>
																										<SPAN style=" FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; FONT-WEIGHT:bold; " ID="ITEM_113" valign="baseline" dir=""><xsl:value-of select="PersonalDetailes/LastName/text()" disable-output-escaping="yes"/></SPAN></TD></TR></TABLE>
																						</DIV></TD></TR><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_100" INLINE="FALSE">
																					<TD ID="ITEM_101" align="LEFT" width="250" height="19" style=" CURSOR:DEFAULT; ">
																						<DIV align="LEFT">
																							<TABLE ID="ITEM_102" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
																									<TD>
																										<TABLE ID="ITEM_115" DISPLAY_NONE="TRUE" width="201" height="18" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_116" INLINE="FALSE">
																													<TD ID="ITEM_117" align="LEFT" width="46" height="18" style=" CURSOR:DEFAULT; ">
																														<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_130" valign="baseline" dir="LTR">Account:</DIV></TD>
																													<TD ID="ITEM_120" align="LEFT" width="65" height="18" style=" CURSOR:DEFAULT; ">
																														<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_132" valign="baseline" dir=""><xsl:value-of select="PersonalDetailes/AccountNo/text()" disable-output-escaping="yes"/></DIV></TD>
																													<TD ID="ITEM_123" align="LEFT" width="40" height="18" style=" CURSOR:DEFAULT; ">
																														<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_134" valign="baseline" dir="LTR">Branch:</DIV></TD>
																													<TD ID="ITEM_126" width="50" height="18" style=" CURSOR:DEFAULT; ">
																														<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_136" valign="baseline" dir=""><xsl:value-of select="PersonalDetailes/Branch/text()" disable-output-escaping="yes"/></DIV></TD></TR></TBODY></TABLE></TD></TR></TABLE>
																						</DIV></TD></TR><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_104" INLINE="FALSE">
																					<TD ID="ITEM_105" align="LEFT" width="250" height="35" style=" CURSOR:DEFAULT; "></TD></TR></TBODY></TABLE></TD></TR></TABLE>
														</DIV></TD></TR></TBODY></TABLE></TD></TR></TABLE>
						</DIV>
						<DIV align="CENTER">
							<TABLE ID="ITEM_318" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
									<TD>
										<TABLE ID="ITEM_147" DISPLAY_NONE="TRUE" background="DATAMALL_PICTURE:386FB7A3E0B745F99291FBBEDD00D8A7" width="650" height="100" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_148" INLINE="FALSE">
													<TD ID="ITEM_149" align="LEFT" width="50" height="100" style=" CURSOR:DEFAULT; "></TD>
													<TD ID="ITEM_152" align="LEFT" valign="TOP" width="550" height="100" style=" CURSOR:DEFAULT; ">
														<DIV align="CENTER">
															<TABLE ID="ITEM_450" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
																	<TD>
																		<TABLE ID="ITEM_160" DISPLAY_NONE="TRUE" ITYPE="DATATABLE" width="500" cellspacing="0" cellpadding="2" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_161" INLINE="FALSE">
																					<TD ID="ITEM_162" align="LEFT" width="80" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; FONT-WEIGHT:bold; " ID="ITEM_283" valign="baseline" dir="LTR">Date</DIV></TD>
																					<TD ID="ITEM_165" width="110" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; FONT-WEIGHT:bold; " ID="ITEM_285" valign="baseline" dir="LTR">Description</DIV></TD>
																					<TD ID="ITEM_168" align="LEFT" width="100" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif;  COLOR:#960018; FONT-WEIGHT:bold; " ID="ITEM_287" valign="baseline" dir="LTR">Debit</DIV></TD>
																					<TD ID="ITEM_171" align="LEFT" width="100" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif;  COLOR:#00693f; FONT-WEIGHT:bold; " ID="ITEM_289" valign="baseline" dir="LTR">Credit</DIV></TD>
																					<TD ID="ITEM_174" align="LEFT" width="100" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; FONT-WEIGHT:bold; " ID="ITEM_291" valign="baseline" dir="LTR">Balance</DIV></TD>
																					<TD ID="ITEM_177" align="LEFT" width="10" height="20" style=" CURSOR:DEFAULT; PADDING-BOTTOM: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; PADDING-TOP: 0px"><SPAN ID="ITEM_179" valign="baseline"><IMG CLASS="COLLAPSE" SRC="../../images/but_colapse_table.gif"><xsl:attribute name="onclick">CollapseTable()</xsl:attribute><xsl:attribute name="style">cursor:hand</xsl:attribute></IMG></SPAN></TD></TR><xsl:for-each select="PersonalDetailes/Statement"><TR OPENCLOSE="OPEN" DATAROW="TRUE" ID="ITEM_160"><xsl:attribute name="onmouseover">myChangeto('#c7e19e')</xsl:attribute><xsl:attribute name="onmouseout">myChangeback()</xsl:attribute>
																						<TD ID="ITEM_181" width="80" height="20" style=" CURSOR:DEFAULT; ">
																							<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_200" valign="baseline" dir=""><xsl:value-of select="Date/text()" disable-output-escaping="yes"/></DIV></TD>
																						<TD ID="ITEM_184" width="110" height="20" style=" CURSOR:DEFAULT; ">
																							<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_202" valign="baseline" dir=""><xsl:value-of select="Description/text()" disable-output-escaping="yes"/></DIV></TD>
																						<TD ID="ITEM_187" width="100" height="20" style=" CURSOR:DEFAULT; ">
																							<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif;  COLOR:#960018; " ID="ITEM_204" valign="baseline" dir="" CID="ITEM_204"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute><xsl:value-of select="Debit/text()" disable-output-escaping="yes"/></DIV></TD>
																						<TD ID="ITEM_190" width="100" height="20" style=" CURSOR:DEFAULT; ">
																							<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif;  COLOR:#00693f; " ID="ITEM_206" valign="baseline" dir="" CID="ITEM_206"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute><xsl:value-of select="Credit/text()" disable-output-escaping="yes"/></DIV></TD>
																						<TD ID="ITEM_193" width="100" height="20" style=" CURSOR:DEFAULT; ">
																							<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_208" valign="baseline" dir="" CID="ITEM_208"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute><xsl:value-of select="Balance/text()" disable-output-escaping="yes"/></DIV></TD>
																						<TD ID="ITEM_196" align="LEFT" width="10" height="20" style=" CURSOR:DEFAULT; PADDING-BOTTOM: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; PADDING-TOP: 0px"><SPAN ID="ITEM_198" valign="baseline"><IMG CLASS="COLLAPSE" SRC="../../images/but_expend_table.gif"><xsl:if expr="ItemHasChildNodes(me,&quot;Code,DetDescription,BankCode,BankBranch,BankAccount,AccountOwner,Document&quot;)"><xsl:attribute name="onclick">ShowHideDataTableLines()</xsl:attribute><xsl:attribute name="style">cursor:hand</xsl:attribute></xsl:if><xsl:if expr="not ItemHasChildNodes(me,&quot;Code,DetDescription,BankCode,BankBranch,BankAccount,AccountOwner,Document&quot;)"><xsl:attribute name="style">display:none</xsl:attribute></xsl:if></IMG></SPAN></TD></TR>
																					<xsl:if expr="ItemHasChildNodes(me,&quot;Code,DetDescription,BankCode,BankBranch,BankAccount,AccountOwner,Document&quot;)"><TR ID="ITEM_213" OPENCLOSE="CLOSE" INLINE="true" DATAROW="TRUE" style="DISPLAY:none">
																							<TD ID="" colspan="6" rowspan="">
																								<DIV align="LEFT">
																									<TABLE ID="" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
																											<TD>
																												<TABLE ID="ITEM_213_SOURCE" DISPLAY_NONE="TRUE" ITYPE="SOURCE_TABLE" width="495" bgcolor="#2fb472" cellspacing="1" cellpadding="2" style=" BACKGROUND-COLOR:#2fb472;  CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_214" INLINE="FALSE">
																															<TD ID="ITEM_215" bgcolor="#89ca9d" align="LEFT" width="35" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_295" valign="baseline" dir="LTR">Code</DIV></TD>
																															<TD ID="ITEM_218" bgcolor="#89ca9d" align="LEFT" width="100" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_297" valign="baseline" dir="LTR">Description</DIV></TD>
																															<TD ID="ITEM_221" bgcolor="#89ca9d" align="LEFT" width="60" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_299" valign="baseline" dir="LTR" CID="ITEM_299"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute>Bank&amp;nbsp;Code</DIV></TD>
																															<TD ID="ITEM_224" bgcolor="#89ca9d" align="LEFT" width="40" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_302" valign="baseline" dir="LTR">Branch</DIV></TD>
																															<TD ID="ITEM_227" bgcolor="#89ca9d" align="LEFT" width="70" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_305" valign="baseline" dir="LTR">Account</DIV></TD>
																															<TD ID="ITEM_230" bgcolor="#89ca9d" align="LEFT" width="120" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_307" valign="baseline" dir="LTR">Account&amp;nbsp;Owner</DIV></TD>
																															<TD ID="ITEM_233" bgcolor="#89ca9d" align="LEFT" width="60" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; ">
																																<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_309" valign="baseline" dir="LTR">Document</DIV></TD>
																															<TD ID="ITEM_236" bgcolor="#89ca9d" align="LEFT" width="10" height="20" style=" BACKGROUND-COLOR:#89ca9d;  CURSOR:DEFAULT; PADDING-BOTTOM: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; PADDING-TOP: 0px"><SPAN ID="ITEM_238" valign="baseline"><IMG SRC="../../images/but_colapse_table_ds.gif"/></SPAN></TD></TR><xsl:for-each select="Detailes"><TR OPENCLOSE="OPEN" DATAROW="TRUE" ID="ITEM_213">
																																<TD ID="ITEM_240" bgcolor="#a8d59d" width="35" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_268" valign="baseline" dir=""><xsl:value-of select="Code/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_243" bgcolor="#a8d59d" width="100" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_270" valign="baseline" dir=""><xsl:value-of select="DetDescription/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_246" bgcolor="#a8d59d" width="60" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_272" valign="baseline" dir="" CID="ITEM_272"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute><xsl:value-of select="BankCode/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_249" bgcolor="#a8d59d" width="40" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_274" valign="baseline" dir=""><xsl:value-of select="BankBranch/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_252" bgcolor="#a8d59d" width="70" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_276" valign="baseline" dir=""><xsl:value-of select="BankAccount/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_255" bgcolor="#a8d59d" align="LEFT" width="120" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_278" valign="baseline" dir=""><xsl:value-of select="AccountOwner/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_258" bgcolor="#a8d59d" width="57" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; ">
																																	<DIV align="LEFT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_280" valign="baseline" dir=""><xsl:value-of select="Document/text()" disable-output-escaping="yes"/></DIV></TD>
																																<TD ID="ITEM_261" bgcolor="#a8d59d" align="LEFT" width="10" height="20" style=" BACKGROUND-COLOR:#a8d59d;  CURSOR:DEFAULT; PADDING-BOTTOM: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; PADDING-TOP: 0px"><SPAN ID="ITEM_263" valign="baseline"><IMG SRC="../../images/but_colapse_table_ds.gif"/></SPAN></TD></TR></xsl:for-each></TBODY></TABLE></TD></TR></TABLE>
																								</DIV></TD></TR>
																					</xsl:if></xsl:for-each><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_513" INLINE="FALSE">
																					<TD ID="ITEM_514" width="98" height="20" style=" CURSOR:DEFAULT; "></TD>
																					<TD ID="ITEM_517" width="98" height="20" style=" CURSOR:DEFAULT; "></TD>
																					<TD ID="ITEM_520" width="98" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif;  COLOR:#960018; " ID="ITEM_636" valign="baseline" dir="" CID="ITEM_636"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute></DIV></TD>
																					<TD ID="ITEM_523" width="98" height="20" style=" CURSOR:DEFAULT; ">
																						<DIV align="RIGHT" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif;  COLOR:#00693f; " ID="ITEM_634" valign="baseline" dir="" CID="ITEM_634"><xsl:attribute name="TARGETID"><xsl:eval>GetItemNumber</xsl:eval></xsl:attribute></DIV></TD>
																					<TD ID="ITEM_526" width="98" height="20" style=" CURSOR:DEFAULT; "></TD>
																					<TD ID="ITEM_529" align="LEFT" width="10" height="20" style=" CURSOR:DEFAULT; PADDING-BOTTOM: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; PADDING-TOP: 0px"></TD></TR></TBODY></TABLE></TD></TR></TABLE>
														</DIV>
														<DIV align="" height="16">&amp;nbsp;</DIV></TD>
													<TD ID="ITEM_155" align="LEFT" valign="BOTTOM" width="50" height="100" style=" CURSOR:DEFAULT; ">
														<DIV align="" height="16">&amp;nbsp;</DIV>
														<DIV align="CENTER">
															<TABLE ID="ITEM_424" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
																	<TD>
																		<TABLE ID="ITEM_434" DISPLAY_NONE="TRUE" width="40" height="60" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_435" INLINE="FALSE">
																					<TD ID="ITEM_436" width="40" height="30" style=" CURSOR:DEFAULT; ">
																						<DIV align="" valign="baseline"><IMG ID="ITEM_431" SRC="DATAMALL_PICTURE:7B342635485B43D6A1CFEC94FB928D70" style=" CURSOR:DEFAULT; "/><IMG style="display:none" ID="ITEM_431_S" SRC="DATAMALL_PICTURE:7B342635485B43D6A1CFEC94FB928D70"/></DIV></TD></TR><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_439" INLINE="FALSE">
																					<TD ID="ITEM_440" width="40" height="30" style=" CURSOR:DEFAULT; ">
																						<DIV align="" valign="baseline"><IMG ID="ITEM_445" SRC="DATAMALL_PICTURE:7B342635485B43D6A1CFEC94FB928D70" style=" CURSOR:DEFAULT; "/><IMG style="display:none" ID="ITEM_445_S" SRC="DATAMALL_PICTURE:7B342635485B43D6A1CFEC94FB928D70"/></DIV></TD></TR></TBODY></TABLE></TD></TR></TABLE>
														</DIV>
														<DIV align="" height="16">&amp;nbsp;</DIV></TD></TR></TBODY></TABLE></TD></TR></TABLE>
						</DIV>
						<DIV align="CENTER" valign="baseline"><IMG ID="ITEM_319" SRC="DATAMALL_PICTURE:3BA27CDC8D384B7D889FBEAEB1E91D07" style=" CURSOR:DEFAULT; "/><IMG style="display:none" ID="ITEM_319_S" SRC="DATAMALL_PICTURE:3BA27CDC8D384B7D889FBEAEB1E91D07"/></DIV></TD></TR><xsl:if expr="isLastNode(me)"><xsl:eval>AddToItemsCounter</xsl:eval><TR CHECK_IF_EMPTY="SEPERATOR" OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_19" INLINE="FALSE">
						<TD ID="ITEM_20" border="#FFFFFF">
							<DIV align="LEFT">
								<TABLE ID="ITEM_21" cellpadding="0" cellspacing="0"><TR openclose="OPEN"><TD></TD>
										<TD>
											<TABLE ID="ITEM_978" DISPLAY_NONE="TRUE" background="DATAMALL_PICTURE:5B10F8B774104D57B2FE1FC3A568F9D8" width="650" height="3" cellspacing="0" cellpadding="0" style=" CURSOR:DEFAULT; "><TBODY><TR OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_979" INLINE="FALSE">
														<TD ID="ITEM_980" width="650" height="3" style=" CURSOR:DEFAULT; ">
															<DIV align="CENTER" style=";  FONT-SIZE:8pt;  FONT-FAMILY:Microsoft Sans Serif; " ID="ITEM_984" valign="baseline" dir="LTR">-------------------------------------------------------------------------------------</DIV></TD></TR></TBODY></TABLE></TD></TR></TABLE>
							</DIV></TD></TR></xsl:if><xsl:if test="context()[end()]"/><xsl:if test="@bLastPage"/></xsl:for-each><xsl:for-each select="PreRender"><TR PRESUBMIT="TRUE" OPENCLOSE="OPEN" DATAROW="FALSE" ID="ITEM_970" INLINE="FALSE"><xsl:eval>RaiseFlagPreSubmit</xsl:eval>
					<TD ID="ITEM_971" border="#FFFFFF"></TD></TR></xsl:for-each><xsl:if expr="NotInPreSubmit()"><SCRIPT LANGUAGE="VBScript" SCRIPTS_GO_HERE="TRUE"><xsl:comment><![CDATA[
function SetAllFunctions_EscapeString(srcString):
  Dim destString:
  destString = replace(srcString, "&nbsp;", " "):
  destString = replace(destString, "&", "&amp;"):
  destString = replace(destString, "<", "&lt;"):
  destString = replace(destString, ">", "&gt;"):
  destString = replace(destString, "'", "&apos;"):
  destString = replace(destString, """", "&quot;"):
  SetAllFunctions_EscapeString = destString:
End Function: 

function SetAllFunctions_StrToXml(srcString):
  Dim destString:
  destString = replace(destString, "&", "&amp;"):
  destString = replace(destString, "<", "&lt;"):
  destString = replace(destString, ">", "&gt;"):
  destString = replace(destString, "'", "&apos;"):
  destString = replace(destString, """", "&quot;"):
  SetAllFunctions_EscapeString = destString:
End Function: 

Function SetAllFunctions_MakeArray(arrItems):
  execute "SetAllFunctions_MakeArray = array(" & arrItems & ")":
End Function: 

Function SetAllFunctions_MakeRefArray(arrRefernces):
  Dim refArray:
  Dim refText:
  Dim fakeItems:
  Dim iItem:
  For Each refText In Split(arrRefernces, "$"):
    If fakeItems <> "" Then fakeItems = fakeItems & ",":
    fakeItems = fakeItems & """X""":
  Next:
  execute "refArray = array(" & fakeItems & ")":
  iItem = 0:
  For Each refText In Split(arrRefernces, "$"):
    refText = Replace(refText, "'", """"):
    execute "set refArray(iItem) = " & refText:
    iItem = iItem + 1:
  Next:
  SetAllFunctions_MakeRefArray = refArray:
End Function:

Function SetAllFunctions_GetReference(parentElement, tagName, sId, lTargetId):
  On Error Resume Next:
  Dim colElements:
  Dim oElement:
  Set SetAllFunctions_GetReference = Nothing:
  Set colElements = parentElement.getElementsByTagName(tagName):
  For Each oElement In colElements:
    If oElement.getAttribute("id") = sId Then
      If lTargetId <> -1 Then
        If CLng(oElement.getAttribute("targetid")) = CLng(lTargetId) Then
          Set SetAllFunctions_GetReference = oElement:
          Exit Function:
        End If:
      Else
        Set SetAllFunctions_GetReference = oElement:
        Exit Function:
      End If:
    End If:
  Next:
End Function: 

function SetAllFunctions(transformedHtml):
  Dim processXml: 
  Dim oProcessItem: 
  set processXml = CreateObject("Microsoft.XMLDOM"): 
  processXml.preserveWhiteSpace = True: 
  If not processXml.loadXml(transformedHtml) then 
    SetAllFunctions = transformedHtml: 
    Exit Function: 
  End If: 

  ' Process function - Total (id: ITEM_634)
  Dim funcObj_0: 
  Dim ObjCnt_1: 
  Dim ObjTest_2: 
  ObjCnt_1 = 0 : 
  For Each funcObj_0 in processXml.selectNodes("//*[@CID='ITEM_634']")
    Dim array_3: 
    Dim var_4: 
    Dim Table_5: 
    Set Table_5 = funcObj_0.parentNode :
    Do While Table_5.selectSingleNode("@ITYPE[text() $ieq$ 'DATATABLE' $or$ text() $ieq$ 'SOURCE_TABLE' $or$ text() $ieq$ 'UNUSED_TABLE'  $or$ text() $ieq$ 'TARGET_TABLE']") is nothing
      Set Table_5 = Table_5.parentNode :
    Loop:
    For Each var_4 in Table_5.selectNodes(".//*[@CID='ITEM_206']")
      array_3 = array_3 & """" & replace(var_4.text,"""","""""") & """,": 
    Next:
    If array_3 <> "" then
      array_3 = left(array_3, len(array_3)-1)
      execute "array_3 = array(" & array_3 & ")"
    End If:
    funcObj_0.text = Total(array_3): 
    array_3 = "": 
    ObjCnt_1 = ObjCnt_1 + 1: 
  Next: 

  ' Process function - Total (id: ITEM_636)
  Dim funcObj_6: 
  Dim ObjCnt_7: 
  Dim ObjTest_8: 
  ObjCnt_7 = 0 : 
  For Each funcObj_6 in processXml.selectNodes("//*[@CID='ITEM_636']")
    Dim array_9: 
    Dim var_10: 
    Dim Table_11: 
    Set Table_11 = funcObj_6.parentNode :
    Do While Table_11.selectSingleNode("@ITYPE[text() $ieq$ 'DATATABLE' $or$ text() $ieq$ 'SOURCE_TABLE' $or$ text() $ieq$ 'UNUSED_TABLE'  $or$ text() $ieq$ 'TARGET_TABLE']") is nothing
      Set Table_11 = Table_11.parentNode :
    Loop:
    For Each var_10 in Table_11.selectNodes(".//*[@CID='ITEM_204']")
      array_9 = array_9 & """" & replace(var_10.text,"""","""""") & """,": 
    Next:
    If array_9 <> "" then
      array_9 = left(array_9, len(array_9)-1)
      execute "array_9 = array(" & array_9 & ")"
    End If:
    funcObj_6.text = Total(array_9): 
    array_9 = "": 
    ObjCnt_7 = ObjCnt_7 + 1: 
  Next: 
  ' Set all dynamic attributes (those who depend on results of statements) - Begin
  Dim condObj_12: 
  Dim bConMatched_13: 
  Dim objUnderLine_14: 
  Dim sKeepText_15: 
  Dim oAttrStyle_16: 
  Dim ObjTest_17: 
  Dim colObjects_18: 
  Dim iCounter_19: 
  Set colObjects_18 = processXml.selectNodes("//*[@CID='ITEM_208']"): 
  iCounter_19 = 0: 
  Dim colObjects_20: 
  Dim iCounter_21: 
  Set colObjects_20 = processXml.selectNodes("//*[@CID='ITEM_208']"): 
  iCounter_21 = 0: 
  For Each condObj_12 in processXml.selectNodes("//*[@CID='ITEM_208']")
    bConMatched_13 = False: 
    Set oAttrStyle_16 = condObj_12.Attributes.getNamedItem("style"): 
    If oAttrStyle_16 is nothing then
      Set oAttrStyle_16 = processXml.createAttribute("style"):
      condObj_12.Attributes.SetNamedItem oAttrStyle_16: 
    End If: 
    Dim var_22: 
    Dim line_23: 
    Dim var_24: 
    Set line_23 = condObj_12.parentNode :
    Do While line_23.selectSingleNode ("@DATAROW[text()='TRUE']") is nothing
      Set line_23 = line_23.parentNode :
    Loop:
    For Each var_24 in line_23.selectNodes(".//*[@CID='ITEM_208']")
      var_22 = var_24.text: 
    Next:
    Dim var_25: 
    Dim line_26: 
    Dim var_27: 
    Set line_26 = condObj_12.parentNode :
    Do While line_26.selectSingleNode ("@DATAROW[text()='TRUE']") is nothing
      Set line_26 = line_26.parentNode :
    Loop:
    var_25 = "0":
    If isNumeric(var_22) and isNumeric(var_25) Then 
      If Not bConMatched_13 and Cdbl(var_22) < Cdbl(var_25) Then 
        condObj_12.Attributes.getNamedItem("style").text = "FONT-SIZE:8pt; COLOR:#ff0000; FONT-STYLE:normal ; FONT-WEIGHT:normal": 
        bConMatched_13 = True: 
      End If: 
    Else: 
      If Not bConMatched_13 and UCase(var_22) < UCase(var_25) Then 
        condObj_12.Attributes.getNamedItem("style").text = "FONT-SIZE:8pt; COLOR:#ff0000; FONT-STYLE:normal ; FONT-WEIGHT:normal": 
        bConMatched_13 = True: 
      End If: 
    End if: 
    var_22 = "": 
    var_25 = "": 
    Dim var_28: 
    Dim line_29: 
    Dim var_30: 
    Set line_29 = condObj_12.parentNode :
    Do While line_29.selectSingleNode ("@DATAROW[text()='TRUE']") is nothing
      Set line_29 = line_29.parentNode :
    Loop:
    For Each var_30 in line_29.selectNodes(".//*[@CID='ITEM_208']")
      var_28 = var_30.text: 
    Next:
    Dim var_31: 
    Dim line_32: 
    Dim var_33: 
    Set line_32 = condObj_12.parentNode :
    Do While line_32.selectSingleNode ("@DATAROW[text()='TRUE']") is nothing
      Set line_32 = line_32.parentNode :
    Loop:
    var_31 = "0":
    If isNumeric(var_28) and isNumeric(var_31) Then 
      If Not bConMatched_13 and Cdbl(var_28) > Cdbl(var_31) Then 
        condObj_12.Attributes.getNamedItem("style").text = "FONT-SIZE:8pt; COLOR:#00693f; FONT-STYLE:normal ; FONT-WEIGHT:normal": 
        bConMatched_13 = True: 
      End If: 
    Else: 
      If Not bConMatched_13 and UCase(var_28) > UCase(var_31) Then 
        condObj_12.Attributes.getNamedItem("style").text = "FONT-SIZE:8pt; COLOR:#00693f; FONT-STYLE:normal ; FONT-WEIGHT:normal": 
        bConMatched_13 = True: 
      End If: 
    End if: 
    var_28 = "": 
    var_31 = "": 
  Next: 
  Dim condObj_34: 
  Dim bConMatched_35: 
  Dim objUnderLine_36: 
  Dim sKeepText_37: 
  Dim oAttrStyle_38: 
  Dim ObjTest_39: 
  Dim colObjects_40: 
  Dim iCounter_41: 
  Set colObjects_40 = processXml.selectNodes("//*[@CID='ITEM_272']"): 
  iCounter_41 = 0: 
  For Each condObj_34 in processXml.selectNodes("//*[@CID='ITEM_299']")
    bConMatched_35 = False: 
    Set oAttrStyle_38 = condObj_34.Attributes.getNamedItem("style"): 
    If oAttrStyle_38 is nothing then
      Set oAttrStyle_38 = processXml.createAttribute("style"):
      condObj_34.Attributes.SetNamedItem oAttrStyle_38: 
    End If: 
    Dim var_42: 
    Dim Table_43: 
    Set Table_43 = condObj_34 :
    Do While Table_43.selectSingleNode("@ITYPE[text() $ieq$ 'DATATABLE' $or$ text() $ieq$ 'SOURCE_TABLE' $or$ text() $ieq$ 'UNUSED_TABLE'  $or$ text() $ieq$ 'TARGET_TABLE']") is nothing
      Set Table_43 = Table_43.parentNode :
    Loop:
    Set ObjTest_39 = processXml.selectSingleNode("//*[@CID='ITEM_272']"): 
    If not ObjTest_39 is nothing then 
      var_42 = SetAllFunctions_EscapeString(ObjTest_39.text): 
    End If:
    Dim var_44: 
    var_44 = "":
    If isNumeric(var_42) and isNumeric(var_44) Then 
      If Not bConMatched_35 and Cdbl(var_42) = Cdbl(var_44) Then 
        condObj_34.Attributes.getNamedItem("style").text = "FONT-SIZE:8pt; COLOR:#000000; display:none; FONT-STYLE:normal ; FONT-WEIGHT:normal": 
        bConMatched_35 = True: 
      End If: 
    Else: 
      If Not bConMatched_35 and UCase(var_42) = UCase(var_44) Then 
        condObj_34.Attributes.getNamedItem("style").text = "FONT-SIZE:8pt; COLOR:#000000; display:none; FONT-STYLE:normal ; FONT-WEIGHT:normal": 
        bConMatched_35 = True: 
      End If: 
    End if: 
    var_42 = "": 
    var_44 = "": 
  Next: 
  ' Set all dynamic attributes - End
  SetAllFunctions = processXml.xml: 
  set processXml = nothing: 
End Function: 


function Total(sValuesArray)
	dim lTotal
	dim lTemp
	dim i
	
	if UBound(sValuesArray) = -1 then
		Total = 0
		exit function
	end if
	
	lTotal = 0
	if IsNumeric(sValuesArray(0)) then
		lTotal = sValuesArray(0)
	end if
	
	for i = 1 to UBound(sValuesArray)
		lTemp = sValuesArray(i)
		if IsNumeric(lTemp) then
			lTotal = lTotal + Clng(lTemp)
		end if
	next
	
	Total = cdbl(lTotal)
end function
]]></xsl:comment></SCRIPT></xsl:if></TABLE><xsl:if expr="NoDataWasFound()"/></xsl:template></xsl:stylesheet>

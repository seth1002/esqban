<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
	<xsl:template match="/">
		<xsl:apply-templates select="ServiceData/MasterData/Expenses"/>
	</xsl:template>
	<xsl:template match="ServiceData/MasterData/Expenses">
		<TechnionData>
			<File001>
				<Account>
					<!--Connection from Source Node "@Account" to Destination Node "Account"-->
					<xsl:value-of select="@Account"/>
				</Account>
				<Year>
					<!--Connection from Source Node "@Year" to Destination Node "Year"-->
					<xsl:value-of select="@Year"/>
				</Year>
				<Tikzuv>
					<xsl:variable name="var:v1" select="user:convertIncV(string(ancestor::*[1]/@BudgetType))"/>
					<xsl:value-of select="$var:v1"/>
				</Tikzuv>
				<CurNis>
					<!--Connection from Source Node "@CurTypeH" to Destination Node "CurNis"-->
					<xsl:value-of select="@CurTypeH"/>
				</CurNis>
				<CurAcc>
					<!--Connection from Source Node "@CurTypeE" to Destination Node "CurAcc"-->
					<xsl:value-of select="@CurTypeE"/>
				</CurAcc>
				<Expences001>
					<Expenses-002>
						<!--Connection from Source Node "Expenses" to Destination Node "Expenses-002"-->
						<xsl:for-each select="TotalExpenses">
							<PirutExpenses_003>
								<!--Connection from Source Node "TotalExpenses" to Destination Node "PirutExpenses_003"-->
								<xsl:value-of select="./text()"/>
								<ExpUpdated>
									<xsl:variable name="var:v2" select="user:convert2(string(@UpdateDate))"/>
									<xsl:value-of select="$var:v2"/>
								</ExpUpdated>
								<ExpValueDate>
									<xsl:variable name="var:v3" select="user:convertDate2(string(@ValueDate))"/>
									<xsl:value-of select="$var:v3"/>
								</ExpValueDate>
								<ExpSumNis003>
									<!--Connection from Source Node "@ExpNis" to Destination Node "ExpSumNis003"-->
									<xsl:value-of select="@ExpNis"/>
								</ExpSumNis003>
								<ExpSumAcc003>
									<!--Connection from Source Node "@ExpAcc" to Destination Node "ExpSumAcc003"-->
									<xsl:value-of select="@ExpAcc"/>
								</ExpSumAcc003>
								<ExpAsmachtaA>
									<!--Connection from Source Node "@AsmahtaA" to Destination Node "ExpAsmachtaA"-->
									<xsl:value-of select="@AsmahtaA"/>
								</ExpAsmachtaA>
								<ExpSugA>
									<!--Connection from Source Node "@AsmahtaTypeA" to Destination Node "ExpSugA"-->
									<xsl:value-of select="@AsmahtaTypeA"/>
								</ExpSugA>
								<ExpAsmachtaB>
									<!--Connection from Source Node "@AsmahtaB" to Destination Node "ExpAsmachtaB"-->
									<xsl:value-of select="@AsmahtaB"/>
								</ExpAsmachtaB>
								<ExpSugB>
									<!--Connection from Source Node "@AsmahtaTypeB" to Destination Node "ExpSugB"-->
									<xsl:value-of select="@AsmahtaTypeB"/>
								</ExpSugB>
								<ExpCrossAcc>
									<!--Connection from Source Node "@AccountCross" to Destination Node "ExpCrossAcc"-->
									<xsl:value-of select="@AccountCross"/>
								</ExpCrossAcc>
								<ExpDetails>
									<!--Connection from Source Node "@Remarks" to Destination Node "ExpDetails"-->
									<xsl:value-of select="@Remarks"/>
								</ExpDetails>
							</PirutExpenses_003>
						</xsl:for-each>
						<xsl:value-of select="./text()"/>
						<ExpGroup>
							<!--Connection from Source Node "@ExpIncGroup" to Destination Node "ExpGroup"-->
							<xsl:value-of select="@ExpIncGroup"/>
						</ExpGroup>
						<ExpGroupDescH>
							<!--Connection from Source Node "@GrpNameH" to Destination Node "ExpGroupDescH"-->
							<xsl:value-of select="@GrpNameH"/>
						</ExpGroupDescH>
						<ExpGroupDescE>
							<!--Connection from Source Node "@GrpNameE" to Destination Node "ExpGroupDescE"-->
							<xsl:value-of select="@GrpNameE"/>
						</ExpGroupDescE>
						<ExpNis002>
							<!--Connection from Source Node "@ExpNis" to Destination Node "ExpNis002"-->
							<xsl:value-of select="@ExpNis"/>
						</ExpNis002>
						<ExpAcc002>
							<!--Connection from Source Node "@ExpAcc" to Destination Node "ExpAcc002"-->
							<xsl:value-of select="@ExpAcc"/>
						</ExpAcc002>
					</Expenses-002>
					<ExpCurNis>
						<!--Connection from Source Node "@ExpNis" to Destination Node "ExpCurNis"-->
						<xsl:value-of select="@ExpNis"/>
					</ExpCurNis>
					<ExpCurAcc>
						<!--Connection from Source Node "@ExpAcc" to Destination Node "ExpCurAcc"-->
						<xsl:value-of select="@ExpAcc"/>
					</ExpCurAcc>
				</Expences001>
				<Incomes001>
					<xsl:for-each select="Incomes">
						<Incomes002>
							<!--Connection from Source Node "Incomes" to Destination Node "Incomes002"-->
							<xsl:for-each select="TotalIncomes">
								<PirutIncomess_003>
									<!--Connection from Source Node "TotalIncomes" to Destination Node "PirutIncomess_003"-->
									<xsl:value-of select="./text()"/>
									<IncDebCrd>
										<xsl:variable name="var:v4" select="user:convertDate(string(ancestor::*[3]/@HovaZhut))"/>
										<xsl:value-of select="$var:v4"/>
									</IncDebCrd>
									<IncUpdated>
										<xsl:variable name="var:v5" select="user:convertInc(string(ancestor::*[3]/@UpdateDate))"/>
										<xsl:value-of select="$var:v5"/>
									</IncUpdated>
									<IncValueDate>
										<xsl:variable name="var:v6" select="user:convertDateUpInc(string(ancestor::*[3]/@ValueDate))"/>
										<xsl:value-of select="$var:v6"/>
									</IncValueDate>
									<IncSumNis003>
										<!--Connection from Source Node "@SumNis" to Destination Node "IncSumNis003"-->
										<xsl:value-of select="@SumNis"/>
									</IncSumNis003>
									<IncSumAcc003>
										<!--Connection from Source Node "@SumAcc" to Destination Node "IncSumAcc003"-->
										<xsl:value-of select="@SumAcc"/>
									</IncSumAcc003>
									<IncAsmachtaA>
										<!--Connection from Source Node "@AsmahtaA" to Destination Node "IncAsmachtaA"-->
										<xsl:value-of select="@AsmahtaA"/>
									</IncAsmachtaA>
									<IncSugA>
										<!--Connection from Source Node "@AsmahtaTypeA" to Destination Node "IncSugA"-->
										<xsl:value-of select="@AsmahtaTypeA"/>
									</IncSugA>
									<IncAsmachtaB>
										<!--Connection from Source Node "@AsmahtaB" to Destination Node "IncAsmachtaB"-->
										<xsl:value-of select="@AsmahtaB"/>
									</IncAsmachtaB>
									<IncSugB>
										<!--Connection from Source Node "@AsmahtaTypeB" to Destination Node "IncSugB"-->
										<xsl:value-of select="@AsmahtaTypeB"/>
									</IncSugB>
									<IncCrossAcc>
										<!--Connection from Source Node "@AccountCross" to Destination Node "IncCrossAcc"-->
										<xsl:value-of select="@AccountCross"/>
									</IncCrossAcc>
									<IncDetails>
										<!--Connection from Source Node "@Remarks" to Destination Node "IncDetails"-->
										<xsl:value-of select="@Remarks"/>
									</IncDetails>
								</PirutIncomess_003>
							</xsl:for-each>
							<xsl:value-of select="./text()"/>
							<IncGroup>
								<!--Connection from Source Node "@ExpIncGroup" to Destination Node "IncGroup"-->
								<xsl:value-of select="@ExpIncGroup"/>
							</IncGroup>
							<IncGroupDescH>
								<!--Connection from Source Node "@GrpNameH" to Destination Node "IncGroupDescH"-->
								<xsl:value-of select="@GrpNameH"/>
							</IncGroupDescH>
							<IncGroupDescE>
								<!--Connection from Source Node "@GrpNameE" to Destination Node "IncGroupDescE"-->
								<xsl:value-of select="@GrpNameE"/>
							</IncGroupDescE>
							<IncNis002>
								<!--Connection from Source Node "@IncNis" to Destination Node "IncNis002"-->
								<xsl:value-of select="@IncNis"/>
							</IncNis002>
							<IncAcc002>
								<!--Connection from Source Node "@IncAcc" to Destination Node "IncAcc002"-->
								<xsl:value-of select="@IncAcc"/>
							</IncAcc002>
						</Incomes002>
					</xsl:for-each>
					<IncCurNis>
						<!--Connection from Source Node "@IncNis" to Destination Node "IncCurNis"-->
						<xsl:value-of select="@IncNis"/>
					</IncCurNis>
					<IncCurAcc>
						<!--Connection from Source Node "@IncAcc" to Destination Node "IncCurAcc"-->
						<xsl:value-of select="@IncAcc"/>
					</IncCurAcc>
				</Incomes001>
				<AccBalance001>
					<AccBalanceNis>
						<!--Connection from Source Node "@HeshbonBalNis" to Destination Node "AccBalanceNis"-->
						<xsl:value-of select="@HeshbonBalNis"/>
					</AccBalanceNis>
					<AccBalanceCurAcc>
						<!--Connection from Source Node "@HeshbonBalAcc" to Destination Node "AccBalanceCurAcc"-->
						<xsl:value-of select="@HeshbonBalAcc"/>
					</AccBalanceCurAcc>
				</AccBalance001>
				<BalanceIthaevut001>
					<Ithaevut002>
						<!--Connection from Source Node "Expenses" to Destination Node "Ithaevut002"-->
						<xsl:for-each select="TotalBal">
							<Ithaev004>
								<!--Connection from Source Node "TotalBal" to Destination Node "Ithaev004"-->
								<xsl:value-of select="./text()"/>
								<IthNo>
									<!--Connection from Source Node "@IthaevutNo" to Destination Node "IthNo"-->
									<xsl:value-of select="@IthaevutNo"/>
								</IthNo>
								<IthCurrH>
									<!--Connection from Source Node "@IthCurH" to Destination Node "IthCurrH"-->
									<xsl:value-of select="@IthCurH"/>
								</IthCurrH>
								<IthCurrE>
									<!--Connection from Source Node "@IthCurE" to Destination Node "IthCurrE"-->
									<xsl:value-of select="@IthCurE"/>
								</IthCurrE>
								<IthBalanceNis>
									<!--Connection from Source Node "@BalNis" to Destination Node "IthBalanceNis"-->
									<xsl:value-of select="@BalNis"/>
								</IthBalanceNis>
								<IthBalanceCurrAcc>
									<!--Connection from Source Node "@BalAcc" to Destination Node "IthBalanceCurrAcc"-->
									<xsl:value-of select="@BalAcc"/>
								</IthBalanceCurrAcc>
								<IthBalanceCurrInv>
									<!--Connection from Source Node "@BalIth" to Destination Node "IthBalanceCurrInv"-->
									<xsl:value-of select="@BalIth"/>
								</IthBalanceCurrInv>
								<IthDescH>
									<!--Connection from Source Node "@IthDescH" to Destination Node "IthDescH"-->
									<xsl:value-of select="@IthDescH"/>
								</IthDescH>
							</Ithaev004>
						</xsl:for-each>
						<xsl:value-of select="./text()"/>
						<IthaevGroup>
							<!--Connection from Source Node "@ExpIncGroup" to Destination Node "IthaevGroup"-->
							<xsl:value-of select="@ExpIncGroup"/>
						</IthaevGroup>
						<IthGroupDescH>
							<!--Connection from Source Node "@GrpNameH" to Destination Node "IthGroupDescH"-->
							<xsl:value-of select="@GrpNameH"/>
						</IthGroupDescH>
						<IthGroupDescE>
							<!--Connection from Source Node "@GrpNameE" to Destination Node "IthGroupDescE"-->
							<xsl:value-of select="@GrpNameE"/>
						</IthGroupDescE>
						<IthNis002>
							<!--Connection from Source Node "@BalNis" to Destination Node "IthNis002"-->
							<xsl:value-of select="@BalNis"/>
						</IthNis002>
						<IthCurrAcc002>
							<!--Connection from Source Node "@BalAcc" to Destination Node "IthCurrAcc002"-->
							<xsl:value-of select="@BalAcc"/>
						</IthCurrAcc002>
					</Ithaevut002>
					<IthCurNis>
						<!--Connection from Source Node "@BalNis" to Destination Node "IthCurNis"-->
						<xsl:value-of select="@BalNis"/>
					</IthCurNis>
					<IthCurAcc>
						<!--Connection from Source Node "@BalAcc" to Destination Node "IthCurAcc"-->
						<xsl:value-of select="@BalAcc"/>
					</IthCurAcc>
				</BalanceIthaevut001>
				<Budget001>
					<TotalBgtNis001>
						<!--Connection from Source Node "@TotalBudgetNis" to Destination Node "TotalBgtNis001"-->
						<xsl:value-of select="@TotalBudgetNis"/>
					</TotalBgtNis001>
					<TotalBgtCurrAcc001>
						<!--Connection from Source Node "@TotalBudgetAcc" to Destination Node "TotalBgtCurrAcc001"-->
						<xsl:value-of select="@TotalBudgetAcc"/>
					</TotalBgtCurrAcc001>
					<ItraBgtNis001>
						<!--Connection from Source Node "@BalBudgetNis" to Destination Node "ItraBgtNis001"-->
						<xsl:value-of select="@BalBudgetNis"/>
					</ItraBgtNis001>
					<ItraBgtAcc001>
						<!--Connection from Source Node "@BalBudgetAcc" to Destination Node "ItraBgtAcc001"-->
						<xsl:value-of select="@BalBudgetAcc"/>
					</ItraBgtAcc001>
					<xsl:for-each select="TotalBudget">
						<DetailesBgt002>
							<!--Connection from Source Node "TotalBudget" to Destination Node "DetailesBgt002"-->
							<xsl:value-of select="./text()"/>
							<GroupBgt>
								<!--Connection from Source Node "@ExpIncGroup" to Destination Node "GroupBgt"-->
								<xsl:value-of select="@ExpIncGroup"/>
							</GroupBgt>
							<GroupBgtDescH>
								<!--Connection from Source Node "@GrpNameH" to Destination Node "GroupBgtDescH"-->
								<xsl:value-of select="@GrpNameH"/>
							</GroupBgtDescH>
							<GroupBgtDescE>
								<!--Connection from Source Node "@GrpNameE" to Destination Node "GroupBgtDescE"-->
								<xsl:value-of select="@GrpNameE"/>
							</GroupBgtDescE>
							<TotalGroupBgtCurrNis>
								<!--Connection from Source Node "@TotalBudgetNis" to Destination Node "TotalGroupBgtCurrNis"-->
								<xsl:value-of select="@TotalBudgetNis"/>
							</TotalGroupBgtCurrNis>
							<TotalGroupBgtCirrAcc>
								<!--Connection from Source Node "@TotalBudgetAcc" to Destination Node "TotalGroupBgtCirrAcc"-->
								<xsl:value-of select="@TotalBudgetAcc"/>
							</TotalGroupBgtCirrAcc>
							<ItraGroupBgtNis>
								<!--Connection from Source Node "@BalBudgetNis" to Destination Node "ItraGroupBgtNis"-->
								<xsl:value-of select="@BalBudgetNis"/>
							</ItraGroupBgtNis>
							<ItraGroupBgtAcc>
								<!--Connection from Source Node "@BalBudgetAcc" to Destination Node "ItraGroupBgtAcc"-->
								<xsl:value-of select="@BalBudgetAcc"/>
							</ItraGroupBgtAcc>
						</DetailesBgt002>
					</xsl:for-each>
				</Budget001>
				<SponsorInvoicesTakbulim>
					<SpnInvNis>
						<!--Connection from Source Node "@HeshbonitNis" to Destination Node "SpnInvNis"-->
						<xsl:value-of select="@HeshbonitNis"/>
					</SpnInvNis>
					<SpnInvAcc>
						<!--Connection from Source Node "@HeshbonitAcc" to Destination Node "SpnInvAcc"-->
						<xsl:value-of select="@HeshbonitAcc"/>
					</SpnInvAcc>
					<SpnTakbulimNis>
						<!--Connection from Source Node "@TakbulNis" to Destination Node "SpnTakbulimNis"-->
						<xsl:value-of select="@TakbulNis"/>
					</SpnTakbulimNis>
					<SpnTakbulimAcc>
						<!--Connection from Source Node "@TakbulAcc" to Destination Node "SpnTakbulimAcc"-->
						<xsl:value-of select="@TakbulAcc"/>
					</SpnTakbulimAcc>
					<xsl:for-each select="HeshbTakbul">
						<SpnDetailes005>
							<!--Connection from Source Node "HeshbTakbul" to Destination Node "SpnDetailes005"-->
							<xsl:value-of select="./text()"/>
							<InvoiceNo>
								<!--Connection from Source Node "@HeshbonitNo" to Destination Node "InvoiceNo"-->
								<xsl:value-of select="@HeshbonitNo"/>
							</InvoiceNo>
							<InvSumNis>
								<!--Connection from Source Node "@HeshbonitNis" to Destination Node "InvSumNis"-->
								<xsl:value-of select="@HeshbonitNis"/>
							</InvSumNis>
							<InvSumAcc>
								<!--Connection from Source Node "@HeshbonitAcc" to Destination Node "InvSumAcc"-->
								<xsl:value-of select="@HeshbonitAcc"/>
							</InvSumAcc>
							<TkSumNis>
								<!--Connection from Source Node "@TakbulNis" to Destination Node "TkSumNis"-->
								<xsl:value-of select="@TakbulNis"/>
							</TkSumNis>
							<TkSumAcc>
								<!--Connection from Source Node "@TakbulAcc" to Destination Node "TkSumAcc"-->
								<xsl:value-of select="@TakbulAcc"/>
							</TkSumAcc>
							<InvItraNis>
								<!--Connection from Source Node "@HeshbonitBalNis" to Destination Node "InvItraNis"-->
								<xsl:value-of select="@HeshbonitBalNis"/>
							</InvItraNis>
							<InvItraAcc>
								<!--Connection from Source Node "@HeshbonitBalAcc" to Destination Node "InvItraAcc"-->
								<xsl:value-of select="@HeshbonitBalAcc"/>
							</InvItraAcc>
						</SpnDetailes005>
					</xsl:for-each>
				</SponsorInvoicesTakbulim>
			</File001>
			<File006>
				<xsl:for-each select="File006">
					<ReserchCoordCod>
						<!--Connection from Source Node "@MetaemCode" to Destination Node "ReserchCoordCod"-->
						<xsl:value-of select="@MetaemCode"/>
					</ReserchCoordCod>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ReserchCoordDescH>
						<!--Connection from Source Node "@MetaemDescH" to Destination Node "ReserchCoordDescH"-->
						<xsl:value-of select="@MetaemDescH"/>
					</ReserchCoordDescH>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ReserchCoordDescE>
						<!--Connection from Source Node "@MetaemDescE" to Destination Node "ReserchCoordDescE"-->
						<xsl:value-of select="@MetaemDescE"/>
					</ReserchCoordDescE>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<Phone>
						<!--Connection from Source Node "@Phone" to Destination Node "Phone"-->
						<xsl:value-of select="@Phone"/>
					</Phone>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<Email>
						<!--Connection from Source Node "@EMail" to Destination Node "Email"-->
						<xsl:value-of select="@EMail"/>
					</Email>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ResSubjectE>
						<!--Connection from Source Node "@ResearchSubjectE" to Destination Node "ResSubjectE"-->
						<xsl:value-of select="@ResearchSubjectE"/>
					</ResSubjectE>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ResSubjectH>
						<!--Connection from Source Node "@ResearchSubjectH" to Destination Node "ResSubjectH"-->
						<xsl:value-of select="@ResearchSubjectH"/>
					</ResSubjectH>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvLast>
						<!--Connection from Source Node "@InvLast" to Destination Node "InvLast"-->
						<xsl:value-of select="@InvLast"/>
					</InvLast>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvLastNameH>
						<!--Connection from Source Node "@InvLastNameH" to Destination Node "InvLastNameH"-->
						<xsl:value-of select="@InvLastNameH"/>
					</InvLastNameH>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvLastNameE>
						<!--Connection from Source Node "@InvLastNameE" to Destination Node "InvLastNameE"-->
						<xsl:value-of select="@InvLastNameE"/>
					</InvLastNameE>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvSumAcc>
						<!--Connection from Source Node "@InvSumAcc" to Destination Node "InvSumAcc"-->
						<xsl:value-of select="@InvSumAcc"/>
					</InvSumAcc>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvSumNis>
						<!--Connection from Source Node "@InvSumNis" to Destination Node "InvSumNis"-->
						<xsl:value-of select="@InvSumNis"/>
					</InvSumNis>
				</xsl:for-each>
			</File006>
		</TechnionData>
	</xsl:template>
	<msxsl:script language="VBScript" implements-prefix="user">
		<![CDATA[function convert2(param1)
     if param1 <> "" and isnumeric(param1) then
     select case clng(param1)
         case 1
              convert2="?"
         case 2
              convert2="?"
     end select
   end if
end function

function convertDate2(param1)
    if len(param1) > 7 then
      convertDate2=left(param1, 2) & "/" & mid(param1, 3, 2) & "/" & right(param1, 4)
 else
    convertDate2=(param1)
    end if
end function

function convertDate(param1)
  if len(param1) > 7 then
 convertDate=left(param1, 2) & "/" & mid(param1, 3, 2) & "/" & right(param1, 4)
    else
 convertDate=(param1)
end if

end function

function convertInc(param1)
    if param1 <> "" and isnumeric(param1) then
       select case clng(param1)
          case 1
              convertInc="?"
         case 2
              convertInc="?"
       end select
    end if
end function

function convertDateUpInc(param1)
    if len(param1) > 7 then
      convertDateUpInc=left(param1, 2) & "/" & mid(param1, 3, 2) & "/" & right(param1, 4)
    else
       convertDateUpInc=(param1)
   end if
end function

function convertIncV(param1)
     if len(param1) > 7 then
         convertIncV=left(param1, 2) & "/" & mid(param1, 3, 2) & "/" & right(param1, 4)
     else
         convertIncV=(param1)
     end if
end function

function converttype(param1)
  if param1 <> "" and isnumeric(param1) then
     select case clng(param1)
         case 1
              converttype="?????? ?? ???????"
         case 0
              converttype="?????? ???????"
     end select
  end if
end function
]]>
	</msxsl:script>
</xsl:stylesheet>

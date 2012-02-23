<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
	<xsl:template match="/">
		<xsl:apply-templates select="ServiceData/MasterData"/>
	</xsl:template>
	<xsl:template match="ServiceData/MasterData">
		<TechnionData>
			<!--Connection from Source Node "MasterData" to Destination Node "TechnionData"-->
			<xsl:value-of select="./text()"/>
			<File001>
				<Account>
					<!--Connection from Source Node "MasterData@Account" to Destination Node "Account"-->
					<xsl:value-of select="MasterData@Account"/>
				</Account>
				<Year>
					<!--Connection from Source Node "MasterData@Year" to Destination Node "Year"-->
					<xsl:value-of select="MasterData@Year"/>
				</Year>
				<Tikzuv>
					<!--Connection from Source Node "MasterData@BudgetType" to Destination Node "Tikzuv"-->
					<xsl:value-of select="MasterData@BudgetType"/>
				</Tikzuv>
				<CurNis>
					<!--Connection from Source Node "MasterData@CurTypeH" to Destination Node "CurNis"-->
					<xsl:value-of select="MasterData@CurTypeH"/>
				</CurNis>
				<CurAcc>
					<!--Connection from Source Node "MasterData@CurTypeE" to Destination Node "CurAcc"-->
					<xsl:value-of select="MasterData@CurTypeE"/>
				</CurAcc>
				<Expences001>
					<xsl:for-each select="Expenses">
						<Expenses-002>
							<!--Connection from Source Node "Expenses" to Destination Node "Expenses-002"-->
							<xsl:value-of select="./text()"/>
							<ExpGroup>
								<!--Connection from Source Node "Expenses@ExpIncGroup" to Destination Node "ExpGroup"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@ExpIncGroup"/>
							</ExpGroup>
							<ExpGroupDescH>
								<!--Connection from Source Node "Expenses@GrpNameH" to Destination Node "ExpGroupDescH"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@GrpNameH"/>
							</ExpGroupDescH>
							<ExpGroupDescE>
								<!--Connection from Source Node "Expenses@GrpNameE" to Destination Node "ExpGroupDescE"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@GrpNameE"/>
							</ExpGroupDescE>
							<ExpNis002>
								<!--Connection from Source Node "Expenses@ExpNis" to Destination Node "ExpNis002"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@ExpNis"/>
							</ExpNis002>
							<ExpAcc002>
								<!--Connection from Source Node "Expenses@ExpAcc" to Destination Node "ExpAcc002"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@ExpAcc"/>
							</ExpAcc002>
							<xsl:for-each select="TotalExpenses">
								<PirutExpenses_003>
									<!--Connection from Source Node "TotalExpenses" to Destination Node "PirutExpenses_003"-->
									<xsl:value-of select="./text()"/>
									<ExpDebCrd>
										<!--Connection from Source Node "TotalExpenses@HovaZhut" to Destination Node "ExpDebCrd"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@HovaZhut"/>
									</ExpDebCrd>
									<ExpUpdated>
										<!--Connection from Source Node "TotalExpenses@UpdateDate" to Destination Node "ExpUpdated"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@UpdateDate"/>
									</ExpUpdated>
									<ExpValueDate>
										<!--Connection from Source Node "TotalExpenses@ValueDate" to Destination Node "ExpValueDate"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@ValueDate"/>
									</ExpValueDate>
									<ExpSumNis003>
										<!--Connection from Source Node "TotalExpenses@ExpNis" to Destination Node "ExpSumNis003"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@ExpNis"/>
									</ExpSumNis003>
									<ExpSumAcc003>
										<!--Connection from Source Node "TotalExpenses@ExpAcc" to Destination Node "ExpSumAcc003"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@ExpAcc"/>
									</ExpSumAcc003>
									<ExpAsmachtaA>
										<!--Connection from Source Node "TotalExpenses@AsmahtaA" to Destination Node "ExpAsmachtaA"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@AsmahtaA"/>
									</ExpAsmachtaA>
									<ExpSugA>
										<!--Connection from Source Node "TotalExpenses@AsmahtaTypeA" to Destination Node "ExpSugA"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@AsmahtaTypeA"/>
									</ExpSugA>
									<ExpAsmachtaB>
										<!--Connection from Source Node "TotalExpenses@AsmahtaB" to Destination Node "ExpAsmachtaB"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@AsmahtaB"/>
									</ExpAsmachtaB>
									<ExpSugB>
										<!--Connection from Source Node "TotalExpenses@AsmahtaTypeB" to Destination Node "ExpSugB"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@AsmahtaTypeB"/>
									</ExpSugB>
									<ExpCrossAcc>
										<!--Connection from Source Node "TotalExpenses@AccountCross" to Destination Node "ExpCrossAcc"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@AccountCross"/>
									</ExpCrossAcc>
									<ExpDetails>
										<!--Connection from Source Node "TotalExpenses@Remarks" to Destination Node "ExpDetails"-->
										<xsl:value-of select="ancestor::*[1]/TotalExpenses@Remarks"/>
									</ExpDetails>
								</PirutExpenses_003>
							</xsl:for-each>
						</Expenses-002>
					</xsl:for-each>
					<ExpCurNis>
						<!--Connection from Source Node "MasterData@ExpNis" to Destination Node "ExpCurNis"-->
						<xsl:value-of select="MasterData@ExpNis"/>
					</ExpCurNis>
					<ExpCurAcc>
						<!--Connection from Source Node "MasterData@ExpAcc" to Destination Node "ExpCurAcc"-->
						<xsl:value-of select="MasterData@ExpAcc"/>
					</ExpCurAcc>
				</Expences001>
				<Incomes001>
					<xsl:for-each select="Incomes">
						<Incomes002>
							<!--Connection from Source Node "Incomes" to Destination Node "Incomes002"-->
							<xsl:value-of select="./text()"/>
							<IncGroup>
								<!--Connection from Source Node "Incomes@ExpIncGroup" to Destination Node "IncGroup"-->
								<xsl:value-of select="ancestor::*[1]/Incomes@ExpIncGroup"/>
							</IncGroup>
							<IncGroupDescH>
								<!--Connection from Source Node "Incomes@GrpNameH" to Destination Node "IncGroupDescH"-->
								<xsl:value-of select="ancestor::*[1]/Incomes@GrpNameH"/>
							</IncGroupDescH>
							<IncGroupDescE>
								<!--Connection from Source Node "Incomes@GrpNameE" to Destination Node "IncGroupDescE"-->
								<xsl:value-of select="ancestor::*[1]/Incomes@GrpNameE"/>
							</IncGroupDescE>
							<IncNis002>
								<!--Connection from Source Node "Incomes@IncNis" to Destination Node "IncNis002"-->
								<xsl:value-of select="ancestor::*[1]/Incomes@IncNis"/>
							</IncNis002>
							<IncAcc002>
								<!--Connection from Source Node "Incomes@IncAcc" to Destination Node "IncAcc002"-->
								<xsl:value-of select="ancestor::*[1]/Incomes@IncAcc"/>
							</IncAcc002>
							<xsl:for-each select="TotalIncomes">
								<PirutIncomess_003>
									<!--Connection from Source Node "TotalIncomes" to Destination Node "PirutIncomess_003"-->
									<xsl:value-of select="./text()"/>
									<IncDebCrd>
										<!--Connection from Source Node "TotalIncomes@HovaZhut" to Destination Node "IncDebCrd"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@HovaZhut"/>
									</IncDebCrd>
									<IncUpdated>
										<!--Connection from Source Node "TotalIncomes@UpdateDate" to Destination Node "IncUpdated"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@UpdateDate"/>
									</IncUpdated>
									<IncValueDate>
										<!--Connection from Source Node "TotalIncomes@ValueDate" to Destination Node "IncValueDate"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@ValueDate"/>
									</IncValueDate>
									<IncSumNis003>
										<!--Connection from Source Node "TotalIncomes@SumNis" to Destination Node "IncSumNis003"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@SumNis"/>
									</IncSumNis003>
									<IncSumAcc003>
										<!--Connection from Source Node "TotalIncomes@SumAcc" to Destination Node "IncSumAcc003"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@SumAcc"/>
									</IncSumAcc003>
									<IncAsmachtaA>
										<!--Connection from Source Node "TotalIncomes@AsmahtaA" to Destination Node "IncAsmachtaA"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@AsmahtaA"/>
									</IncAsmachtaA>
									<IncSugA>
										<!--Connection from Source Node "TotalIncomes@AsmahtaTypeA" to Destination Node "IncSugA"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@AsmahtaTypeA"/>
									</IncSugA>
									<IncAsmachtaB>
										<!--Connection from Source Node "TotalIncomes@AsmahtaB" to Destination Node "IncAsmachtaB"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@AsmahtaB"/>
									</IncAsmachtaB>
									<IncSugB>
										<!--Connection from Source Node "TotalIncomes@AsmahtaTypeB" to Destination Node "IncSugB"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@AsmahtaTypeB"/>
									</IncSugB>
									<IncCrossAcc>
										<!--Connection from Source Node "TotalIncomes@AccountCross" to Destination Node "IncCrossAcc"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@AccountCross"/>
									</IncCrossAcc>
									<IncDetails>
										<!--Connection from Source Node "TotalIncomes@Remarks" to Destination Node "IncDetails"-->
										<xsl:value-of select="ancestor::*[1]/TotalIncomes@Remarks"/>
									</IncDetails>
								</PirutIncomess_003>
							</xsl:for-each>
						</Incomes002>
					</xsl:for-each>
					<IncCurNis>
						<!--Connection from Source Node "MasterData@IncNis" to Destination Node "IncCurNis"-->
						<xsl:value-of select="MasterData@IncNis"/>
					</IncCurNis>
					<IncCurAcc>
						<!--Connection from Source Node "MasterData@IncAcc" to Destination Node "IncCurAcc"-->
						<xsl:value-of select="MasterData@IncAcc"/>
					</IncCurAcc>
				</Incomes001>
				<AccBalance001>
					<AccBalanceNis>
						<!--Connection from Source Node "MasterData@HeshbonBalNis" to Destination Node "AccBalanceNis"-->
						<xsl:value-of select="MasterData@HeshbonBalNis"/>
					</AccBalanceNis>
					<AccBalanceCurAcc>
						<!--Connection from Source Node "MasterData@HeshbonBalAcc" to Destination Node "AccBalanceCurAcc"-->
						<xsl:value-of select="MasterData@HeshbonBalAcc"/>
					</AccBalanceCurAcc>
				</AccBalance001>
				<BalanceIthaevut001>
					<xsl:for-each select="Expenses">
						<Ithaevut002>
							<!--Connection from Source Node "Expenses" to Destination Node "Ithaevut002"-->
							<xsl:value-of select="./text()"/>
							<xsl:for-each select="TotalBal">
								<Ithaev004>
									<!--Connection from Source Node "TotalBal" to Destination Node "Ithaev004"-->
									<xsl:value-of select="./text()"/>
									<IthNo>
										<!--Connection from Source Node "TotalBal@IthaevutNo" to Destination Node "IthNo"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@IthaevutNo"/>
									</IthNo>
									<IthCurrH>
										<!--Connection from Source Node "TotalBal@IthCurH" to Destination Node "IthCurrH"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@IthCurH"/>
									</IthCurrH>
									<IthCurrE>
										<!--Connection from Source Node "TotalBal@IthCurE" to Destination Node "IthCurrE"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@IthCurE"/>
									</IthCurrE>
									<IthBalanceNis>
										<!--Connection from Source Node "TotalBal@BalNis" to Destination Node "IthBalanceNis"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@BalNis"/>
									</IthBalanceNis>
									<IthBalanceCurrAcc>
										<!--Connection from Source Node "TotalBal@BalAcc" to Destination Node "IthBalanceCurrAcc"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@BalAcc"/>
									</IthBalanceCurrAcc>
									<IthBalanceCurrInv>
										<!--Connection from Source Node "TotalBal@BalIth" to Destination Node "IthBalanceCurrInv"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@BalIth"/>
									</IthBalanceCurrInv>
									<IthDescH>
										<!--Connection from Source Node "TotalBal@IthDescH" to Destination Node "IthDescH"-->
										<xsl:value-of select="ancestor::*[1]/TotalBal@IthDescH"/>
									</IthDescH>
								</Ithaev004>
							</xsl:for-each>
							<IthaevGroup>
								<!--Connection from Source Node "Expenses@ExpIncGroup" to Destination Node "IthaevGroup"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@ExpIncGroup"/>
							</IthaevGroup>
							<IthGroupDescH>
								<!--Connection from Source Node "Expenses@GrpNameH" to Destination Node "IthGroupDescH"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@GrpNameH"/>
							</IthGroupDescH>
							<IthGroupDescE>
								<!--Connection from Source Node "Expenses@GrpNameE" to Destination Node "IthGroupDescE"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@GrpNameE"/>
							</IthGroupDescE>
							<IthNis002>
								<!--Connection from Source Node "Expenses@BalNis" to Destination Node "IthNis002"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@BalNis"/>
							</IthNis002>
							<IthCurrAcc002>
								<!--Connection from Source Node "Expenses@BalAcc" to Destination Node "IthCurrAcc002"-->
								<xsl:value-of select="ancestor::*[1]/Expenses@BalAcc"/>
							</IthCurrAcc002>
						</Ithaevut002>
					</xsl:for-each>
					<IthCurNis>
						<!--Connection from Source Node "MasterData@BalNis" to Destination Node "IthCurNis"-->
						<xsl:value-of select="MasterData@BalNis"/>
					</IthCurNis>
					<IthCurAcc>
						<!--Connection from Source Node "MasterData@BalAcc" to Destination Node "IthCurAcc"-->
						<xsl:value-of select="MasterData@BalAcc"/>
					</IthCurAcc>
				</BalanceIthaevut001>
				<Budget001>
					<TotalBgtNis001>
						<!--Connection from Source Node "MasterData@TotalBudgetNis" to Destination Node "TotalBgtNis001"-->
						<xsl:value-of select="MasterData@TotalBudgetNis"/>
					</TotalBgtNis001>
					<TotalBgtCurrAcc001>
						<!--Connection from Source Node "MasterData@TotalBudgetAcc" to Destination Node "TotalBgtCurrAcc001"-->
						<xsl:value-of select="MasterData@TotalBudgetAcc"/>
					</TotalBgtCurrAcc001>
					<ItraBgtNis001>
						<!--Connection from Source Node "MasterData@BalBudgetNis" to Destination Node "ItraBgtNis001"-->
						<xsl:value-of select="MasterData@BalBudgetNis"/>
					</ItraBgtNis001>
					<ItraBgtAcc001>
						<!--Connection from Source Node "MasterData@BalBudgetAcc" to Destination Node "ItraBgtAcc001"-->
						<xsl:value-of select="MasterData@BalBudgetAcc"/>
					</ItraBgtAcc001>
					<xsl:for-each select="TotalBudget">
						<DetailesBgt002>
							<!--Connection from Source Node "TotalBudget" to Destination Node "DetailesBgt002"-->
							<xsl:value-of select="./text()"/>
							<GroupBgt>
								<!--Connection from Source Node "TotalBudget@ExpIncGroup" to Destination Node "GroupBgt"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@ExpIncGroup"/>
							</GroupBgt>
							<GroupBgtDescH>
								<!--Connection from Source Node "TotalBudget@GrpNameH" to Destination Node "GroupBgtDescH"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@GrpNameH"/>
							</GroupBgtDescH>
							<GroupBgtDescE>
								<!--Connection from Source Node "TotalBudget@GrpNameE" to Destination Node "GroupBgtDescE"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@GrpNameE"/>
							</GroupBgtDescE>
							<TotalGroupBgtCurrNis>
								<!--Connection from Source Node "TotalBudget@TotalBudgetNis" to Destination Node "TotalGroupBgtCurrNis"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@TotalBudgetNis"/>
							</TotalGroupBgtCurrNis>
							<TotalGroupBgtCirrAcc>
								<!--Connection from Source Node "TotalBudget@TotalBudgetAcc" to Destination Node "TotalGroupBgtCirrAcc"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@TotalBudgetAcc"/>
							</TotalGroupBgtCirrAcc>
							<ItraGroupBgtNis>
								<!--Connection from Source Node "TotalBudget@BalBudgetNis" to Destination Node "ItraGroupBgtNis"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@BalBudgetNis"/>
							</ItraGroupBgtNis>
							<ItraGroupBgtAcc>
								<!--Connection from Source Node "TotalBudget@BalBudgetAcc" to Destination Node "ItraGroupBgtAcc"-->
								<xsl:value-of select="ancestor::*[1]/TotalBudget@BalBudgetAcc"/>
							</ItraGroupBgtAcc>
						</DetailesBgt002>
					</xsl:for-each>
				</Budget001>
				<SponsorInvoicesTakbulim>
					<SpnInvNis>
						<!--Connection from Source Node "MasterData@HeshbonitNis" to Destination Node "SpnInvNis"-->
						<xsl:value-of select="MasterData@HeshbonitNis"/>
					</SpnInvNis>
					<SpnInvAcc>
						<!--Connection from Source Node "MasterData@HeshbonitAcc" to Destination Node "SpnInvAcc"-->
						<xsl:value-of select="MasterData@HeshbonitAcc"/>
					</SpnInvAcc>
					<SpnTakbulimNis>
						<!--Connection from Source Node "MasterData@TakbulNis" to Destination Node "SpnTakbulimNis"-->
						<xsl:value-of select="MasterData@TakbulNis"/>
					</SpnTakbulimNis>
					<SpnTakbulimAcc>
						<!--Connection from Source Node "MasterData@TakbulAcc" to Destination Node "SpnTakbulimAcc"-->
						<xsl:value-of select="MasterData@TakbulAcc"/>
					</SpnTakbulimAcc>
					<xsl:for-each select="HeshbTakbul">
						<SpnDetailes005>
							<!--Connection from Source Node "HeshbTakbul" to Destination Node "SpnDetailes005"-->
							<xsl:value-of select="./text()"/>
							<InvoiceNo>
								<!--Connection from Source Node "HeshbTakbul@HeshbonitNo" to Destination Node "InvoiceNo"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@HeshbonitNo"/>
							</InvoiceNo>
							<InvSumNis>
								<!--Connection from Source Node "HeshbTakbul@HeshbonitNis" to Destination Node "InvSumNis"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@HeshbonitNis"/>
							</InvSumNis>
							<InvSumAcc>
								<!--Connection from Source Node "HeshbTakbul@HeshbonitAcc" to Destination Node "InvSumAcc"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@HeshbonitAcc"/>
							</InvSumAcc>
							<TkSumNis>
								<!--Connection from Source Node "HeshbTakbul@TakbulNis" to Destination Node "TkSumNis"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@TakbulNis"/>
							</TkSumNis>
							<TkSumAcc>
								<!--Connection from Source Node "HeshbTakbul@TakbulAcc" to Destination Node "TkSumAcc"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@TakbulAcc"/>
							</TkSumAcc>
							<InvItraNis>
								<!--Connection from Source Node "HeshbTakbul@HeshbonitBalNis" to Destination Node "InvItraNis"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@HeshbonitBalNis"/>
							</InvItraNis>
							<InvItraAcc>
								<!--Connection from Source Node "HeshbTakbul@HeshbonitBalAcc" to Destination Node "InvItraAcc"-->
								<xsl:value-of select="ancestor::*[1]/HeshbTakbul@HeshbonitBalAcc"/>
							</InvItraAcc>
						</SpnDetailes005>
					</xsl:for-each>
				</SponsorInvoicesTakbulim>
			</File001>
			<File006>
				<xsl:for-each select="File006">
					<ReserchCoordCod>
						<!--Connection from Source Node "File006@MetaemCode" to Destination Node "ReserchCoordCod"-->
						<xsl:value-of select="File006@MetaemCode"/>
					</ReserchCoordCod>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ReserchCoordDescH>
						<!--Connection from Source Node "File006@MetaemDescH" to Destination Node "ReserchCoordDescH"-->
						<xsl:value-of select="File006@MetaemDescH"/>
					</ReserchCoordDescH>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ReserchCoordDescE>
						<!--Connection from Source Node "File006@MetaemDescE" to Destination Node "ReserchCoordDescE"-->
						<xsl:value-of select="File006@MetaemDescE"/>
					</ReserchCoordDescE>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<Phone>
						<!--Connection from Source Node "File006@Phone" to Destination Node "Phone"-->
						<xsl:value-of select="File006@Phone"/>
					</Phone>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<Email>
						<!--Connection from Source Node "File006@EMail" to Destination Node "Email"-->
						<xsl:value-of select="File006@EMail"/>
					</Email>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ResSubjectE>
						<!--Connection from Source Node "File006@ResearchSubjectE" to Destination Node "ResSubjectE"-->
						<xsl:value-of select="File006@ResearchSubjectE"/>
					</ResSubjectE>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<ResSubjectH>
						<!--Connection from Source Node "File006@ResearchSubjectH" to Destination Node "ResSubjectH"-->
						<xsl:value-of select="File006@ResearchSubjectH"/>
					</ResSubjectH>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvLast>
						<!--Connection from Source Node "File006@InvLast" to Destination Node "InvLast"-->
						<xsl:value-of select="File006@InvLast"/>
					</InvLast>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvLastNameH>
						<!--Connection from Source Node "File006@InvLastNameH" to Destination Node "InvLastNameH"-->
						<xsl:value-of select="File006@InvLastNameH"/>
					</InvLastNameH>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvLastNameE>
						<!--Connection from Source Node "File006@InvLastNameE" to Destination Node "InvLastNameE"-->
						<xsl:value-of select="File006@InvLastNameE"/>
					</InvLastNameE>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvSumAcc>
						<!--Connection from Source Node "File006@InvSumAcc" to Destination Node "InvSumAcc"-->
						<xsl:value-of select="File006@InvSumAcc"/>
					</InvSumAcc>
				</xsl:for-each>
				<xsl:for-each select="File006">
					<InvSumNis>
						<!--Connection from Source Node "File006@InvSumNis" to Destination Node "InvSumNis"-->
						<xsl:value-of select="File006@InvSumNis"/>
					</InvSumNis>
				</xsl:for-each>
			</File006>
		</TechnionData>
	</xsl:template>
	<msxsl:script language="VBScript" implements-prefix="user">
		<![CDATA[]]>
	</msxsl:script>
</xsl:stylesheet>

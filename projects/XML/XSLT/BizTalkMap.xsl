<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:var='urn:var' xmlns:user='urn:user' version='1.0'>
<xsl:output method='xml' indent='yes' omit-xml-declaration='yes' />
<xsl:template match='/'>
	<xsl:apply-templates select='T-Cnt_Aviv_Demog'/>
</xsl:template>
<xsl:template match='T-Cnt_Aviv_Demog'>
<Demog>
	<IdKey>
		<xsl:for-each select='T-Cnt_Aviv_Medicines'>
			<Medicines>
				<xsl:for-each select='R-Cnt_Aviv_Medicines'>
					<xsl:for-each select='Med_Cat_No'>
						<CatalogNo>
						<!-- Connection from source node "Med_Cat_No" to destination node "CatalogNo" -->
						<xsl:value-of select='./text()'/>
						</CatalogNo>
					</xsl:for-each>
				</xsl:for-each>
				<xsl:for-each select='R-Cnt_Aviv_Medicines'>
					<xsl:for-each select='Dosage'>
						<Dosage>
						<!-- Connection from source node "Dosage" to destination node "Dosage" -->
						<xsl:value-of select='./text()'/>
						</Dosage>
					</xsl:for-each>
				</xsl:for-each>
				<xsl:for-each select='R-Cnt_Aviv_Medicines'>
					<xsl:for-each select='Duration'>
						<UsePeriod>
						<!-- Connection from source node "Duration" to destination node "UsePeriod" -->
						<xsl:value-of select='./text()'/>
						</UsePeriod>
					</xsl:for-each>
				</xsl:for-each>
				<xsl:for-each select='R-Cnt_Aviv_Medicines'>
					<xsl:for-each select='Units'>
						<Units>
						<!-- Connection from source node "Units" to destination node "Units" -->
						<xsl:value-of select='./text()'/>
						</Units>
					</xsl:for-each>
				</xsl:for-each>
				<xsl:for-each select='R-Cnt_Aviv_Medicines'>
					<xsl:for-each select='Quantity'>
						<Quantity>
						<!-- Connection from source node "Quantity" to destination node "Quantity" -->
						<xsl:value-of select='./text()'/>
						</Quantity>
					</xsl:for-each>
				</xsl:for-each>
			<!-- Connection from source node "T-Cnt_Aviv_Medicines" to destination node "Medicines" -->
			<xsl:value-of select='./text()'/>
			</Medicines>
		</xsl:for-each>
		<xsl:for-each select='T-Cnt_Aviv_Labs_Header'>
			<xsl:for-each select='R-Cnt_Aviv_Labs_Header'>
				<LabsHeader>
					<xsl:for-each select='Test_Type'>
						<TestKind>
						<!-- Connection from source node "Test_Type" to destination node "TestKind" -->
						<xsl:value-of select='./text()'/>
						</TestKind>
					</xsl:for-each>
					<xsl:for-each select='Test_Date'>
						<TestDate>
						<!-- Connection from source node "Test_Date" to destination node "TestDate" -->
						<xsl:value-of select='./text()'/>
						</TestDate>
					</xsl:for-each>
					<xsl:for-each select='Test_No'>
						<TestNo>
						<!-- Connection from source node "Test_No" to destination node "TestNo" -->
						<xsl:value-of select='./text()'/>
						</TestNo>
					</xsl:for-each>
					<xsl:for-each select='Lab_Id'>
						<LaborCode>
						<!-- Connection from source node "Lab_Id" to destination node "LaborCode" -->
						<xsl:value-of select='./text()'/>
						</LaborCode>
					</xsl:for-each>
					<xsl:for-each select='Station_Id'>
						<StationCode>
						<!-- Connection from source node "Station_Id" to destination node "StationCode" -->
						<xsl:value-of select='./text()'/>
						</StationCode>
					</xsl:for-each>
					<xsl:for-each select='Substance_Type'>
						<StaffKinde>
						<!-- Connection from source node "Substance_Type" to destination node "StaffKinde" -->
						<xsl:value-of select='./text()'/>
						</StaffKinde>
					</xsl:for-each>
					<xsl:for-each select='Answer_State'>
						<Answer>
						<!-- Connection from source node "Answer_State" to destination node "Answer" -->
						<xsl:value-of select='./text()'/>
						</Answer>
					</xsl:for-each>
					<xsl:for-each select='Sent_By'>
						<SendedBy>
						<!-- Connection from source node "Sent_By" to destination node "SendedBy" -->
						<xsl:value-of select='./text()'/>
						</SendedBy>
					</xsl:for-each>
					<xsl:for-each select='Transfer_Date'>
						<TransferDate>
						<!-- Connection from source node "Transfer_Date" to destination node "TransferDate" -->
						<xsl:value-of select='./text()'/>
						</TransferDate>
					</xsl:for-each>
					<xsl:for-each select='Test_Unique_Id'>
						<UniqeCode>
						<!-- Connection from source node "Test_Unique_Id" to destination node "UniqeCode" -->
						<xsl:value-of select='./text()'/>
						</UniqeCode>
					</xsl:for-each>
					<xsl:for-each select='Lab_Origin'>
						<LabPlace>
						<!-- Connection from source node "Lab_Origin" to destination node "LabPlace" -->
						<xsl:value-of select='./text()'/>
						</LabPlace>
					</xsl:for-each>
					<xsl:for-each select='T-Cnt_Aviv_Labs_Results'>
						<xsl:for-each select='R-Cnt_Aviv_Labs_Results'>
							<LabsResalts>
								<xsl:for-each select='Test_Code'>
									<TestCode>
									<!-- Connection from source node "Test_Code" to destination node "TestCode" -->
									<xsl:value-of select='./text()'/>
									</TestCode>
								</xsl:for-each>
								<xsl:for-each select='Upper_Limit_Bound'>
									<UpBoder>
									<!-- Connection from source node "Upper_Limit_Bound" to destination node "UpBoder" -->
									<xsl:value-of select='./text()'/>
									</UpBoder>
								</xsl:for-each>
								<xsl:for-each select='Lower_Limit_Bound'>
									<DownBoder>
									<!-- Connection from source node "Lower_Limit_Bound" to destination node "DownBoder" -->
									<xsl:value-of select='./text()'/>
									</DownBoder>
								</xsl:for-each>
								<xsl:for-each select='Result'>
									<Resalt>
									<!-- Connection from source node "Result" to destination node "Resalt" -->
									<xsl:value-of select='./text()'/>
									</Resalt>
								</xsl:for-each>
								<xsl:for-each select='Graph'>
									<ResaltSchema>
									<!-- Connection from source node "Graph" to destination node "ResaltSchema" -->
									<xsl:value-of select='./text()'/>
									</ResaltSchema>
								</xsl:for-each>
								<xsl:for-each select='Messure_Unit'>
									<Unit>
									<!-- Connection from source node "Messure_Unit" to destination node "Unit" -->
									<xsl:value-of select='./text()'/>
									</Unit>
								</xsl:for-each>
								<xsl:for-each select='Short_Description'>
									<Description>
									<!-- Connection from source node "Short_Description" to destination node "Description" -->
									<xsl:value-of select='./text()'/>
									</Description>
								</xsl:for-each>
								<xsl:for-each select='Additional_Remarks'>
									<AddComments>
									<!-- Connection from source node "Additional_Remarks" to destination node "AddComments" -->
									<xsl:value-of select='./text()'/>
									</AddComments>
								</xsl:for-each>
								<xsl:for-each select='Line_No'>
									<LineNo>
									<!-- Connection from source node "Line_No" to destination node "LineNo" -->
									<xsl:value-of select='./text()'/>
									</LineNo>
								</xsl:for-each>
							<!-- Connection from source node "R-Cnt_Aviv_Labs_Results" to destination node "LabsResalts" -->
							<xsl:value-of select='./text()'/>
							</LabsResalts>
						</xsl:for-each>
					</xsl:for-each>
				<!-- Connection from source node "R-Cnt_Aviv_Labs_Header" to destination node "LabsHeader" -->
				<xsl:value-of select='./text()'/>
				</LabsHeader>
			</xsl:for-each>
		</xsl:for-each>
	</IdKey>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Name'>
			<LastName>
			<!-- Connection from source node "Name" to destination node "LastName" -->
			<xsl:value-of select='./text()'/>
			</LastName>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='F_Name'>
			<FirstName>
			<!-- Connection from source node "F_Name" to destination node "FirstName" -->
			<xsl:value-of select='./text()'/>
			</FirstName>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Father_Name'>
			<FatherName>
			<!-- Connection from source node "Father_Name" to destination node "FatherName" -->
			<xsl:value-of select='./text()'/>
			</FatherName>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Address'>
			<Address>
			<!-- Connection from source node "Address" to destination node "Address" -->
			<xsl:value-of select='./text()'/>
			</Address>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Sex'>
			<Sex>
			<!-- Connection from source node "Sex" to destination node "Sex" -->
			<xsl:value-of select='./text()'/>
			</Sex>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Birth_Date'>
			<BirthDate>
			<!-- Connection from source node "Birth_Date" to destination node "BirthDate" -->
			<xsl:value-of select='./text()'/>
			</BirthDate>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Family_Status'>
			<FamilyStatus>
			<!-- Connection from source node "Family_Status" to destination node "FamilyStatus" -->
			<xsl:value-of select='./text()'/>
			</FamilyStatus>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='No_Of_Children'>
			<NoChild>
			<!-- Connection from source node "No_Of_Children" to destination node "NoChild" -->
			<xsl:value-of select='./text()'/>
			</NoChild>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Deceased'>
			<Deceased>
			<!-- Connection from source node "Deceased" to destination node "Deceased" -->
			<xsl:value-of select='./text()'/>
			</Deceased>
		</xsl:for-each>
	</xsl:for-each>
	<xsl:for-each select='R-Cnt_Aviv_Demog'>
		<xsl:for-each select='Id_No'>
			<Id>
			<!-- Connection from source node "Id_No" to destination node "Id" -->
			<xsl:value-of select='./text()'/>
			</Id>
		</xsl:for-each>
	</xsl:for-each>
</Demog>
</xsl:template>
<msxsl:script language='VBScript' implements-prefix='user'>
<![CDATA[

Function FctGetDefValue(strNodeValue, strDefValue)
  If strNodeValue <> "" Then
    FctGetDefValue = strNodeValue
  Else
    FctGetDefValue = strDefValue
  End If
End Function

]]>
</msxsl:script>
</xsl:stylesheet>

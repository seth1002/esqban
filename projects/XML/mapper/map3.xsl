<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:var='urn:var' xmlns:user='urn:user' version='1.0'>
<xsl:output method='xml' indent='yes' omit-xml-declaration='yes' />
<xsl:template match='/'>
	<xsl:apply-templates select='MasterSelect'/>
</xsl:template>
<xsl:template match='MasterSelect'>
<T-Cnt_Aviv_Demog>
	<R-Cnt_Aviv_Demog>
		<Id_No>
		<!-- Connection from source node "ID_NO" to destination node "Id_No" -->
		<xsl:value-of select='@ID_NO'/>
		</Id_No>
		<F_Name>
		<!-- Connection from source node "F_NAME" to destination node "F_Name" -->
		<xsl:value-of select='@F_NAME'/>
		</F_Name>
		<Name>
		<!-- Connection from source node "NAME" to destination node "Name" -->
		<xsl:value-of select='@NAME'/>
		</Name>
		<Father_Name>
		<!-- Connection from source node "FATHER_NAME" to destination node "Father_Name" -->
		<xsl:value-of select='@FATHER_NAME'/>
		</Father_Name>
		<Address>
		<!-- Connection from source node "ADDRESS" to destination node "Address" -->
		<xsl:value-of select='@ADDRESS'/>
		</Address>
		<Sex>
		<!-- Connection from source node "SEX" to destination node "Sex" -->
		<xsl:value-of select='@SEX'/>
		</Sex>
		<Birth_Date>
		<!-- Connection from source node "BIRTH_DATE" to destination node "Birth_Date" -->
		<xsl:value-of select='@BIRTH_DATE'/>
		</Birth_Date>
		<Family_Status>
		<!-- Connection from source node "FAMILY_STATUS" to destination node "Family_Status" -->
		<xsl:value-of select='@FAMILY_STATUS'/>
		</Family_Status>
		<No_Of_Children>
		<!-- Connection from source node "NO_OF_CHILDREN" to destination node "No_Of_Children" -->
		<xsl:value-of select='@NO_OF_CHILDREN'/>
		</No_Of_Children>
		<Deceased>
		<!-- Connection from source node "DECEASED" to destination node "Deceased" -->
		<xsl:value-of select='@DECEASED'/>
		</Deceased>
	</R-Cnt_Aviv_Demog>
	<T-Cnt_Aviv_Medicines>
		<R-Cnt_Aviv_Medicines>
			<xsl:for-each select='Medicines'>
				<Med_Cat_No>
				<!-- Connection from source node "FIELD1" to destination node "Med_Cat_No" -->
				<xsl:value-of select='@FIELD1'/>
				</Med_Cat_No>
			</xsl:for-each>
			<xsl:for-each select='Medicines'>
				<Demog_Rec_Id>
				<!-- Connection from source node "FIELD2" to destination node "Demog_Rec_Id" -->
				<xsl:value-of select='@FIELD2'/>
				</Demog_Rec_Id>
			</xsl:for-each>
			<xsl:for-each select='Medicines'>
				<Dosage>
				<!-- Connection from source node "FIELD3" to destination node "Dosage" -->
				<xsl:value-of select='@FIELD3'/>
				</Dosage>
			</xsl:for-each>
			<xsl:for-each select='Medicines'>
				<Remarks>
				<!-- Connection from source node "FIELD4" to destination node "Remarks" -->
				<xsl:value-of select='@FIELD4'/>
				</Remarks>
			</xsl:for-each>
			<xsl:for-each select='Medicines'>
				<Duration>
				<!-- Connection from source node "FIELD5" to destination node "Duration" -->
				<xsl:value-of select='@FIELD5'/>
				</Duration>
			</xsl:for-each>
			<xsl:for-each select='Medicines'>
				<Units>
				<!-- Connection from source node "FIELD6" to destination node "Units" -->
				<xsl:value-of select='@FIELD6'/>
				</Units>
			</xsl:for-each>
			<xsl:for-each select='Medicines'>
				<Quantity>
				<!-- Connection from source node "FIELD7" to destination node "Quantity" -->
				<xsl:value-of select='@FIELD7'/>
				</Quantity>
			</xsl:for-each>
		</R-Cnt_Aviv_Medicines>
	</T-Cnt_Aviv_Medicines>
	<T-Cnt_Aviv_Labs_Header>
		<R-Cnt_Aviv_Labs_Header>
			<xsl:for-each select='LabsHeader'>
				<Test_Type>
				<!-- Connection from source node "FIELD1" to destination node "Test_Type" -->
				<xsl:value-of select='@FIELD1'/>
				</Test_Type>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Rec_Id>
				<!-- Connection from source node "FIELD2" to destination node "Rec_Id" -->
				<xsl:value-of select='@FIELD2'/>
				</Rec_Id>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Demog_Rec_Id>
				<!-- Connection from source node "FIELD3" to destination node "Demog_Rec_Id" -->
				<xsl:value-of select='@FIELD3'/>
				</Demog_Rec_Id>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Test_Date>
				<!-- Connection from source node "FIELD4" to destination node "Test_Date" -->
				<xsl:value-of select='@FIELD4'/>
				</Test_Date>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Test_No>
				<!-- Connection from source node "FIELD5" to destination node "Test_No" -->
				<xsl:value-of select='@FIELD5'/>
				</Test_No>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Lab_Id>
				<!-- Connection from source node "FIELD6" to destination node "Lab_Id" -->
				<xsl:value-of select='@FIELD6'/>
				</Lab_Id>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Station_Id>
				<!-- Connection from source node "FIELD7" to destination node "Station_Id" -->
				<xsl:value-of select='@FIELD7'/>
				</Station_Id>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Substance_Type>
				<!-- Connection from source node "FIELD8" to destination node "Substance_Type" -->
				<xsl:value-of select='@FIELD8'/>
				</Substance_Type>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Answer_State>
				<!-- Connection from source node "FIELD9" to destination node "Answer_State" -->
				<xsl:value-of select='@FIELD9'/>
				</Answer_State>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Sent_By>
				<!-- Connection from source node "FIELD10" to destination node "Sent_By" -->
				<xsl:value-of select='@FIELD10'/>
				</Sent_By>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Transfer_Date>
				<!-- Connection from source node "FIELD11" to destination node "Transfer_Date" -->
				<xsl:value-of select='@FIELD11'/>
				</Transfer_Date>
			</xsl:for-each>
			<xsl:for-each select='LabsHeader'>
				<Test_Unique_Id>
				<!-- Connection from source node "FIELD12" to destination node "Test_Unique_Id" -->
				<xsl:value-of select='@FIELD12'/>
				</Test_Unique_Id>
			</xsl:for-each>
			<Lab_Origin>
			<!-- Connection from source node "ORIGIN" to destination node "Lab_Origin" -->
			<xsl:value-of select='@ORIGIN'/>
			</Lab_Origin>
			<T-Cnt_Aviv_Labs_Results>
				<R-Cnt_Aviv_Labs_Results>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Test_Rec_Id>
							<!-- Connection from source node "FIELD1" to destination node "Test_Rec_Id" -->
							<xsl:value-of select='@FIELD1'/>
							</Test_Rec_Id>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Test_Code>
							<!-- Connection from source node "FIELD2" to destination node "Test_Code" -->
							<xsl:value-of select='@FIELD2'/>
							</Test_Code>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Upper_Limit_Bound>
							<!-- Connection from source node "FIELD3" to destination node "Upper_Limit_Bound" -->
							<xsl:value-of select='@FIELD3'/>
							</Upper_Limit_Bound>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Lower_Limit_Bound>
							<!-- Connection from source node "FIELD4" to destination node "Lower_Limit_Bound" -->
							<xsl:value-of select='@FIELD4'/>
							</Lower_Limit_Bound>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Result>
							<!-- Connection from source node "FIELD5" to destination node "Result" -->
							<xsl:value-of select='@FIELD5'/>
							</Result>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Graph>
							<!-- Connection from source node "FIELD6" to destination node "Graph" -->
							<xsl:value-of select='@FIELD6'/>
							</Graph>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Messure_Unit>
							<!-- Connection from source node "FIELD7" to destination node "Messure_Unit" -->
							<xsl:value-of select='@FIELD7'/>
							</Messure_Unit>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Short_Description>
							<!-- Connection from source node "FIELD8" to destination node "Short_Description" -->
							<xsl:value-of select='@FIELD8'/>
							</Short_Description>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Additional_Remarks>
							<!-- Connection from source node "FIELD9" to destination node "Additional_Remarks" -->
							<xsl:value-of select='@FIELD9'/>
							</Additional_Remarks>
						</xsl:for-each>
					</xsl:for-each>
					<xsl:for-each select='LabsHeader'>
						<xsl:for-each select='LabsResults'>
							<Line_No>
							<!-- Connection from source node "FIELD10" to destination node "Line_No" -->
							<xsl:value-of select='@FIELD10'/>
							</Line_No>
						</xsl:for-each>
					</xsl:for-each>
				</R-Cnt_Aviv_Labs_Results>
			</T-Cnt_Aviv_Labs_Results>
		</R-Cnt_Aviv_Labs_Header>
	</T-Cnt_Aviv_Labs_Header>
</T-Cnt_Aviv_Demog>
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

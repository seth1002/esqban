<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
	<xsl:template match="/">
		<xsl:apply-templates select="ServiceData/BL_NLFR_REPORT/LIST_G_PAYING_CUST_NO/G_PAYING_CUST_NO"/>
	</xsl:template>
	<xsl:template match="ServiceData/BL_NLFR_REPORT/LIST_G_PAYING_CUST_NO/G_PAYING_CUST_NO">
		<G_PAYING_CUST_NO>
			<!--Connection from Source Node "G_PAYING_CUST_NO" to Destination Node "G_PAYING_CUST_NO"-->
			<xsl:value-of select="./text()"/>
			<xsl:for-each select="PAYING_CUST_NO">
				<PAYING_CUST_NO>
					<!--Connection from Source Node "PAYING_CUST_NO" to Destination Node "PAYING_CUST_NO"-->
					<xsl:value-of select="./text()"/>
				</PAYING_CUST_NO>
			</xsl:for-each>
			<xsl:for-each select="CUST_NO">
				<CUST_NO>
					<!--Connection from Source Node "CUST_NO" to Destination Node "CUST_NO"-->
					<xsl:value-of select="./text()"/>
				</CUST_NO>
			</xsl:for-each>
			<xsl:for-each select="INVOICE_NO">
				<INVOICE_NO>
					<xsl:variable name="var:v1" select="user:convertDate(string(./text()), string(ancestor::*[1]/LIST_G_ATAR/G_ATAR/LIST_G_LINE/G_LINE/LIST_G_TERM_CODE/G_TERM_CODE/CURRENCY_CODE/text()))"/>
					<xsl:value-of select="$var:v1"/>
				</INVOICE_NO>
			</xsl:for-each>
			<xsl:for-each select="INVOICE_YEAR">
				<INVOICE_YEAR>
					<!--Connection from Source Node "INVOICE_YEAR" to Destination Node "INVOICE_YEAR"-->
					<xsl:value-of select="./text()"/>
				</INVOICE_YEAR>
			</xsl:for-each>
			<xsl:for-each select="LIST_G_ATAR">
				<LIST_G_ATAR>
					<!--Connection from Source Node "LIST_G_ATAR" to Destination Node "LIST_G_ATAR"-->
					<xsl:value-of select="./text()"/>
					<xsl:for-each select="G_ATAR">
						<G_ATAR>
							<!--Connection from Source Node "G_ATAR" to Destination Node "G_ATAR"-->
							<xsl:value-of select="./text()"/>
							<xsl:for-each select="LIST_G_LINE">
								<LIST_G_LINE>
									<!--Connection from Source Node "LIST_G_LINE" to Destination Node "LIST_G_LINE"-->
									<xsl:value-of select="./text()"/>
									<xsl:for-each select="G_LINE">
										<G_LINE>
											<!--Connection from Source Node "G_LINE" to Destination Node "G_LINE"-->
											<xsl:value-of select="./text()"/>
											<xsl:for-each select="LINE_ID">
												<LINE_ID>
													<!--Connection from Source Node "LINE_ID" to Destination Node "LINE_ID"-->
													<xsl:value-of select="./text()"/>
												</LINE_ID>
											</xsl:for-each>
											<xsl:for-each select="LIST_G_TERM_CODE">
												<LIST_G_TERM_CODE>
													<!--Connection from Source Node "LIST_G_TERM_CODE" to Destination Node "LIST_G_TERM_CODE"-->
													<xsl:value-of select="./text()"/>
													<xsl:for-each select="G_TERM_CODE">
														<G_TERM_CODE>
															<!--Connection from Source Node "G_TERM_CODE" to Destination Node "G_TERM_CODE"-->
															<xsl:value-of select="./text()"/>
															<xsl:for-each select="CURRENCY_CODE">
																<CURRENCY_CODE>
																	<!--Connection from Source Node "CURRENCY_CODE" to Destination Node "CURRENCY_CODE"-->
																	<xsl:value-of select="./text()"/>
																</CURRENCY_CODE>
															</xsl:for-each>
															<xsl:for-each select="ancestor::*[6]/CF_AFTER_HANAHA_CLIENT">
																<TOTAL_PRICE_DOLAR>
																	<!--Connection from Source Node "CF_AFTER_HANAHA_CLIENT" to Destination Node "TOTAL_PRICE_DOLAR"-->
																	<xsl:value-of select="./text()"/>
																</TOTAL_PRICE_DOLAR>
															</xsl:for-each>
															<xsl:for-each select="ancestor::*[6]/LIST_G_INVOICE/LIST_G_INVOICE/LIST_G_INVOICE">
																<CF_SHIRUT>
																	<!--Connection from Source Node "ORD" to Destination Node "CF_SHIRUT"-->
																	<xsl:value-of select="./text()"/>
																</CF_SHIRUT>
															</xsl:for-each>
															<xsl:for-each select="ancestor::*[2]/LIST_G_HANAHA_LINE">
																<CF_SHEKEL_PRICE>
																	<!--Connection from Source Node "LIST_G_HANAHA_LINE" to Destination Node "CF_SHEKEL_PRICE"-->
																	<xsl:value-of select="./text()"/>
																</CF_SHEKEL_PRICE>
															</xsl:for-each>
														</G_TERM_CODE>
													</xsl:for-each>
												</LIST_G_TERM_CODE>
											</xsl:for-each>
											<xsl:for-each select="ancestor::*[4]/INVOICE_NO">
												<CF_KAV>
													<xsl:variable name="var:v2" select="user:convertDate(string(./text()), string(ancestor::*[1]/LIST_G_ATAR/G_ATAR/LIST_G_LINE/G_LINE/LIST_G_TERM_CODE/G_TERM_CODE/CURRENCY_CODE/text()))"/>
													<xsl:value-of select="$var:v2"/>
												</CF_KAV>
											</xsl:for-each>
											<xsl:for-each select="CP_LINE_TYPE">
												<CP_LINE_TYPE>
													<!--Connection from Source Node "CP_LINE_TYPE" to Destination Node "CP_LINE_TYPE"-->
													<xsl:value-of select="./text()"/>
												</CP_LINE_TYPE>
											</xsl:for-each>
											<xsl:for-each select="CP_DESTINATION">
												<CP_DESTINATION>
													<!--Connection from Source Node "CP_DESTINATION" to Destination Node "CP_DESTINATION"-->
													<xsl:value-of select="./text()"/>
												</CP_DESTINATION>
											</xsl:for-each>
											<xsl:for-each select="CP_PORT_SPEED">
												<CP_PORT_SPEED>
													<!--Connection from Source Node "CP_PORT_SPEED" to Destination Node "CP_PORT_SPEED"-->
													<xsl:value-of select="./text()"/>
												</CP_PORT_SPEED>
											</xsl:for-each>
											<xsl:for-each select="CP_ORIGIN">
												<CP_ORIGIN>
													<!--Connection from Source Node "CP_ORIGIN" to Destination Node "CP_ORIGIN"-->
													<xsl:value-of select="./text()"/>
												</CP_ORIGIN>
											</xsl:for-each>
										</G_LINE>
									</xsl:for-each>
								</LIST_G_LINE>
							</xsl:for-each>
							<xsl:for-each select="CF_ATAR">
								<CF_ATAR>
									<!--Connection from Source Node "CF_ATAR" to Destination Node "CF_ATAR"-->
									<xsl:value-of select="./text()"/>
								</CF_ATAR>
							</xsl:for-each>
							<xsl:for-each select="CP_AFTER_HANAHA_ATAR">
								<CP_AFTER_HANAHA_ATAR>
									<!--Connection from Source Node "CP_AFTER_HANAHA_ATAR" to Destination Node "CP_AFTER_HANAHA_ATAR"-->
									<xsl:value-of select="./text()"/>
								</CP_AFTER_HANAHA_ATAR>
							</xsl:for-each>
						</G_ATAR>
					</xsl:for-each>
				</LIST_G_ATAR>
			</xsl:for-each>
			<xsl:for-each select="LIST_G_HANAHOT">
				<LIST_G_HANAHOT>
					<!--Connection from Source Node "LIST_G_HANAHOT" to Destination Node "LIST_G_HANAHOT"-->
					<xsl:value-of select="./text()"/>
					<xsl:for-each select="G_HANAHOT">
						<G_HANAHOT>
							<!--Connection from Source Node "G_HANAHOT" to Destination Node "G_HANAHOT"-->
							<xsl:value-of select="./text()"/>
							<xsl:for-each select="ACHUZ_HANACHA_BLLING">
								<ACHUZ_HANACHA_BLLING>
									<!--Connection from Source Node "ACHUZ_HANACHA_BLLING" to Destination Node "ACHUZ_HANACHA_BLLING"-->
									<xsl:value-of select="./text()"/>
								</ACHUZ_HANACHA_BLLING>
							</xsl:for-each>
							<xsl:for-each select="SUM_TOTAL_PRICE_HANACHA">
								<SUM_TOTAL_PRICE_HANACHA>
									<!--Connection from Source Node "SUM_TOTAL_PRICE_HANACHA" to Destination Node "SUM_TOTAL_PRICE_HANACHA"-->
									<xsl:value-of select="./text()"/>
								</SUM_TOTAL_PRICE_HANACHA>
							</xsl:for-each>
							<xsl:for-each select="CURR_CLIENT">
								<CURR_CLIENT>
									<!--Connection from Source Node "CURR_CLIENT" to Destination Node "CURR_CLIENT"-->
									<xsl:value-of select="./text()"/>
								</CURR_CLIENT>
							</xsl:for-each>
							<xsl:for-each select="CF_SHEKEL_HANACHA_CLIENT">
								<CF_SHEKEL_HANACHA_CLIENT>
									<!--Connection from Source Node "CF_SHEKEL_HANACHA_CLIENT" to Destination Node "CF_SHEKEL_HANACHA_CLIENT"-->
									<xsl:value-of select="./text()"/>
								</CF_SHEKEL_HANACHA_CLIENT>
							</xsl:for-each>
						</G_HANAHOT>
					</xsl:for-each>
				</LIST_G_HANAHOT>
			</xsl:for-each>
			<xsl:for-each select="LIST_G_INVOICE">
				<LIST_G_INVOICE>
					<!--Connection from Source Node "LIST_G_INVOICE" to Destination Node "LIST_G_INVOICE"-->
					<xsl:value-of select="./text()"/>
					<xsl:for-each select="G_INVOICE">
						<G_INVOICE>
							<!--Connection from Source Node "G_INVOICE" to Destination Node "G_INVOICE"-->
							<xsl:value-of select="./text()"/>
							<xsl:for-each select="REASON">
								<REASON>
									<!--Connection from Source Node "REASON" to Destination Node "REASON"-->
									<xsl:value-of select="./text()"/>
								</REASON>
							</xsl:for-each>
							<xsl:for-each select="TOT_AMOU">
								<TOT_AMOU>
									<!--Connection from Source Node "TOT_AMOU" to Destination Node "TOT_AMOU"-->
									<xsl:value-of select="./text()"/>
								</TOT_AMOU>
							</xsl:for-each>
						</G_INVOICE>
					</xsl:for-each>
				</LIST_G_INVOICE>
			</xsl:for-each>
			<xsl:for-each select="CF_MAIN_CUST_NO">
				<CF_MAIN_CUST_NO>
					<!--Connection from Source Node "CF_MAIN_CUST_NO" to Destination Node "CF_MAIN_CUST_NO"-->
					<xsl:value-of select="./text()"/>
				</CF_MAIN_CUST_NO>
			</xsl:for-each>
			<xsl:for-each select="CP_CONTACTS">
				<CP_CONTACTS>
					<!--Connection from Source Node "CP_CONTACTS" to Destination Node "CP_CONTACTS"-->
					<xsl:value-of select="./text()"/>
				</CP_CONTACTS>
			</xsl:for-each>
			<xsl:for-each select="CP_KTOVET_POB">
				<CP_KTOVET_POB>
					<!--Connection from Source Node "CP_KTOVET_POB" to Destination Node "CP_KTOVET_POB"-->
					<xsl:value-of select="./text()"/>
				</CP_KTOVET_POB>
			</xsl:for-each>
			<xsl:for-each select="CP_CITY_ZIP">
				<CP_CITY_ZIP>
					<!--Connection from Source Node "CP_CITY_ZIP" to Destination Node "CP_CITY_ZIP"-->
					<xsl:value-of select="./text()"/>
				</CP_CITY_ZIP>
			</xsl:for-each>
			<xsl:for-each select="CP_ADDRESS_NAME">
				<CP_ADDRESS_NAME>
					<!--Connection from Source Node "CP_ADDRESS_NAME" to Destination Node "CP_ADDRESS_NAME"-->
					<xsl:value-of select="./text()"/>
				</CP_ADDRESS_NAME>
			</xsl:for-each>
			<xsl:for-each select="CS_CLIENT_SHUM">
				<CS_CLIENT_SHUM>
					<!--Connection from Source Node "CS_CLIENT_SHUM" to Destination Node "CS_CLIENT_SHUM"-->
					<xsl:value-of select="./text()"/>
				</CS_CLIENT_SHUM>
			</xsl:for-each>
			<xsl:for-each select="CS_CLIENT_HANAHA">
				<CS_CLIENT_HANAHA>
					<!--Connection from Source Node "CS_CLIENT_HANAHA" to Destination Node "CS_CLIENT_HANAHA"-->
					<xsl:value-of select="./text()"/>
				</CS_CLIENT_HANAHA>
			</xsl:for-each>
			<xsl:for-each select="CP_CURRENCY_XML">
				<CURRENCY_XML>
					<!--Connection from Source Node "CP_CURRENCY_XML" to Destination Node "CURRENCY_XML"-->
					<xsl:value-of select="./text()"/>
				</CURRENCY_XML>
			</xsl:for-each>
			<xsl:for-each select="CP_MONTH_XML">
				<MON>
					<xsl:variable name="var:v3" select="user:fncmonth(string(./text()))"/>
					<xsl:value-of select="$var:v3"/>
				</MON>
			</xsl:for-each>
		</G_PAYING_CUST_NO>
	</xsl:template>
	<msxsl:script language="VBScript" implements-prefix="user">
		<![CDATA[function convertDate(param1, param2)    
convertDate=param2 & "_" & param1
end function

function fncmonth(Par1)  
fncmonth = Par1
end function
]]>
	</msxsl:script>
</xsl:stylesheet>

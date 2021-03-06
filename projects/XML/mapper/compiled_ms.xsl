<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:var='urn:var' xmlns:user='urn:user' version='1.0'>
<xsl:output method='xml' indent='yes' omit-xml-declaration='yes' />
<xsl:template match='/'>
	<xsl:apply-templates select='BL_NLFR_REPORT'/>
</xsl:template>
<xsl:template match='BL_NLFR_REPORT'>
<ServiceData>
	<BL_NLFR_REPORT>
		<LIST_G_PAYING_CUST_NO>
			<xsl:for-each select='LIST_G_PAYING_CUST_NO'>
				<xsl:for-each select='G_PAYING_CUST_NO'>
					<xsl:for-each select='PAYING_CUST_NO'>
						<G_PAYING_CUST_NO>
							<!-- Connection from source node "PAYING_CUST_NO" to destination node "PAYING_CUST_NO" -->
							<PAYING_CUST_NO><xsl:value-of select='./text()'/></PAYING_CUST_NO>
							<!-- Connection from source node "CUST_NO" to destination node "CUST_NO" -->
							<CUST_NO><xsl:value-of select='ancestor::*[1]/CUST_NO/text()'/></CUST_NO>
							<!-- Connection from source node "INVOICE_NO" to destination node "INVOICE_NO" -->
							<INVOICE_NO><xsl:value-of select='ancestor::*[1]/INVOICE_NO/text()'/></INVOICE_NO>
							<!-- Connection from source node "INVOICE_YEAR" to destination node "INVOICE_YEAR" -->
							<INVOICE_YEAR><xsl:value-of select='ancestor::*[1]/INVOICE_YEAR/text()'/></INVOICE_YEAR>
							<LIST_G_ATAR>
								<xsl:for-each select='ancestor::*[1]/LIST_G_ATAR'>
									<xsl:for-each select='G_ATAR'>
										<xsl:for-each select='ATAR'>
											<G_ATAR>
												<!-- Connection from source node "ATAR" to destination node "ATAR" -->
												<ATAR><xsl:value-of select='./text()'/></ATAR>
												<LIST_G_LINE>
													<xsl:for-each select='ancestor::*[1]/LIST_G_LINE'>
														<xsl:for-each select='G_LINE'>
															<xsl:for-each select='LINE_ID'>
																<G_LINE>
																	<!-- Connection from source node "LINE_ID" to destination node "LINE_ID" -->
																	<LINE_ID><xsl:value-of select='./text()'/></LINE_ID>
																	<LIST_G_TERM_CODE>
																		<xsl:for-each select='ancestor::*[1]/LIST_G_TERM_CODE'>
																			<xsl:for-each select='G_TERM_CODE'>
																				<xsl:for-each select='TERM_CODE'>
																					<G_TERM_CODE>
																						<!-- Connection from source node "TERM_CODE" to destination node "TERM_CODE" -->
																						<TERM_CODE><xsl:value-of select='./text()'/></TERM_CODE>
																						<!-- Connection from source node "CURRENCY_CODE" to destination node "CURRENCY_CODE" -->
																						<CURRENCY_CODE><xsl:value-of select='ancestor::*[1]/CURRENCY_CODE/text()'/></CURRENCY_CODE>
																						<!-- Connection from source node "TOTAL_PRICE_BEFOR_HANACHA" to destination node "TOTAL_PRICE_BEFOR_HANACHA" -->
																						<TOTAL_PRICE_BEFOR_HANACHA><xsl:value-of select='ancestor::*[1]/TOTAL_PRICE_BEFOR_HANACHA/text()'/></TOTAL_PRICE_BEFOR_HANACHA>
																						<!-- Connection from source node "TOTAL_INVOICE_PRICE" to destination node "TOTAL_INVOICE_PRICE" -->
																						<TOTAL_INVOICE_PRICE><xsl:value-of select='ancestor::*[1]/TOTAL_INVOICE_PRICE/text()'/></TOTAL_INVOICE_PRICE>
																						<!-- Connection from source node "CF_SHIRUT" to destination node "CF_SHIRUT" -->
																						<CF_SHIRUT><xsl:value-of select='ancestor::*[1]/CF_SHIRUT/text()'/></CF_SHIRUT>
																						<!-- Connection from source node "CF_SHEKEL_PRICE" to destination node "CF_SHEKEL_PRICE" -->
																						<CF_SHEKEL_PRICE><xsl:value-of select='ancestor::*[1]/CF_SHEKEL_PRICE/text()'/></CF_SHEKEL_PRICE>
																					</G_TERM_CODE>
																				</xsl:for-each>
																			</xsl:for-each>
																		</xsl:for-each>
																	</LIST_G_TERM_CODE>
																	<!-- Connection from source node "LIST_G_HANAHA_LINE" to destination node "LIST_G_HANAHA_LINE" -->
																	<LIST_G_HANAHA_LINE><xsl:value-of select='ancestor::*[1]/LIST_G_HANAHA_LINE/text()'/></LIST_G_HANAHA_LINE>
																	<!-- Connection from source node "CF_KAV" to destination node "CF_KAV" -->
																	<CF_KAV><xsl:value-of select='ancestor::*[1]/CF_KAV/text()'/></CF_KAV>
																	<!-- Connection from source node "CP_LINE_DIRECTION" to destination node "CP_LINE_DIRECTION" -->
																	<CP_LINE_DIRECTION><xsl:value-of select='ancestor::*[1]/CP_LINE_DIRECTION/text()'/></CP_LINE_DIRECTION>
																	<!-- Connection from source node "CP_LINE_TYPE" to destination node "CP_LINE_TYPE" -->
																	<CP_LINE_TYPE><xsl:value-of select='ancestor::*[1]/CP_LINE_TYPE/text()'/></CP_LINE_TYPE>
																	<!-- Connection from source node "CP_DESTINATION" to destination node "CP_DESTINATION" -->
																	<CP_DESTINATION><xsl:value-of select='ancestor::*[1]/CP_DESTINATION/text()'/></CP_DESTINATION>
																	<!-- Connection from source node "CP_PORT_SPEED" to destination node "CP_PORT_SPEED" -->
																	<CP_PORT_SPEED><xsl:value-of select='ancestor::*[1]/CP_PORT_SPEED/text()'/></CP_PORT_SPEED>
																	<!-- Connection from source node "CP_ORIGIN" to destination node "CP_ORIGIN" -->
																	<CP_ORIGIN><xsl:value-of select='ancestor::*[1]/CP_ORIGIN/text()'/></CP_ORIGIN>
																</G_LINE>
															</xsl:for-each>
														</xsl:for-each>
													</xsl:for-each>
												</LIST_G_LINE>
												<!-- Connection from source node "LIST_G_HANAHA_ATAR" to destination node "LIST_G_HANAHA_ATAR" -->
												<LIST_G_HANAHA_ATAR><xsl:value-of select='ancestor::*[1]/LIST_G_HANAHA_ATAR/text()'/></LIST_G_HANAHA_ATAR>
												<!-- Connection from source node "CS_SHERUT" to destination node "CS_SHERUT" -->
												<CS_SHERUT><xsl:value-of select='ancestor::*[1]/CS_SHERUT/text()'/></CS_SHERUT>
												<!-- Connection from source node "CS_HANACHA" to destination node "CS_HANACHA" -->
												<CS_HANACHA><xsl:value-of select='ancestor::*[1]/CS_HANACHA/text()'/></CS_HANACHA>
												<!-- Connection from source node "CF_ATAR" to destination node "CF_ATAR" -->
												<CF_ATAR><xsl:value-of select='ancestor::*[1]/CF_ATAR/text()'/></CF_ATAR>
												<!-- Connection from source node "CS_HANAHA_ATAR" to destination node "CS_HANAHA_ATAR" -->
												<CS_HANAHA_ATAR><xsl:value-of select='ancestor::*[1]/CS_HANAHA_ATAR/text()'/></CS_HANAHA_ATAR>
												<!-- Connection from source node "CF_SACH_ATAR" to destination node "CF_SACH_ATAR" -->
												<CF_SACH_ATAR><xsl:value-of select='ancestor::*[1]/CF_SACH_ATAR/text()'/></CF_SACH_ATAR>
												<!-- Connection from source node "CP_AFTER_HANAHA_ATAR" to destination node "CP_AFTER_HANAHA_ATAR" -->
												<CP_AFTER_HANAHA_ATAR><xsl:value-of select='ancestor::*[1]/CP_AFTER_HANAHA_ATAR/text()'/></CP_AFTER_HANAHA_ATAR>
											</G_ATAR>
										</xsl:for-each>
									</xsl:for-each>
								</xsl:for-each>
							</LIST_G_ATAR>
							<LIST_G_HANAHOT>
								<xsl:for-each select='ancestor::*[1]/LIST_G_HANAHOT'>
									<xsl:for-each select='G_HANAHOT'>
										<xsl:for-each select='ADIFUT'>
											<G_HANAHOT>
												<!-- Connection from source node "ADIFUT" to destination node "ADIFUT" -->
												<ADIFUT><xsl:value-of select='./text()'/></ADIFUT>
												<!-- Connection from source node "ACHUZ_HANACHA_BLLING" to destination node "ACHUZ_HANACHA_BLLING" -->
												<ACHUZ_HANACHA_BLLING><xsl:value-of select='ancestor::*[1]/ACHUZ_HANACHA_BLLING/text()'/></ACHUZ_HANACHA_BLLING>
												<!-- Connection from source node "PAYING_CLIENT" to destination node "PAYING_CLIENT" -->
												<PAYING_CLIENT><xsl:value-of select='ancestor::*[1]/PAYING_CLIENT/text()'/></PAYING_CLIENT>
												<!-- Connection from source node "SUM_TOTAL_PRICE_HANACHA" to destination node "SUM_TOTAL_PRICE_HANACHA" -->
												<SUM_TOTAL_PRICE_HANACHA><xsl:value-of select='ancestor::*[1]/SUM_TOTAL_PRICE_HANACHA/text()'/></SUM_TOTAL_PRICE_HANACHA>
												<!-- Connection from source node "CURR_CLIENT" to destination node "CURR_CLIENT" -->
												<CURR_CLIENT><xsl:value-of select='ancestor::*[1]/CURR_CLIENT/text()'/></CURR_CLIENT>
												<!-- Connection from source node "CF_SHEKEL_HANACHA_CLIENT" to destination node "CF_SHEKEL_HANACHA_CLIENT" -->
												<CF_SHEKEL_HANACHA_CLIENT><xsl:value-of select='ancestor::*[1]/CF_SHEKEL_HANACHA_CLIENT/text()'/></CF_SHEKEL_HANACHA_CLIENT>
											</G_HANAHOT>
										</xsl:for-each>
									</xsl:for-each>
								</xsl:for-each>
							</LIST_G_HANAHOT>
							<LIST_G_INVOICE>
								<xsl:for-each select='ancestor::*[1]/LIST_G_INVOICE'>
									<xsl:for-each select='G_INVOICE'>
										<xsl:for-each select='REASON'>
											<G_INVOICE>
												<!-- Connection from source node "REASON" to destination node "REASON" -->
												<REASON><xsl:value-of select='./text()'/></REASON>
												<!-- Connection from source node "TOT_AMOU" to destination node "TOT_AMOU" -->
												<TOT_AMOU><xsl:value-of select='ancestor::*[1]/TOT_AMOU/text()'/></TOT_AMOU>
												<!-- Connection from source node "ORD" to destination node "ORD" -->
												<ORD><xsl:value-of select='ancestor::*[1]/ORD/text()'/></ORD>
												<!-- Connection from source node "LINE_NO" to destination node "LINE_NO" -->
												<LINE_NO><xsl:value-of select='ancestor::*[1]/LINE_NO/text()'/></LINE_NO>
												<!-- Connection from source node "INV_NO" to destination node "INV_NO" -->
												<INV_NO><xsl:value-of select='ancestor::*[1]/INV_NO/text()'/></INV_NO>
												<!-- Connection from source node "INV_YEAR" to destination node "INV_YEAR" -->
												<INV_YEAR><xsl:value-of select='ancestor::*[1]/INV_YEAR/text()'/></INV_YEAR>
												<!-- Connection from source node "ASMACHTA_NO" to destination node "ASMACHTA_NO" -->
												<ASMACHTA_NO><xsl:value-of select='ancestor::*[1]/ASMACHTA_NO/text()'/></ASMACHTA_NO>
											</G_INVOICE>
										</xsl:for-each>
									</xsl:for-each>
								</xsl:for-each>
							</LIST_G_INVOICE>
							<!-- Connection from source node "CF_MAIN_CUST_NO" to destination node "CF_MAIN_CUST_NO" -->
							<CF_MAIN_CUST_NO><xsl:value-of select='ancestor::*[1]/CF_MAIN_CUST_NO/text()'/></CF_MAIN_CUST_NO>
							<!-- Connection from source node "CP_CONTACT_PERSON" to destination node "CP_CONTACT_PERSON" -->
							<CP_CONTACT_PERSON><xsl:value-of select='ancestor::*[1]/CP_CONTACT_PERSON/text()'/></CP_CONTACT_PERSON>
							<!-- Connection from source node "CP_CONTACTS" to destination node "CP_CONTACTS" -->
							<CP_CONTACTS><xsl:value-of select='ancestor::*[1]/CP_CONTACTS/text()'/></CP_CONTACTS>
							<!-- Connection from source node "CP_KTOVET_POB" to destination node "CP_KTOVET_POB" -->
							<CP_KTOVET_POB><xsl:value-of select='ancestor::*[1]/CP_KTOVET_POB/text()'/></CP_KTOVET_POB>
							<!-- Connection from source node "CP_ADDRESS_SEQ_NO" to destination node "CP_ADDRESS_SEQ_NO" -->
							<CP_ADDRESS_SEQ_NO><xsl:value-of select='ancestor::*[1]/CP_ADDRESS_SEQ_NO/text()'/></CP_ADDRESS_SEQ_NO>
							<!-- Connection from source node "CP_CITY_ZIP" to destination node "CP_CITY_ZIP" -->
							<CP_CITY_ZIP><xsl:value-of select='ancestor::*[1]/CP_CITY_ZIP/text()'/></CP_CITY_ZIP>
							<!-- Connection from source node "CP_ADDRESS_NAME" to destination node "CP_ADDRESS_NAME" -->
							<CP_ADDRESS_NAME><xsl:value-of select='ancestor::*[1]/CP_ADDRESS_NAME/text()'/></CP_ADDRESS_NAME>
							<!-- Connection from source node "CS_CLIENT_SHUM" to destination node "CS_CLIENT_SHUM" -->
							<CS_CLIENT_SHUM><xsl:value-of select='ancestor::*[1]/CS_CLIENT_SHUM/text()'/></CS_CLIENT_SHUM>
							<!-- Connection from source node "CS_CLIENT_HANAHA" to destination node "CS_CLIENT_HANAHA" -->
							<CS_CLIENT_HANAHA><xsl:value-of select='ancestor::*[1]/CS_CLIENT_HANAHA/text()'/></CS_CLIENT_HANAHA>
							<!-- Connection from source node "CS_COUNTER_ATAR" to destination node "CS_COUNTER_ATAR" -->
							<CS_COUNTER_ATAR><xsl:value-of select='ancestor::*[1]/CS_COUNTER_ATAR/text()'/></CS_COUNTER_ATAR>
							<!-- Connection from source node "CF_AFTER_HANAHA_CLIENT" to destination node "CF_AFTER_HANAHA_CLIENT" -->
							<CF_AFTER_HANAHA_CLIENT><xsl:value-of select='ancestor::*[1]/CF_AFTER_HANAHA_CLIENT/text()'/></CF_AFTER_HANAHA_CLIENT>
						</G_PAYING_CUST_NO>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:for-each>
		</LIST_G_PAYING_CUST_NO>
	</BL_NLFR_REPORT>
</ServiceData>
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

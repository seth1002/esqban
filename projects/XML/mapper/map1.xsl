<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
    <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
    <xsl:template match="/">
      <xsl:apply-templates select="ServiceData/BL_NLFR_REPORT/LIST_G_PAYING_CUST_NO"/>
 </xsl:template>
    <xsl:template match="ServiceData/BL_NLFR_REPORT/LIST_G_PAYING_CUST_NO">
      <BL_NLFR_REPORT>
        <LIST_G_PAYING_CUST_NO>
          <!--Connection from Source Node "LIST_G_PAYING_CUST_NO" to Destination Node "LIST_G_PAYING_CUST_NO"-->
          <xsl:value-of select="./text()"/>
          <G_PAYING_CUST_NO>
            <!--Connection from Source Node "G_PAYING_CUST_NO" to Destination Node "G_PAYING_CUST_NO"-->
            <xsl:value-of select="./text()"/>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <PAYING_CUST_NO>
                <!--Connection from Source Node "PAYING_CUST_NO" to Destination Node "PAYING_CUST_NO"-->
                <xsl:value-of select="./text()"/>
      </PAYING_CUST_NO>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <CUST_NO>
                <!--Connection from Source Node "CUST_NO" to Destination Node "CUST_NO"-->
                <xsl:value-of select="./text()"/>
      </CUST_NO>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="INVOICE_NO">
                <INVOICE_NO>
                  <!--Connection from Source Node "INVOICE_NO" to Destination Node "INVOICE_NO"-->
                  <xsl:value-of select="./text()"/>
       </INVOICE_NO>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="INVOICE_YEAR">
                <INVOICE_YEAR>
                  <!--Connection from Source Node "INVOICE_YEAR" to Destination Node "INVOICE_YEAR"-->
                  <xsl:value-of select="./text()"/>
       </INVOICE_YEAR>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
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
                                      <xsl:for-each select="CF_SHIRUT">
                                        <CF_SHIRUT>
                                          <!--Connection from Source Node "CF_SHIRUT" to Destination Node "CF_SHIRUT"-->
                                          <xsl:value-of select="./text()"/>
                   </CF_SHIRUT>
                  </xsl:for-each>
                                      <xsl:for-each select="CF_SHEKEL_PRICE">
                                        <CF_SHEKEL_PRICE>
                                          <!--Connection from Source Node "CF_SHEKEL_PRICE" to Destination Node "CF_SHEKEL_PRICE"-->
                                          <xsl:value-of select="./text()"/>
                   </CF_SHEKEL_PRICE>
                  </xsl:for-each>
                                      <xsl:for-each select="TOTAL_PRICE_BEFOR_HANACHA">
                                        <DOLAR_PRICE>
                                          <!--Connection from Source Node "TOTAL_PRICE_BEFOR_HANACHA" to Destination Node "DOLAR_PRICE"-->
                                          <xsl:value-of select="./text()"/>
                   </DOLAR_PRICE>
                  </xsl:for-each>
                 </G_TERM_CODE>
                </xsl:for-each>
               </LIST_G_TERM_CODE>
              </xsl:for-each>
                              <xsl:for-each select="CF_KAV">
                                <CF_KAV>
                                  <!--Connection from Source Node "CF_KAV" to Destination Node "CF_KAV"-->
                                  <xsl:value-of select="./text()"/>
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
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CP_CONTACTS">
                <CP_CONTACTS>
                  <!--Connection from Source Node "CP_CONTACTS" to Destination Node "CP_CONTACTS"-->
                  <xsl:value-of select="./text()"/>
       </CP_CONTACTS>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CP_KTOVET_POB">
                <CP_KTOVET_POB>
                  <!--Connection from Source Node "CP_KTOVET_POB" to Destination Node "CP_KTOVET_POB"-->
                  <xsl:value-of select="./text()"/>
       </CP_KTOVET_POB>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CP_CITY_ZIP">
                <CP_CITY_ZIP>
                  <!--Connection from Source Node "CP_CITY_ZIP" to Destination Node "CP_CITY_ZIP"-->
                  <xsl:value-of select="./text()"/>
       </CP_CITY_ZIP>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CP_ADDRESS_NAME">
                <CP_ADDRESS_NAME>
                  <!--Connection from Source Node "CP_ADDRESS_NAME" to Destination Node "CP_ADDRESS_NAME"-->
                  <xsl:value-of select="./text()"/>
       </CP_ADDRESS_NAME>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CS_CLIENT_SHUM">
                <CS_CLIENT_SHUM>
                  <!--Connection from Source Node "CS_CLIENT_SHUM" to Destination Node "CS_CLIENT_SHUM"-->
                  <xsl:value-of select="./text()"/>
       </CS_CLIENT_SHUM>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CS_CLIENT_HANAHA">
                <CS_CLIENT_HANAHA>
                  <!--Connection from Source Node "CS_CLIENT_HANAHA" to Destination Node "CS_CLIENT_HANAHA"-->
                  <xsl:value-of select="./text()"/>
       </CS_CLIENT_HANAHA>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CF_AFTER_HANAHA_CLIENT">
                <CF_AFTER_HANAHA_CLIENT>
                  <!--Connection from Source Node "CF_AFTER_HANAHA_CLIENT" to Destination Node "CF_AFTER_HANAHA_CLIENT"-->
                  <xsl:value-of select="./text()"/>
       </CF_AFTER_HANAHA_CLIENT>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="LIST_G_INVOICE">
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
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="LIST_G_HANAHOT">
                <xsl:for-each select="G_HANAHOT">
                  <HANAHOT>
                    <!--Connection from Source Node "G_HANAHOT" to Destination Node "HANAHOT"-->
                    <xsl:value-of select="./text()"/>
                    <xsl:for-each select="CF_SHEKEL_HANACHA_CLIENT">
                      <CF_SHEKEL_HANAHA>
                        <!--Connection from Source Node "CF_SHEKEL_HANACHA_CLIENT" to Destination Node "CF_SHEKEL_HANAHA"-->
                        <xsl:value-of select="./text()"/>
          </CF_SHEKEL_HANAHA>
         </xsl:for-each>
                    <xsl:for-each select="SUM_TOTAL_PRICE_HANACHA">
                      <SUM_TOTAL_PRICE_HANAHA_D>
                        <!--Connection from Source Node "SUM_TOTAL_PRICE_HANACHA" to Destination Node "SUM_TOTAL_PRICE_HANAHA_D"-->
                        <xsl:value-of select="./text()"/>
          </SUM_TOTAL_PRICE_HANAHA_D>
         </xsl:for-each>
                    <xsl:for-each select="ACHUZ_HANACHA_BLLING">
                      <ACHUZ>
                        <!--Connection from Source Node "ACHUZ_HANACHA_BLLING" to Destination Node "ACHUZ"-->
                        <xsl:value-of select="./text()"/>
          </ACHUZ>
         </xsl:for-each>
        </HANAHOT>
       </xsl:for-each>
      </xsl:for-each>
     </xsl:for-each>
            <xsl:for-each select="G_PAYING_CUST_NO">
              <xsl:for-each select="CF_MAIN_CUST_NO">
                <CF_MAIN_CUST_NO>
                  <!--Connection from Source Node "CF_MAIN_CUST_NO" to Destination Node "CF_MAIN_CUST_NO"-->
                  <xsl:value-of select="./text()"/>
       </CF_MAIN_CUST_NO>
      </xsl:for-each>
     </xsl:for-each>
    </G_PAYING_CUST_NO>
   </LIST_G_PAYING_CUST_NO>
  </BL_NLFR_REPORT>
 </xsl:template>
    <msxsl:script language="VBScript" implements-prefix="user"/>
</xsl:stylesheet>
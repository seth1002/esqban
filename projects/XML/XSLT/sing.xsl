<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
    <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
    <xsl:template match="/">
      <xsl:apply-templates select="ServiceData/BezeqRecordData"/>
    </xsl:template>
    <xsl:template match="ServiceData/BezeqRecordData">
      <InvoiceDetails>
        <!--Connection from Source Node "BezeqRecordData" to Destination Node "InvoiceDetails"-->
        <xsl:value-of select="./text()"/>
        <PersonalDetailes>
          <xsl:for-each select="personalDetails">
            <Name>
              <!--Connection from Source Node "@name" to Destination Node "Name"-->
              <xsl:value-of select="@name"/>
            </Name>
          </xsl:for-each>
          <xsl:for-each select="personalDetails">
            <Attention>
              <!--Connection from Source Node "@attention" to Destination Node "Attention"-->
              <xsl:value-of select="@attention"/>
            </Attention>
          </xsl:for-each>
          <xsl:for-each select="personalDetails">
            <StreetNo>
              <!--Connection from Source Node "@streetNo" to Destination Node "StreetNo"-->
              <xsl:value-of select="@streetNo"/>
            </StreetNo>
          </xsl:for-each>
          <xsl:for-each select="personalDetails">
            <City>
              <!--Connection from Source Node "@city" to Destination Node "City"-->
              <xsl:value-of select="@city"/>
            </City>
          </xsl:for-each>
          <xsl:for-each select="personalDetails">
            <Zip>
              <!--Connection from Source Node "@ZIP" to Destination Node "Zip"-->
              <xsl:value-of select="@ZIP"/>
            </Zip>
          </xsl:for-each>
        </PersonalDetailes>
        <InvoiceDetailes>
          <xsl:for-each select="invoiceDetails">
            <InvoiceDate>
              <!--Connection from Source Node "@date" to Destination Node "InvoiceDate"-->
              <xsl:value-of select="@date"/>
            </InvoiceDate>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <FromBillingDate>
              <!--Connection from Source Node "@fromBillingDate" to Destination Node "FromBillingDate"-->
              <xsl:value-of select="@fromBillingDate"/>
            </FromBillingDate>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <ToBillingDate>
              <!--Connection from Source Node "@toBillingDate" to Destination Node "ToBillingDate"-->
              <xsl:value-of select="@toBillingDate"/>
            </ToBillingDate>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <InvoiceNo>
              <!--Connection from Source Node "@invoiceNo" to Destination Node "InvoiceNo"-->
              <xsl:value-of select="@invoiceNo"/>
            </InvoiceNo>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <xsl:for-each select="currentBills">
              <CurrentBills>
                <!--Connection from Source Node "currentBills" to Destination Node "CurrentBills"-->
                <xsl:value-of select="./text()"/>
                <xsl:for-each select="currentBill">
                  <CurrentBillsDetailes>
                    <!--Connection from Source Node "currentBill" to Destination Node "CurrentBillsDetailes"-->
                    <xsl:value-of select="./text()"/>
                    <CurrBillsDesc>
                      <!--Connection from Source Node "@description" to Destination Node "CurrBillsDesc"-->
                      <xsl:value-of select="ancestor::*[1]/@description"/>
                    </CurrBillsDesc>
                    <CurrBillQuantiy>
                      <!--Connection from Source Node "@quantity" to Destination Node "CurrBillQuantiy"-->
                      <xsl:value-of select="ancestor::*[1]/@quantity"/>
                    </CurrBillQuantiy>
                    <CurrBillDuration>
                      <!--Connection from Source Node "@duration" to Destination Node "CurrBillDuration"-->
                      <xsl:value-of select="ancestor::*[1]/@duration"/>
                    </CurrBillDuration>
                    <CurrBillAmmount>
                      <!--Connection from Source Node "@ammount" to Destination Node "CurrBillAmmount"-->
                      <xsl:value-of select="ancestor::*[1]/@ammount"/>
                    </CurrBillAmmount>
                  </CurrentBillsDetailes>
                </xsl:for-each>
              </CurrentBills>
            </xsl:for-each>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <xsl:for-each select="periodServiceDetails">
              <PeriodService>
                <!--Connection from Source Node "periodServiceDetails" to Destination Node "PeriodService"-->
                <xsl:value-of select="./text()"/>
                <xsl:for-each select="periodService">
                  <PeriodServiceDetailes>
                    <!--Connection from Source Node "periodService" to Destination Node "PeriodServiceDetailes"-->
                    <xsl:value-of select="./text()"/>
                    <PerServDesc>
                      <!--Connection from Source Node "@description" to Destination Node "PerServDesc"-->
                      <xsl:value-of select="ancestor::*[1]/@description"/>
                    </PerServDesc>
                    <PerServAmount>
                      <!--Connection from Source Node "@ammount" to Destination Node "PerServAmount"-->
                      <xsl:value-of select="ancestor::*[1]/@ammount"/>
                    </PerServAmount>
                  </PeriodServiceDetailes>
                </xsl:for-each>
              </PeriodService>
            </xsl:for-each>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <xsl:for-each select="creditDebitDetails">
              <CreditDebit>
                <!--Connection from Source Node "creditDebitDetails" to Destination Node "CreditDebit"-->
                <xsl:value-of select="./text()"/>
                <xsl:for-each select="creditDebit">
                  <CreditDebitDetailes>
                    <!--Connection from Source Node "creditDebit" to Destination Node "CreditDebitDetailes"-->
                    <xsl:value-of select="./text()"/>
                    <CreditDebitDesc>
                      <!--Connection from Source Node "@description" to Destination Node "CreditDebitDesc"-->
                      <xsl:value-of select="ancestor::*[1]/@description"/>
                    </CreditDebitDesc>
                    <CreditDebitAmount>
                      <!--Connection from Source Node "@ammount" to Destination Node "CreditDebitAmount"-->
                      <xsl:value-of select="ancestor::*[1]/@ammount"/>
                    </CreditDebitAmount>
                  </CreditDebitDetailes>
                </xsl:for-each>
              </CreditDebit>
            </xsl:for-each>
          </xsl:for-each>
        </InvoiceDetailes>
        <RegNumber>
          <!--Connection from Source Node "@companyID" to Destination Node "RegNumber"-->
          <xsl:value-of select="@companyID"/>
        </RegNumber>
        <NoVATTotalBills>
          <!--Connection from Source Node "@totalBeforeTax" to Destination Node "NoVATTotalBills"-->
          <xsl:value-of select="@totalBeforeTax"/>
        </NoVATTotalBills>
        <VatRate>
          <!--Connection from Source Node "@vatRate" to Destination Node "VatRate"-->
          <xsl:value-of select="@vatRate"/>
        </VatRate>
        <Vat>
          <!--Connection from Source Node "@VAT" to Destination Node "Vat"-->
          <xsl:value-of select="@VAT"/>
        </Vat>
        <ValidDate>
          <!--Connection from Source Node "@validDate" to Destination Node "ValidDate"-->
          <xsl:value-of select="@validDate"/>
        </ValidDate>
        <Rate>
          <!--Connection from Source Node "@rate" to Destination Node "Rate"-->
          <xsl:value-of select="@rate"/>
        </Rate>
        <xsl:for-each select="totalDesc">
          <TotalDesc>
            <!--Connection from Source Node "totalDesc" to Destination Node "TotalDesc"-->
            <xsl:value-of select="./text()"/>
            <Line1>
              <!--Connection from Source Node "@line1" to Destination Node "Line1"-->
              <xsl:value-of select="ancestor::*[1]/@line1"/>
            </Line1>
            <Line2>
              <!--Connection from Source Node "@line2" to Destination Node "Line2"-->
              <xsl:value-of select="ancestor::*[1]/@line2"/>
            </Line2>
            <Line3>
              <!--Connection from Source Node "@line3" to Destination Node "Line3"-->
              <xsl:value-of select="ancestor::*[1]/@line3"/>
            </Line3>
          </TotalDesc>
        </xsl:for-each>
        <Telephone>
          <!--Connection from Source Node "@telephone" to Destination Node "Telephone"-->
          <xsl:value-of select="@telephone"/>
        </Telephone>
        <Fax>
          <!--Connection from Source Node "@fax" to Destination Node "Fax"-->
          <xsl:value-of select="@fax"/>
        </Fax>
        <Messages>
          <xsl:for-each select="messages">
            <Msg1>
              <!--Connection from Source Node "@msg1" to Destination Node "Msg1"-->
              <xsl:value-of select="@msg1"/>
            </Msg1>
          </xsl:for-each>
          <xsl:for-each select="messages">
            <Msg2>
              <!--Connection from Source Node "@msg2" to Destination Node "Msg2"-->
              <xsl:value-of select="@msg2"/>
            </Msg2>
          </xsl:for-each>
        </Messages>
        <Internal>
          <xsl:for-each select="internal">
            <FilePage>
              <!--Connection from Source Node "@FilePage" to Destination Node "FilePage"-->
              <xsl:value-of select="@FilePage"/>
            </FilePage>
          </xsl:for-each>
          <xsl:for-each select="internal">
            <BBCUserId>
              <!--Connection from Source Node "@BCCUserID" to Destination Node "BBCUserId"-->
              <xsl:value-of select="@BCCUserID"/>
            </BBCUserId>
          </xsl:for-each>
          <xsl:for-each select="internal">
            <FileName>
              <!--Connection from Source Node "@fileName" to Destination Node "FileName"-->
              <xsl:value-of select="@fileName"/>
            </FileName>
          </xsl:for-each>
          <xsl:for-each select="internal">
            <Year>
              <!--Connection from Source Node "@year" to Destination Node "Year"-->
              <xsl:value-of select="@year"/>
            </Year>
          </xsl:for-each>
          <xsl:for-each select="internal">
            <Month>
              <!--Connection from Source Node "@month" to Destination Node "Month"-->
              <xsl:value-of select="@month"/>
            </Month>
          </xsl:for-each>
          <xsl:for-each select="invoiceDetails">
            <InUserid>
              <!--Connection from Source Node "@accountNo" to Destination Node "InUserid"-->
              <xsl:value-of select="@accountNo"/>
            </InUserid>
          </xsl:for-each>
        </Internal>
        <TotalPayment>
          <!--Connection from Source Node "@totalPayment" to Destination Node "TotalPayment"-->
          <xsl:value-of select="@totalPayment"/>
        </TotalPayment>
      </InvoiceDetails>
    </xsl:template>
    <msxsl:script language="VBScript" implements-prefix="user">
      <![CDATA[]]>
    </msxsl:script>
  </xsl:stylesheet>
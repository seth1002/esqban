<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
   <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
   <xsl:template match="/">
      <xsl:apply-templates select="ENTUSERS/XMLDATA/US_SERS"/>
   </xsl:template>
   <CATEGORY_SERVICES>
      <RESULTSCOUNT>
         <!-- xsl:variable name="var:v1" select="" /-->
         <!-- xsl:value-of select="$var:v1" /-->
      </RESULTSCOUNT>
      <xsl:template match="ENTUSERS/XMLDATA/US_SERS/US_SER">
         <SERVICE>
            <CUSTID>
               <xsl:value-of select="CID"/>
            </CUSTID>
            <CUST_NAME>
               <xsl:value-of select="CNAME"/>
            </CUST_NAME>
            <NODE_PARENT>
               <xsl:value-of select="DKEY"/>
            </NODE_PARENT>
            <SERV_DESC>
               <xsl:value-of select="SNAME"/>
            </SERV_DESC>
            <SERVICEID>
               <xsl:value-of select="SID"/>
            </SERVICEID>
            <COUNT>
               <xsl:value-of select="RDATA"/>
            </COUNT>
            <CATALOG>
               <xsl:value-of select="ISCAT"/>
            </CATALOG>
            <LAST_UPDATE>
               <xsl:value-of select="INDATE"/>
            </LAST_UPDATE>
         </SERVICE>
      </xsl:template>
   </CATEGORY_SERVICES>
</xsl:stylesheet>

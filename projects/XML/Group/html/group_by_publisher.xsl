<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
 <xsl:output method="html"/>
 <xsl:key name="services-by-publisher" match="SERVICE" use="PUBLISHER_ID" />
 <xsl:template match="SERVICES">
  <html>
   <head>
    <title>Services grouped by publisher</title>
   </head>
   <body>
    <xsl:for-each select="SERVICE[count(. | key('services-by-publisher', PUBLISHER_ID)[1]) = 1]">
     <xsl:sort select="SERVICE_ID" />
     <table border="1" cellpadding="10" width="80%">
      <tr align="center">
       <td><xsl:value-of select="PUBLISHER_ID" /></td>
      </tr>
      <xsl:for-each select="key('services-by-publisher', PUBLISHER_ID)">
       <xsl:sort select="PUBLISHER_ID" />
       <tr align="center">
        <td><xsl:value-of select="SERVICE_ID"/></td>
       </tr>
      </xsl:for-each>
     </table>
    </xsl:for-each>
   </body>
  </html>
 </xsl:template>
</xsl:stylesheet>


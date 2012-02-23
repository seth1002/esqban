<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>

  <xsl:template match="/">
    <html>
     <xsl:apply-templates/>
    </html>
  </xsl:template>

  <xsl:template match="headlines">
    <head>
      <title>headlines</title>
    </head>
    <body>
      <table border="0" cellpadding="2" width="80%">
        <xsl:for-each select="item">
          <tr align="left">
            <td><A target="viewFrame">
                <xsl:attribute name="HREF">
                  <xsl:value-of select="link/text()"/>
                </xsl:attribute>
                <xsl:value-of select="title/text()"/>
            </A></td>
          </tr>
        </xsl:for-each>
      </table>
    </body>
  </xsl:template>
</xsl:stylesheet>
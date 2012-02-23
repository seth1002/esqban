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
      <table border="1" cellpadding="10" width="80%">
        <tr align="center">
          <th>./text()</th><th>@name</th><th>@value</th>
        </tr>
        <xsl:for-each select="item">
          <tr align="center">
            <td><xsl:value-of select="date/text()"/></td>
            <td><A>
                <xsl:attribute name="HREF">
                  <xsl:value-of select="link/text()"/>
                </xsl:attribute>
                <xsl:value-of select="body/text()"/>
            </A></td>
            <td><xsl:value-of select="link/text()"/></td>
          </tr>
        </xsl:for-each>
      </table>
    </body>
  </xsl:template>
</xsl:stylesheet>
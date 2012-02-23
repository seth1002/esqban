<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>

  <xsl:template match="/">
    <html>
     <xsl:apply-templates/>
    </html>
  </xsl:template>

  <xsl:template match="items">
    <head>
      <title>Items</title>
    </head>
    <body>
      <table border="1" cellpadding="10" width="80%">
        <tr align="center">
          <th>./text()</th><th>@name</th><th>@value</th>
        </tr>
        <xsl:for-each select="item">
          <tr align="center">
            <td><xsl:value-of select="./text()"/></td>
            <td><xsl:value-of select="@name"/></td>
            <td><xsl:value-of select="@value"/></td>
          </tr>
        </xsl:for-each>
      </table>
    </body>
  </xsl:template>
</xsl:stylesheet>
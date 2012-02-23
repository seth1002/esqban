<?xml version="1.0" ?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:template match="/">
    <HTML>
      <BODY>
        <TABLE>
          <xsl:for-each select="customers/customer">
            <TR>
              <xsl:apply-templates select="name" />
              <xsl:apply-templates select="address" />
              <xsl:apply-templates select="phone" />
            </TR>
          </xsl:for-each>
        </TABLE>
      </BODY>
    </HTML>
  </xsl:template>
  <xsl:template match="name">
    <TD STYLE="font-size:14pt font-family:serif">
      <xsl:apply-templates />
    </TD>
  </xsl:template>
  <xsl:template match="address">
    <TD> <xsl:apply-templates /> </TD>
  </xsl:template>
  <xsl:template match="phone">
    <TD> <xsl:apply-templates /> </TD>
  </xsl:template>
  <xsl:template match="text()"><xsl:value-of /></xsl:template>
</xsl:stylesheet>

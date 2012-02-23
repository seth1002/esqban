<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
    <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
    <xsl:template match="/">
      <xsl:apply-templates select="ServiceData/TextDB"/>
    </xsl:template>
    <xsl:template match="ServiceData/TextDB">
      <User>
          <UserDetails>
            <!--Connection from Source Node "TextDB" to Destination Node "UserDetails"-->
            <xsl:for-each select="Col1">
              <UserId>
                <!--Connection from Source Node "Col1" to Destination Node "UserId"-->
                <xsl:value-of select="./text()"/>
              </UserId>
            </xsl:for-each>
            <xsl:for-each select="Col2">
              <Password>
                <!--Connection from Source Node "Col2" to Destination Node "Password"-->
                <xsl:value-of select="./text()"/>
              </Password>
            </xsl:for-each>
          </UserDetails>
      </User>
    </xsl:template>
    <msxsl:script language="VBScript" implements-prefix="user">
      <![CDATA[]]>
    </msxsl:script>
  </xsl:stylesheet>
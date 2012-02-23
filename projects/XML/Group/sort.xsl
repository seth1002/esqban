<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:m="mmm">
<xsl:output omit-xml-declaration="yes"/>
<xsl:key name="myKey" match="person/date" use="concat(../account, '::', .)"/>
	<m:month name="Jan" value="0"/>
	<m:month name="Feb" value="1"/>
	<m:month name="Mar" value="2"/>
	<m:month name="Apr" value="3"/>
	<m:month name="May" value="4"/>
	<m:month name="Jun" value="5"/>
	<m:month name="Jul" value="6"/>
	<m:month name="Aug" value="7"/>
	<m:month name="Sep" value="8"/>
	<m:month name="Oct" value="9"/>
	<m:month name="Nov" value="10"/>
	<m:month name="Dec" value="11"/>
	<xsl:template match="/">
	<xsl:variable name="months" select="document('')//m:month"/>
	<xsl:for-each select="/person/date[generate-id(.)=generate-id(key('myKey', concat(../account, '::', .))[1])]">
		<xsl:sort select="$months[ starts-with( current(),  @name ) ]/@value" data-type="number" order="descending"/>
		<xsl:copy-of select="."/>
		<xsl:text>&#xA;</xsl:text>
	</xsl:for-each>		
	</xsl:template>
</xsl:stylesheet>
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:var='urn:var' xmlns:user='urn:user' version='1.0'>
<xsl:output method='xml' indent='yes' omit-xml-declaration='yes' />
<xsl:template match='/'>
	<xsl:apply-templates select='Research'/>
</xsl:template>
<xsl:template match='Research'>
<ResearchInfo>
<xsl:for-each select='Language'>
	<Language>
		<!-- Connection from Source Node "Language" to Destination Node "Language" -->
		<xsl:value-of select='./text()'/>
	</Language>
</xsl:for-each>
<xsl:for-each select='ID'>
	<ResearchCode>
		<!-- Connection from Source Node "ID" to Destination Node "ResearchCode" -->
		<xsl:value-of select='./text()'/>
	</ResearchCode>
</xsl:for-each>
<xsl:for-each select='ContractSeq'>
	<ContractYear>
		<!-- Connection from Source Node "ContractSeq" to Destination Node "ContractYear" -->
		<xsl:value-of select='./text()'/>
	</ContractYear>
</xsl:for-each>
<xsl:for-each select='InfoType'>
	<InfoType>
		<!-- Connection from Source Node "InfoType" to Destination Node "InfoType" -->
		<xsl:value-of select='./text()'/>
	</InfoType>
</xsl:for-each>
<xsl:for-each select='Text'>
	<Text>
		<!-- Connection from Source Node "Text" to Destination Node "Text" -->
		<xsl:value-of select='./text()'/>
	</Text>
</xsl:for-each>
</ResearchInfo>
</xsl:template>
</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<!-- Entry point -->
	<xsl:template match="/">
		<asset>
			<xsl:apply-templates/>
		</asset>
	</xsl:template>
	
	<xsl:template match="html">
		<xsl:for-each select="/html/body/div[@id='wrapper']/div[@id='root']/layer[@name='root']/div[@id='tn15']/div[@id='tn15main']/div[@id='tn15content']/div[@class='filmo']/ol/li">
			<asset>
				<title><xsl:value-of select="./a/text()"/></title>
				<url>http://www.imdb.com<xsl:value-of select="./a/@href"/></url>
			</asset>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
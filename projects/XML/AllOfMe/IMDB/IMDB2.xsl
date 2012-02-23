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
		<Title><xsl:value-of select="./head/title/text()"/></Title>
		<xsl:for-each select="/html/body/div[@id='wrapper']/div[@id='root']/layer[@name='root']/div[@id='tn15']/div[@id='tn15main']/div[@id='tn15content']/div[@class='info']">
<!--
			<xsl:if test="./h5/text() = 'Director:'">
				<Director><xsl:value-of select="./a/text()"/></Director>
			</xsl:if>
			<xsl:if test="./h5/text() = 'Writer '">
				<Writer><xsl:value-of select="./a/text()"/></Writer>
			</xsl:if>
			<xsl:if test="./h5/text() = 'Release Date:'">
				<Date><xsl:value-of select="./text()"/></Date>
			</xsl:if>
-->			
			<xsl:choose>
				<xsl:when test="./h5/text() = 'Director:'">
					<Director><xsl:value-of select="./a/text()"/></Director>
				</xsl:when>
				<xsl:when test="./h5/text() = 'Writer '">
					<Writer><xsl:value-of select="./a/text()"/></Writer>
				</xsl:when>
				<xsl:when test="./h5/text() = 'Release Date:'">
					<Date><xsl:value-of select="./text()"/></Date>
				</xsl:when>
				<xsl:when test="./h5/text() = 'Genre:'">
					<Genre><xsl:value-of select="./a/text()"/></Genre>
				</xsl:when>
				<xsl:when test="./h5/text() = 'Tagline:'">
					<Tagline><xsl:value-of select="./text()"/></Tagline>
				</xsl:when>
				<xsl:when test="./h5/text() = 'Plot Outline:'">
					<Plot><xsl:value-of select="./text()"/></Plot>
				</xsl:when>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
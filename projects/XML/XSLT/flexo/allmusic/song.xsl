<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
<!--
	<xsl:include href="../main.xsl"/>
-->

	<xsl:variable name="class"	select="'song'"/>
	
	
	
	<xsl:template match="/">
		<items>
			<item>
				<xsl:attribute name="class">
					<xsl:value-of select="$class"/>
				</xsl:attribute>
				
				<xsl:variable name="val" select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='trackpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td/div[@id='results-table']/table/tr/td/table[@class='details-border']/tr/td/table/tr/td[@id='details']/table/tr[(preceding-sibling::tr/td[@class='title' and text()='Composed By'])[position()=last()]]/td[@class='content']"/>

				<xsl:call-template name="recursive-split">
					<xsl:with-param name="value"		select="$val"/>
					<xsl:with-param name="delimiter"	select="'/'"/>
					<xsl:with-param name="elementName"	select="'composer'"/>
				</xsl:call-template>
			</item>
		</items>
	</xsl:template>
	

	<!-- Recursive split -->
	<xsl:template name="recursive-split">
		<xsl:param name="value"/>
		<xsl:param name="delimiter"/>
		<xsl:param name="elementName"/>

		<xsl:variable name="val1" select="substring-before($value, $delimiter)"/>
		<xsl:variable name="val2" select="substring-after($value, $delimiter)"/>

		<xsl:element name="{$elementName}">
			<xsl:value-of select="$val1"/>
		</xsl:element>

		<xsl:choose>
			<xsl:when test="contains($val2, $delimiter)">
				<xsl:call-template name="recursive-split">
					<xsl:with-param name="value"		select="$val2"/>
					<xsl:with-param name="delimiter"	select="$delimiter"/>
					<xsl:with-param name="elementName"	select="$elementName"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:element name="{$elementName}">
					<xsl:value-of select="$val2"/>
				</xsl:element>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

<!--
Brian Johnson/Malcolm Young/Angus Young


html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='trackpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td/div[@id='results-table']/table/tr/td/table[@class='details-border']/tr/td/table/tr/td[@id='details']/table/tr[(preceding-sibling::tr/td[@class='title' and text()='Composed By'])[position()=last()]]/td[@class='content']
-->

</xsl:stylesheet>

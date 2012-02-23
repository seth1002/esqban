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
		<xsl:variable name="date"	select="table/tr/td[2]/table/tr/td[3]/table[2]/tr[2]/td/h3/text()"/>
		<xsl:variable name="year"	select="substring-after($date, ' ')"/>
		<xsl:variable name="month"	select="substring-before($date, ' ')"/>
		<xsl:variable name="artist"	select="table/tr/td[2]/table/tr/td[3]/table[2]/tr[2]/td/h3[@id='photographerHtmlGenericControl']/a/text()"/>
		<xsl:variable name="desc"	select="table/tr/td[2]/table/tr/td[3]/table[2]/tr[2]/td/h3[@id='modelHtmlGenericControl']/a/text()"/>
		<xsl:variable name="title"	select="table/tr/td[2]/table/tr/td[3]/table[2]/tr[2]/td/p/text()"/>
		<xsl:variable name="image"	select="table/tr/td[2]/table/tr/td/img/@src"/>
		<xsl:variable name="editor"	select="table/tr/td[2]/table/tr/td[3]/table[2]/tr[2]/td/h3[@id='editorHtmlGenericControl']/text()"/>

		<title><xsl:value-of		select="normalize-space($title)"/></title>
		<day/>
		<month><xsl:value-of		select="normalize-space($month)"/></month>
		<year><xsl:value-of			select="normalize-space($year)"/></year>
		<author/>
		<description><xsl:value-of	select="normalize-space($desc)"/></description>
		<image><xsl:value-of		select="normalize-space($image)"/></image>
		<artist><xsl:value-of		select="normalize-space($artist)"/></artist>
		<editor><xsl:value-of		select="normalize-space($editor)"/></editor>
	</xsl:template>
</xsl:stylesheet>
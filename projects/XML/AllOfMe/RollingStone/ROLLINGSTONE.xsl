<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<!-- Entry point -->
	<xsl:template match="/">
		<asset>
			<xsl:apply-templates/>
		</asset>
	</xsl:template>
<!--
	<xsl:template match="html">
		<xsl:variable name="title"	select="head/meta/@content"/>
		<xsl:variable name="date1"	select="substring-after($title, '(')"/>
		<xsl:variable name="date2"	select="substring-before($date1, ')')"/>
		<xsl:variable name="date"	select="substring-before($date2, ',')"/>
		<xsl:variable name="day"	select="substring-after($date, ' ')"/>
		<xsl:variable name="month"	select="substring-before($date, ' ')"/>
		<xsl:variable name="year"	select="substring-after($date2, ',')"/>
				
		<day><xsl:value-of select="$day"/></day>
		<month><xsl:value-of select="$month"/></month>
		<year><xsl:value-of select="$year"/></year>
		<description><xsl:value-of select="body/div[2]/div[3]/div[2]/div/div/div[3]/div/a/text()"/></description>
		<artist><xsl:value-of select="substring-after(body/div/div[2]/div[3]/div/div[7]/text(), ':')"/></artist>
		<image><xsl:value-of select="body/div[2]/div[3]/div[2]/div/div/div[2]/div/img/@src"/></image>
		<title><xsl:value-of select="head/meta/@content"/></title>
	</xsl:template>
-->
	<xsl:template match="html">
		<xsl:variable name="tmp"	select="head/meta/@content"/>
		<xsl:variable name="date1"	select="substring-after($tmp, '(')"/>
		<xsl:variable name="date2"	select="substring-before($date1, ')')"/>
		<xsl:variable name="date"	select="substring-before($date2, ',')"/>
		<xsl:variable name="day"	select="substring-after($date, ' ')"/>
		<xsl:variable name="month"	select="substring-before($date, ' ')"/>
		<xsl:variable name="year"	select="substring-after($date2, ',')"/>
		<xsl:variable name="title"	select="head/meta/@content"/>
		<xsl:variable name="desc"	select="body/div[2]/div[3]/div[2]/div/div/div[3]/div/a/text()"/>
		<xsl:variable name="image"	select="body/div[2]/div[3]/div[2]/div/div/div[2]/div/img/@src"/>
		<xsl:variable name="artist"	select="substring-after(body/div/div[2]/div[3]/div/div[7]/text(), ':')"/>
		
		<title><xsl:value-of		select="normalize-space($title)"/></title>
		<day><xsl:value-of			select="normalize-space($day)"/></day>
		<month><xsl:value-of		select="normalize-space($month)"/></month>
		<year><xsl:value-of			select="normalize-space($year)"/></year>
		<description><xsl:value-of	select="normalize-space($desc)"/></description>
		<image><xsl:value-of		select="normalize-space($image)"/></image>
		<artist><xsl:value-of		select="normalize-space($artist)"/></artist>
		<!--caption><xsl:value-of		select="body/div[@id='container']/div[@id='content']/div[@class='column-wideleft']/div[@id='photodisplay']/div[@class='top']/div[@class='imageCredit']/p/text()"/></caption-->
	</xsl:template>

</xsl:stylesheet>

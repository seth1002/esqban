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
		<xsl:variable name="date"	select="table/tr/td[2]/table/tr/td[2]/text()"/>
		<xsl:variable name="day"	select="substring-after($date, '.')"/>
		<xsl:variable name="month"	select="substring-before($date, '.')"/>
		<xsl:variable name="year"	select="table/tr/td[2]/table/tr/td[2]/a/text()"/>
		<xsl:variable name="title"	select="table/tr/td[2]/table/tr/td/h1/text()"/>
		<xsl:variable name="desc"	select="head/title/text()"/>
		<xsl:variable name="image"	select="table/tr[3]/td[2]/a/img/@src"/>
		<xsl:variable name="artist"	select="table/tr[3]/td[2]/div/text()"/>

		<title><xsl:value-of		select="normalize-space($title)"/></title>
		<day><xsl:value-of			select="normalize-space(substring-before($day, ','))"/></day>
		<month>
			<xsl:choose>
				<xsl:when test="$month='Jan'">January</xsl:when>
				<xsl:when test="$month='Feb'">February</xsl:when>
				<xsl:when test="$month='Mar'">March</xsl:when>
				<xsl:when test="$month='Apr'">April</xsl:when>
				<xsl:when test="$month='May'">May</xsl:when>
				<xsl:when test="$month='Jun'">June</xsl:when>
				<xsl:when test="$month='Jul'">July</xsl:when>
				<xsl:when test="$month='Aug'">August</xsl:when>
				<xsl:when test="$month='Sep'">September</xsl:when>
				<xsl:when test="$month='Oct'">October</xsl:when>
				<xsl:when test="$month='Nov'">November</xsl:when>
				<xsl:when test="$month='Dec'">December</xsl:when>
				<xsl:otherwise>Unknown</xsl:otherwise>
			</xsl:choose>
		</month>
		<year><xsl:value-of			select="normalize-space($year)"/></year>
		<description><xsl:value-of	select="normalize-space($desc)"/></description>
		<image><xsl:value-of		select="normalize-space($image)"/></image>
		<artist><xsl:value-of		select="normalize-space($artist)"/></artist>
	</xsl:template>
</xsl:stylesheet>
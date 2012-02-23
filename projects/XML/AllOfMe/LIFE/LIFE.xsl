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
		<xsl:variable name="date"	select="body/div/div[2]/div[3]/div/div[5]/strong/text()"/>
		<xsl:variable name="tmp"	select="substring-before($date, ',')"/>

		<xsl:variable name="title"	select="body/div[@id='wrap']/div[@id='maincontainer']/div[@id='midcolcover']/div[@id='wrapcover']/div[@id='mainimg']/following-sibling::div[2]/strong/text()"/>
		<xsl:variable name="day"	select="substring-after($tmp, ' ')"/>
		<xsl:variable name="month"	select="substring-before($tmp, ' ')"/>
		<xsl:variable name="year"	select="substring-after($date, ',')"/>
		<xsl:variable name="desc"	select="body/div[@id='wrap']/div[@id='maincontainer']/div[@id='midcolcover']/div[@id='wrapcover']/div[@id='mainimg']/following-sibling::div[2]/strong/text()"/>
		<xsl:variable name="image"	select="body/div[@id='wrap']/div[@id='maincontainer']/div[@id='midcolcover']/div[@id='wrapcover']/div[@id='mainimg']/img/@src"/>
		<xsl:variable name="artist"	select="normalize-space(substring-after(body/div[@id='wrap']/div[@id='maincontainer']/div[@id='midcolcover']/div[@id='wrapcover']/div[@id='mainimg']/following-sibling::div[3]/text(), ':'))"/>
	

		<title><xsl:value-of		select="normalize-space($title)"/></title>
		<day><xsl:value-of			select="normalize-space($day)"/></day>
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

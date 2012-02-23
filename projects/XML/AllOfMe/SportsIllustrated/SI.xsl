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
		<xsl:variable name="date"	select="normalize-space(div[@class='cnn6pxLRpad']/table/tr/td/div[@class='cnnCoversRight']/p[2]/b/text())"/>
		<xsl:variable name="year"	select="substring-after($date, ',')"/>
		<xsl:variable name="tmp1"	select="substring-before($date, ',')"/>
		<xsl:variable name="title"	select="div[@class='cnn6pxLRpad']/table/tr/td/div[@class='cnnCoversRight']/p[7]/text()"/>
		<xsl:variable name="day"	select="substring-after($tmp1, ' ')"/>
		<xsl:variable name="month"	select="substring-before($tmp1, ' ')"/>
		<xsl:variable name="author"	select="div[@class='cnn6pxLRpad']/table/tr/td/div[@class='cnnCoversRight']/p[9]/text()"/>
		<xsl:variable name="image"	select="div[@class='cnn6pxLRpad']/table/tr/td/div[@class='cnnCovers']/img/@src"/>
		<xsl:variable name="artist"	select="div[@class='cnn6pxLRpad']/table/tr/td/div[@class='cnnCoversRight']/p[5]/text()"/>
				
		<title><xsl:value-of		select="normalize-space($title)"/></title>
		<day><xsl:value-of			select="normalize-space($day)"/></day>
		<month><xsl:value-of		select="normalize-space($month)"/></month>
		<year><xsl:value-of			select="normalize-space($year)"/></year>
		<author><xsl:value-of		select="normalize-space($author)"/></author>
		<description><xsl:value-of	select="normalize-space($title)"/></description>
		<image><xsl:value-of		select="normalize-space($image)"/></image>
		<artist><xsl:value-of		select="normalize-space($artist)"/></artist>
	</xsl:template>
</xsl:stylesheet>

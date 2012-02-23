<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
<!--
	<xsl:include href="../main.xsl"/>
-->

	<xsl:variable name="class"	select="'album'"/>
	
	
	
	<xsl:template match="/">
		<items>
			<item class="album">
				<xsl:attribute name="class">
					<xsl:value-of select="$class"/>
				</xsl:attribute>
				<xsl:call-template name="left-sidebar">
					<xsl:with-param name="srcElementName"	select="'Album'"/>
					<xsl:with-param name="dstElementName"	select="'album'"/>
				</xsl:call-template>
				<xsl:call-template name="left-sidebar">
					<xsl:with-param name="srcElementName"	select="'Artist'"/>
					<xsl:with-param name="dstElementName"	select="'artist'"/>
				</xsl:call-template>
				<xsl:call-template name="left-sidebar">
					<xsl:with-param name="srcElementName"	select="'Release Date'"/>
					<xsl:with-param name="dstElementName"	select="'releaseDate'"/>
				</xsl:call-template>
				<xsl:call-template name="left-sidebar">
					<xsl:with-param name="srcElementName"	select="'Recording Date'"/>
					<xsl:with-param name="dstElementName"	select="'recordingDate'"/>
				</xsl:call-template>
				<xsl:call-template name="left-sidebar">
					<xsl:with-param name="srcElementName"	select="'Label'"/>
					<xsl:with-param name="dstElementName"	select="'label'"/>
					<xsl:with-param name="delimiter"		select="'/'"/>
				</xsl:call-template>
				<xsl:call-template name="left-sidebar">
					<xsl:with-param name="srcElementName"	select="'Time'"/>
					<xsl:with-param name="dstElementName"	select="'time'"/>
				</xsl:call-template>
				<xsl:call-template name="left-sidebar-list">
					<xsl:with-param name="index"			select="1"/>
					<xsl:with-param name="listName"			select="'genres'"/>
					<xsl:with-param name="dstElementName"	select="'genre'"/>
				</xsl:call-template>
				<xsl:call-template name="song-list"/>
			</item>
		</items>
	</xsl:template>


	<!-- Left sidebar single element -->
	<xsl:template name="left-sidebar">
		<xsl:param name="srcElementName"/>
		<xsl:param name="dstElementName"/>
		<xsl:param name="delimiter" value="''"/>

		<xsl:variable name="val" select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr/td/table/tr[(preceding-sibling::tr/td/div/span[text()=$srcElementName])[position()=last()]]/td[@class='sub-text']"/>

		<xsl:choose>
			<xsl:when test="$delimiter != ''">
				<xsl:variable name="tmp" select="concat($dstElementName, 's')"/>
				<xsl:element name="{$tmp}">
					<xsl:call-template name="recursive-split">
						<xsl:with-param name="value"		select="$val"/>
						<xsl:with-param name="delimiter"	select="$delimiter"/>
						<xsl:with-param name="elementName"	select="$dstElementName"/>
					</xsl:call-template>
				</xsl:element>
			</xsl:when>
			<xsl:otherwise>
				<xsl:element name="{$dstElementName}">
					<xsl:value-of select="$val"/>
				</xsl:element>
			</xsl:otherwise>
		</xsl:choose>

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


	<!-- Left sidebar list -->
	<xsl:template name="left-sidebar-list">
		<xsl:param name="index"/>
		<xsl:param name="listName"/>
		<xsl:param name="dstElementName"/>
		<xsl:element name="{$listName}">
			<xsl:for-each select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr/td[@class='artist']/table/tr[2]/td[@class='list-cell' and position()=$index]/div[@id='left-sidebar-list']/ul/li/a">
				<xsl:variable name="pos" select="position()"/>
				<!-- BUG: Position() and Last() XPath Functions Are Ignored When You Use Them as XSLT Function Arguments (Q324033) -->
				<xsl:if test="$pos=$index">
					<xsl:element name="{$dstElementName}">
						<xsl:value-of select="text()"/>
					</xsl:element>
				</xsl:if>
			</xsl:for-each>
		</xsl:element>
	</xsl:template>


	<!-- Song list -->
	<xsl:template name="song-list">
		<tracks>
			<xsl:for-each select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[2]/div[@id='results-table']/table/tr[2]/td/table[@id='ExpansionTable1']/tr[@class='visible']">
				<track>
					<index><xsl:value-of select="td[3]/text()"/></index>
					<title><xsl:value-of select="td[5]/a/text()"/></title>
					<composers>
						<xsl:call-template name="recursive-split">
							<xsl:with-param name="value"		select="td[6]/text()"/>
							<xsl:with-param name="delimiter"	select="','"/>
							<xsl:with-param name="elementName"	select="'composer'"/>
						</xsl:call-template>
					</composers>
					<time><xsl:value-of select="td[7]/text()"/></time>
				</track>
			</xsl:for-each>
		</tracks>
	</xsl:template>



<!--
	<xsl:template match="/">
		<items>
			<item class="movie">
				<xsl:attribute name="class">
					<xsl:value-of select="$class"/>
				</xsl:attribute>
				<xsl:call-template name="basic"/>
			</item>
		</items>
	</xsl:template>

	<xsl:template name="basic">
		<artist>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr/td/table/tr[(preceding-sibling::tr/td/div/span[text()='Artist'])[position()=last()]]/td[@class='sub-text']"/>
		</artist>
		<album>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr/td/table/tr[(preceding-sibling::tr/td/div/span[text()='Album'])[position()=last()]]/td[@class='sub-text']"/>
		</album>
		<releaseDate>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr/td/table/tr[(preceding-sibling::tr/td/div/span[text()='Release Date'])[position()=last()]]/td[@class='sub-text']"/>
		</releaseDate>
		<label>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr/td/table/tr[(preceding-sibling::tr/td/div/span[text()='Label'])[position()=last()]]/td[@class='sub-text']"/>
		</label>
		
		<title>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr/td[@class='titlebar']/table/tr/td[@class='titlebar']/span[@class='title']"/>
		</title>
		<band>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr/td[@class='titlebar']/table/tr/td[@class='titlebar']/a"/>
		</band>
		<date>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr[4]/td/table/tr[3]/td[@class='sub-text']"/>
		</date>
		<genre>
			<xsl:value-of select="html/body/div[@id='container']/div[@id='tabcontent_0']/div[@id='albumpage']/table[@class='wrapper']/tr/td[@class='content-cell']/table/tr[2]/td/table/tr/td[@class='left-sidebar']/table[2]/tr[6]/td/table/tr[3]/td[@class='sub-text']"/>
		</genre>
	</xsl:template>
-->


<!--
	<xsl:template match="/">
		<items>
			<item class="movie">
				<xsl:attribute name="class">
					<xsl:value-of select="$class"/>
				</xsl:attribute>
				<xsl:apply-templates select="html/body/div[@id='container']"/>
			</item>
		</items>
	</xsl:template>


	<xsl:template match="div[@id='container']">
		<xsl:apply-templates select="div[@id='tabcontent_0']"/>
	</xsl:template>

	<xsl:template name="div[@id='tabcontent_0']">
		<xsl:apply-templates select="div[@id='albumpage']"/>
	</xsl:template>

	<xsl:template name="div[@id='albumpage']">
		<xsl:apply-templates select="table[@class='wrapper']"/>
	</xsl:template>

	<xsl:template name="table[@class='wrapper']">
		<xsl:apply-templates select="tr/td[@class='content-cell']"/>
	</xsl:template>

	<xsl:template name="tr/td[@class='content-cell']">
		<xsl:apply-templates select=""/>
	</xsl:template>

	<xsl:template name="table/tr/td[@class='titlebar']">
		<a><xsl:value-of select="span[@class='title']"/></a>
		<xsl:apply-templates select="table/tr/td[@class='titlebar']"/>
	</xsl:template>

	<xsl:template name="td[@class='left-sidebar']">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template name="td[@class='sub-text']">
		<xsl:apply-templates/>
	</xsl:template>
-->


</xsl:stylesheet>

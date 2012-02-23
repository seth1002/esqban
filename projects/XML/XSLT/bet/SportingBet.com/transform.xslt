<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:include href="../main.xsl"/>

	<!--<xsl:variable name="sport" select="'Soccer'"/>-->
	<xsl:param name="sport" select="'Soccer'"/>

	<xsl:template match="/">
		<Bets source="sportingbet.com">
			<xsl:attribute name="timeStamp">
				<xsl:value-of select="$timeStamp"/>
			</xsl:attribute>
			<xsl:for-each select="/html/body/table[5]/tr/td[@width='640']/form[@name='sportnavform']/table">
				<!--<xsl:variable name="group"	select="tr[1]/td/span[@class='sportshead']"/>-->
				<xsl:for-each select="tr[@id]">
					<Bet>
						<xsl:variable name="marketname"	select="ancestor::table[1]/tr/td[@class='marketname']"/>
						<!--<xsl:variable name="group"		select="ancestor::table[1]/tr/td/table/tr/td[@class='title']"/>-->
						<xsl:choose>
							<xsl:when test="$marketname">
								<xsl:call-template name="Outright">
									<xsl:with-param name="id"			select="@id"/>
									<xsl:with-param name="sport"		select="$sport"/>
									<xsl:with-param name="marketname" 	select="$marketname"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="Match">
									<xsl:with-param name="id"		select="@id"/>
									<xsl:with-param name="sport"	select="$sport"/>
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</Bet>
				</xsl:for-each>
			</xsl:for-each>
		</Bets>
	</xsl:template>


	<xsl:template name="Match">
		<xsl:param name="id"/>
		<xsl:param name="sport"/>

		<xsl:variable name="group"	select = "ancestor::table[1]/tr/td/span"/>
		<xsl:variable name="match"	select = "td[1]/text()"/>
		<xsl:variable name="date"	select = "normalize-space(substring-before(td/span/text(), ' '))"/>
		<xsl:variable name="time"	select = "normalize-space(substring-after(td/span/text(), ' '))"/>

		<xsl:call-template name="BetAttributes">
			<xsl:with-param name="id"		select="$id"/>
			<xsl:with-param name="sport"	select="$sport"/>
			<xsl:with-param name="name"		select="$match"/>
			<xsl:with-param name="date"		select="$date"/>
			<xsl:with-param name="stops"	select="concat($time, ' ')"/>
			<xsl:with-param name="type"		select="'Match'"/>
		</xsl:call-template>
		

		<xsl:call-template name="CreateEvent">
			<xsl:with-param name="date"		select="$date"/>
			<xsl:with-param name="comp"		select="$group"/>
			<xsl:with-param name="part1"	select="normalize-space(substring-before($match, ' v '))"/>
			<xsl:with-param name="part2"	select="normalize-space(substring-after($match, ' v '))"/>
		</xsl:call-template>

		<BetOptions>
			<xsl:for-each select="td[a and input/@type='checkbox']">
				<xsl:variable name="name">
					<xsl:choose>
						<xsl:when test="position()=1">
							<xsl:value-of select="'1'"/>
						</xsl:when>
						<xsl:when test="position()=2">
							<xsl:value-of select="'X'"/>
						</xsl:when>
						<xsl:when test="position()=3">
							<xsl:value-of select="'2'"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:call-template name="Odd">
					<xsl:with-param name="name"		select="$name"/>
					<xsl:with-param name="val"		select="a/text()"/>
					<xsl:with-param name="href"		select="a/@href"/>
				</xsl:call-template>
			</xsl:for-each>
		</BetOptions>
	</xsl:template>

	<xsl:template name="Outright">
		<xsl:param name="id"/>
		<xsl:param name="sport"/>
		<xsl:param name="marketname"/>
		
		<xsl:variable name="name" select = "ancestor::table[1]/tr/td/table/tr/td[@class='title']"/>

		<xsl:call-template name="BetAttributes">
			<xsl:with-param name="id"		select="$id"/>
			<xsl:with-param name="sport"	select="$sport"/>
			<xsl:with-param name="name"		select="$name"/>
			<xsl:with-param name="type"		select="'Outright'"/>
			<xsl:with-param name="include"	select="$marketname"/>
		</xsl:call-template>

		<BetOptions>
			<xsl:for-each select="td/table/tr[td/span/@class='handicap' and td/a]">
				<xsl:call-template name="Odd">
					<xsl:with-param name="name" select="td[1]"/>
					<xsl:with-param name="val"	select="td/a/text()"/>
					<xsl:with-param name="href" select="td/a/@href"/>
					<xsl:with-param name="type" select="'WIN'"/>
				</xsl:call-template>
			</xsl:for-each>
		</BetOptions>
	</xsl:template>
</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:include href="../main.xsl"/>

	<xsl:template match="/">
		<Bets source="scandicbookmakers.com">
			<xsl:attribute name="timeStamp">
				<xsl:value-of select="$timeStamp"/>
			</xsl:attribute>
			<xsl:call-template name="threeway"/>
			<xsl:call-template name="outrights"/>
		</Bets>
	</xsl:template>

	<xsl:template name="threeway">
		<xsl:for-each select="/html/body/div[@id='pagewrapper']/div[@id='page']/div[@id='betting-text']/div[@id='content']/table[@id='main-table']/tr[@VALIGN='top' and td/font/b/text()!='Vinder']">
			<xsl:variable name="sport"	select="(preceding-sibling::tr[not(@VALIGN) and not(@bgcolor)])[position()=last()]/td[@colspan='5']/font/b"/>
			<xsl:variable name="type"	select="(preceding-sibling::tr[not(@VALIGN) and @bgcolor])[position()=last()]/td[@class='oddscol']/font/b"/>
			<xsl:variable name="match"	select = "td[2]/font/b"/>
			<xsl:variable name="date"	select = "substring(td[1]/font, 1, 8)"/>
			<xsl:variable name="time"	select = "substring(td[1]/font, 9, 5)"/>
			<xsl:if test="$type!='Vinder'">
				<Bet>
					<xsl:call-template name="BetAttributes">
						<xsl:with-param name="date"		select="$date"/>
						<xsl:with-param name="sport"	select="normalize-space(substring-before($sport, ' - '))"/>
						<xsl:with-param name="stops"	select="concat($time, ' ')"/>
						<xsl:with-param name="name"		select="$match"/>
						<xsl:with-param name="type"		select="'Match'"/>
					</xsl:call-template>

					<xsl:call-template name="CreateEvent">
						<xsl:with-param name="date"		select="$date"/>
						<xsl:with-param name="comp"		select="normalize-space(substring-after($sport, ' - '))"/>
						<xsl:with-param name="part1"	select="normalize-space(substring-before($match, ' - '))"/>
						<xsl:with-param name="part2"	select="normalize-space(substring-after($match, ' - '))"/>
					</xsl:call-template>

					<BetOptions>
						<xsl:call-template name="threewayOdds"/>
					</BetOptions>
				</Bet>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="threewayOdds">
		<xsl:call-template name="Odd">
			<xsl:with-param name="name"	select="'1'"/>
			<xsl:with-param name="val"	select="td[3]/font"/>
		</xsl:call-template>
		<xsl:call-template name="Odd">
			<xsl:with-param name="name"	select="'X'"/>
			<xsl:with-param name="val"	select="td[4]/font"/>
		</xsl:call-template>
		<xsl:call-template name="Odd">
			<xsl:with-param name="name"	select="'2'"/>
			<xsl:with-param name="val"	select="td[5]/font"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="outrights">
		<xsl:for-each select="/html/body/div[@id='pagewrapper']/div[@id='page']/div[@id='betting-text']/div[@id='content']/table[@id='main-table']/tr[td/font/b/text()='Vinder']">
			<xsl:variable name="sport"	select="preceding-sibling::tr[1]/td[@colspan='5']/font/b"/>
			<xsl:for-each select="preceding-sibling::tr[1]">
				<Bet>
					<xsl:variable name="tmp" select="(following-sibling::tr[@VALIGN='top'])[position()=1]"/>
					<xsl:call-template name="BetAttributes">
						<xsl:with-param name="date"		select="substring($tmp/td[1]/font, 1, 8)"/>
						<xsl:with-param name="sport"	select="normalize-space(substring-before($sport/text(), '-'))"/>
						<xsl:with-param name="stops"	select="substring($tmp/td[1]/font, 9, 5)"/>
						<xsl:with-param name="name"		select="normalize-space(substring-after($sport/text(), '-'))"/>
						<xsl:with-param name="type"		select="'Outright'"/>
					</xsl:call-template>
					<xsl:call-template name="outrightOdds">
						<xsl:with-param name="sport"	select="$sport"/>
					</xsl:call-template>
				</Bet>
			</xsl:for-each>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="outrightOdds">
		<xsl:param name="sport"/>
		<BetOptions>
			<xsl:for-each select="following-sibling::tr[@VALIGN='top']">
				<xsl:variable name="lsport"	select="(preceding-sibling::tr[not(@VALIGN) and not(@bgcolor)])[position() = last()]/td[@colspan='5']/font/b"/>
				<xsl:if test="$sport = $lsport">
					<xsl:call-template name="Odd">
						<xsl:with-param name="name"	select="td[@WIDTH='300']/font/b"/>
						<xsl:with-param name="val"	select="td[3]/font"/>
						<xsl:with-param name="type"	select="'WIN'"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:for-each>
		</BetOptions>
	</xsl:template>

</xsl:stylesheet>

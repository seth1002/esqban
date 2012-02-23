<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:include href="../main.xsl"/>

	<xsl:template match="/">
		<Bets source="unibet.com">
			<xsl:attribute name="timeStamp">
				<xsl:value-of select="$timeStamp"/>
			</xsl:attribute>
			<xsl:for-each select="/html/body/div[@id='mainpage']/html/body/table[@width='470']">
				<xsl:call-template name="eventTable"/>
			</xsl:for-each>
		</Bets>
	</xsl:template>

	<xsl:template name="eventTable">
		<xsl:variable name="type"	select="(preceding-sibling::span[@class='pagehead'])[position()=last()]"/>
		<xsl:variable name="date"	select="(preceding-sibling::span[@class='head'])[position()=last()]"/>
		<xsl:variable name="sport"	select="preceding-sibling::b[1]"/>

		<xsl:for-each select="tr[@valign='top' and not(@height)]">
			<Bet>
				<xsl:call-template name="BetAttributes">
					<xsl:with-param name="date"		select="$date"/>
					<xsl:with-param name="sport"	select="substring-before($sport, '-')"/>
					<xsl:with-param name="stops"	select="concat(td[1], ':00')"/>
					<xsl:with-param name="id"		select="td[3]/i[1]"/>
					<xsl:with-param name="name"		select="td[3]/b"/>
					<xsl:with-param name="include"	select="td[3]/i[2]"/>
					<xsl:with-param name="text"		select="td[3]/text()"/>
					<xsl:with-param name="type"		select="$type"/>
				</xsl:call-template>

				<xsl:choose>
					<xsl:when test="$type='Over/Under'">
						<xsl:call-template name="overUnder"/>
					</xsl:when>
					<xsl:when test="$type='Handicap'">
						<xsl:call-template name="overUnder"/>
					</xsl:when>
					<xsl:when test="$type='Head to Head'">
						<xsl:call-template name="overUnder"/>
					</xsl:when>
					<xsl:when test="$type='Correct Score'">
						<xsl:call-template name="correctScore"/>
					</xsl:when>
					<xsl:when test="$type='Outright'">
						<xsl:call-template name="outright"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="match">
							<xsl:with-param name="date"		select="$date"/>
							<xsl:with-param name="sport"	select="$sport"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</Bet>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="match">
		<xsl:param name="date"/>
		<xsl:param name="sport"/>
		<xsl:variable name="match"	select = "td[3]/b"/>

		<xsl:call-template name="CreateEvent">
			<xsl:with-param name="date"		select="$date"/>
			<xsl:with-param name="comp"		select="substring-after($sport, '-')"/>
			<xsl:with-param name="part1"	select="normalize-space(substring-before($match, ' - '))"/>
			<xsl:with-param name="part2"	select="normalize-space(substring-after($match, ' - '))"/>
		</xsl:call-template>

		<BetOptions>
			<xsl:if test="td[5]/a">
				<xsl:call-template name="Odd">
					<xsl:with-param name="name"	select="'1'"/>
					<xsl:with-param name="val"	select="td[5]/a"/>
					<xsl:with-param name="href"	select="td[5]/a/@href"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:if test="td[7]/a">
				<xsl:call-template name="Odd">
					<xsl:with-param name="name"	select="'X'"/>
					<xsl:with-param name="val"	select="td[7]/a"/>
					<xsl:with-param name="href"	select="td[7]/a/@href"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:if test="td[9]/a">
				<xsl:call-template name="Odd">
					<xsl:with-param name="name"	select="'2'"/>
					<xsl:with-param name="val"	select="td[9]/a"/>
					<xsl:with-param name="href"	select="td[9]/a/@href"/>
				</xsl:call-template>
			</xsl:if>
		</BetOptions>
	</xsl:template>

	<xsl:template name="overUnder">
		<BetOptions>
			<xsl:for-each select="td[5]/table/tr">
				<xsl:call-template name="Odd">
					<xsl:with-param name="name"	select="td[1]/nobr"/>
					<xsl:with-param name="val"	select="td[2]/a"/>
					<xsl:with-param name="href"	select="td[2]/a/@href"/>
				</xsl:call-template>
			</xsl:for-each>
		</BetOptions>
	</xsl:template>

	<xsl:template name="correctScore">
		<BetOptions>
			<xsl:for-each select="td[5]/table/tr">
				<xsl:call-template name="Odd">
					<xsl:with-param name="name"	select="td[1]"/>
					<xsl:with-param name="val"	select="td[2]/a"/>
					<xsl:with-param name="href"	select="td[2]/a/@href"/>
				</xsl:call-template>
			</xsl:for-each>
		</BetOptions>
	</xsl:template>

	<xsl:template name="outright">
		<BetOptions>
			<xsl:for-each select="following-sibling::tr[1]/td/table/tr[position()>1 and td[3]/a]">
				<BetOption>
					<xsl:attribute name="name">
						<xsl:value-of select="td[2]"/>
					</xsl:attribute>

					<Win>
						<xsl:call-template name="OddAttributes">
							<xsl:with-param name="val"	select="td[3]/a"/>
							<xsl:with-param name="href"	select="td[3]/a/@href"/>
							<xsl:with-param name="prev"	select="td[4]"/>
						</xsl:call-template>
					</Win>

					<xsl:if test="td[5]/a">
						<Placed>
							<xsl:variable name="placed"	select="ancestor::*[1]/tr/td/table/tr/td/nobr/b"/>
							<xsl:call-template name="OddAttributes">
								<xsl:with-param name="name"	select="$placed"/>
								<xsl:with-param name="val"	select="td[5]/a"/>
								<xsl:with-param name="href"	select="td[5]/a/@href"/>
							</xsl:call-template>
						</Placed>
					</xsl:if>
				</BetOption>
			</xsl:for-each>
		</BetOptions>
	</xsl:template>

</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:include href="../main.xsl"/>

	<xsl:template match="/">
		<Bets source="expekt.com">
			<xsl:attribute name="timeStamp">
				<xsl:value-of select="$timeStamp"/>
			</xsl:attribute>
		
			<xsl:for-each select="/html/body/table[@cellspacing='2' and @cellpadding='3' and @hspace='0' and @vspace='0' and @width='95%']/tr[@class='trs0']">
				<xsl:variable name="date"			select = "(preceding-sibling::tr[@align='left' and @class='t']/td[@colspan='7'])[position()=last()]/b"/>
				<xsl:variable name="time"			select = "td[1]"/>
				<xsl:variable name="event0"			select = "td[2]"/>
				<xsl:variable name="competition"	select = "td[3]"/>
				<xsl:variable name="one"			select = "td[4]"/>
				<xsl:variable name="x"				select = "td[5]"/>
				<xsl:variable name="two"			select = "td[6]"/>
				<xsl:variable name="tmp"			select = "normalize-space(substring-after($event0, '-'))"/>
				<xsl:variable name="text"			select = "normalize-space(substring-after($tmp, '-'))"/>

				<Bet>
					<xsl:call-template name="BetAttributes">
						<xsl:with-param name="date"		select="$date"/>
						<xsl:with-param name="sport"	select="MISSING"/>
						<xsl:with-param name="stops"	select="$time"/>
						<xsl:with-param name="name"		select="$text"/>
						<xsl:with-param name="type"		select="'Match'"/>
					</xsl:call-template>

					<xsl:call-template name="CreateEvent">
						<xsl:with-param name="date"		select="$date"/>
						<xsl:with-param name="comp"		select="$competition"/>
						<xsl:with-param name="part1"	select="normalize-space(substring-before($event0, '-'))"/>
						<xsl:with-param name="part2"	select="normalize-space(substring-before($tmp, '-'))"/>
					</xsl:call-template>
					
					<BetOptions>
						<xsl:call-template name="Odd">
							<xsl:with-param name="name"	select="1"/>
							<xsl:with-param name="val"	select="$one"/>
							<xsl:with-param name="href"	select="MISSING"/>
							<xsl:with-param name="prev"	select="MISSING"/>
						</xsl:call-template>
						<xsl:call-template name="Odd">
							<xsl:with-param name="name"	select="X"/>
							<xsl:with-param name="val"	select="$x"/>
							<xsl:with-param name="href"	select="MISSING"/>
							<xsl:with-param name="prev"	select="MISSING"/>
						</xsl:call-template>
						<xsl:call-template name="Odd">
							<xsl:with-param name="name"	select="2"/>
							<xsl:with-param name="val"	select="$two"/>
							<xsl:with-param name="href"	select="MISSING"/>
							<xsl:with-param name="prev"	select="MISSING"/>
						</xsl:call-template>
					</BetOptions>
				</Bet>
			</xsl:for-each>
		</Bets>
	</xsl:template>

</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<!--<xsl:variable name="timeStamp" select="current-dateTime()"/>-->
	<!--<xsl:variable name="timeStamp" select="'MISSING!'"/>-->
	<xsl:param name="timeStamp" select="'1800-01-01T0000:00:00'"/>
		
	<xsl:template name="Odd">
		<xsl:param name="name"/>
		<xsl:param name="val"/>
		<xsl:param name="href"/>
		<xsl:param name="type"/>
		
		<BetOption>
			<xsl:attribute name="name">
				<xsl:value-of select="$name"/>
			</xsl:attribute>
			
			<BetOptionOdd>
				<!--<xsl:attribute name="timeStamp">
					<xsl:value-of select="$timeStamp"/>
				</xsl:attribute>-->

				<xsl:choose>
					<xsl:when test="$type='WIN'">
						<Win>
							<xsl:call-template name="OddAttributes">
								<!--<xsl:with-param name="name"	select="$name"/>-->
								<xsl:with-param name="val"	select="$val"/>
								<xsl:with-param name="href"	select="$href"/>
							</xsl:call-template>
						</Win>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="OddAttributes">
							<!--<xsl:with-param name="name"	select="$name"/>-->
							<xsl:with-param name="val"	select="$val"/>
							<xsl:with-param name="href"	select="$href"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</BetOptionOdd>
		</BetOption>
	</xsl:template>

	<xsl:template name="OddAttributes">
		<xsl:param name="name"/>
		<xsl:param name="val" 	select="'MISSING!'"/>
		<xsl:param name="href"	select="'MISSING!'"/>
		<xsl:param name="prev"/>

		<!--<xsl:if test="$name">
			<xsl:attribute name="name">
				<xsl:value-of select="$name"/>
			</xsl:attribute>
		</xsl:if>-->

		<xsl:attribute name="odd">
			<xsl:value-of select="$val"/>
		</xsl:attribute>

		<xsl:attribute name="href">
			<xsl:value-of select="$href"/>
		</xsl:attribute>

		<xsl:if test="$prev">
			<xsl:attribute name="prev">
				<xsl:value-of select="$prev"/>
			</xsl:attribute>
		</xsl:if>

	</xsl:template>


	<xsl:template name="BetAttributes">
		<xsl:param name="date"		select="'MISSING!'"/>
		<xsl:param name="sport"		select="'MISSING!'"/>
		<xsl:param name="stops"		select="'MISSING!'"/>
		<xsl:param name="id"		select="'MISSING!'"/>
		<xsl:param name="name"		select="'MISSING!'"/>
		<xsl:param name="type"		select="'MISSING!'"/>
		<xsl:param name="include"/>
		<xsl:param name="text"/>

		<xsl:attribute name="type">
			<xsl:value-of select="$type"/>
		</xsl:attribute>

		<xsl:attribute name="date">
			<xsl:value-of select="$date"/>
		</xsl:attribute>
		
		<xsl:attribute name="stops">
			<xsl:value-of select="$stops"/>
		</xsl:attribute>
<!--
		<xsl:attribute name="sport">
			<xsl:value-of select="$sport"/>
		</xsl:attribute>
		
		<xsl:attribute name="id">
			<xsl:value-of select="$id"/>
		</xsl:attribute>
		
		<xsl:attribute name="name">
			<xsl:value-of select="$name"/>
		</xsl:attribute>

		<xsl:if test="$include">
			<xsl:attribute name="include">
				<xsl:value-of select="$include"/>
			</xsl:attribute>
		</xsl:if>
		
		<xsl:if test="$text">
			<xsl:attribute name="text">
				<xsl:value-of select="$text"/>
			</xsl:attribute>
		</xsl:if>
-->
	</xsl:template>

	<xsl:template name="CreateEvent">
		<xsl:param name="date"	select="'MISSING!'"/>
		<xsl:param name="comp"	select="'MISSING!'"/>
		<xsl:param name="part1"	select="'MISSING!'"/>
		<xsl:param name="part2"/>
		<Event>
			<xsl:attribute name="date">
				<xsl:value-of select="$date"/>
			</xsl:attribute>
			<xsl:attribute name="competitionName">
				<xsl:value-of select="$comp"/>
			</xsl:attribute>
			<Participants>
				<Participant>
					<xsl:attribute name="name">
						<xsl:value-of select="$part1"/>
					</xsl:attribute>
				</Participant>
				<xsl:if test="$part2">
					<Participant>
						<xsl:attribute name="name">
							<xsl:value-of select="$part2"/>
						</xsl:attribute>
					</Participant>
				</xsl:if>
			</Participants>
		</Event>
	</xsl:template>

	<!--<xsl:template name="MatchParticipants">
		<xsl:param name="part1" select="'MISSING!'"/>
		<xsl:param name="part2"/>
		<Participants>
			<Participant>
				<xsl:attribute name="name">
					<xsl:value-of select="$part1"/>
				</xsl:attribute>
			</Participant>
			<xsl:if test="$part2">
				<Participant>
					<xsl:attribute name="name">
						<xsl:value-of select="$part2"/>
					</xsl:attribute>
				</Participant>
			</xsl:if>
		</Participants>
	</xsl:template>-->

</xsl:stylesheet>
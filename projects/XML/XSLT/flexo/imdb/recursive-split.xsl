<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


	<!-- Recursive split -->
	<xsl:template name="recursive-split">
		<xsl:param name="value"/>
		<xsl:param name="delimiter"/>
		<xsl:param name="delimiter2" value=""/>
		<xsl:param name="templateName"/>
		<xsl:param name="templateParam1" value=""/>
		<xsl:param name="templateParam2" value=""/>
		<xsl:param name="index">0</xsl:param>


		<xsl:choose>
			<xsl:when test="contains($value, $delimiter)">
				<xsl:variable name="val1" select="substring-before($value, $delimiter)"/>
				<xsl:variable name="val2" select="substring-after($value, $delimiter)"/>
		
				<xsl:apply-templates select="$templateName">
					<xsl:with-param name="param"  select="$val1"/>
					<xsl:with-param name="param1" select="$templateParam1"/>
					<xsl:with-param name="param2" select="$templateParam2"/>
					<xsl:with-param name="index"  select="$index"/>
				</xsl:apply-templates>
		
				<xsl:choose>
					<xsl:when test="contains($val2, $delimiter)">
						<xsl:call-template name="recursive-split">
							<xsl:with-param name="value"			select="$val2"/>
							<xsl:with-param name="delimiter"		select="$delimiter"/>
							<xsl:with-param name="templateName"		select="$templateName"/>
							<xsl:with-param name="templateParam1"	select="$templateParam1"/>
							<xsl:with-param name="templateParam2"	select="$templateParam2"/>
							<xsl:with-param name="index"			select="$index+1"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="$templateName">
							<xsl:with-param name="param"  select="$val2"/>
							<xsl:with-param name="param1" select="$templateParam1"/>
							<xsl:with-param name="param2" select="$templateParam2"/>
							<xsl:with-param name="index"  select="$index+1"/>
						</xsl:apply-templates>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="contains($value, $delimiter2)">
						<xsl:call-template name="recursive-split">
							<xsl:with-param name="value"			select="$value"/>
							<xsl:with-param name="delimiter"		select="$delimiter2"/>
							<xsl:with-param name="templateName"		select="$templateName"/>
							<xsl:with-param name="templateParam1"	select="$templateParam1"/>
							<xsl:with-param name="templateParam2"	select="$templateParam2"/>
							<xsl:with-param name="index"			select="$index+1"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="$templateName">
							<xsl:with-param name="param"  select="$value"/>
							<xsl:with-param name="param1" select="$templateParam1"/>
							<xsl:with-param name="param2" select="$templateParam2"/>
							<xsl:with-param name="index"  select="$index+1"/>
						</xsl:apply-templates>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

</xsl:stylesheet>

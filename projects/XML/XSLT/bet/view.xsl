<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html"/>
	
	<xsl:template match="/">
		<html>
			<xsl:apply-templates/>
		</html>
	</xsl:template>
	
	<xsl:template match="Bets">
		<head>
			<title>Bets</title>
		</head>
		<body>
			<table border="0" cellpadding="0" width="100%">
				<tr align="center">
					<th width="70">Date</th><th>Stops</th><th>Name</th><th>1</th><th>X</th><th>2</th>
				</tr>
				<xsl:for-each select="Bet">
					<xsl:variable name="type" select="@type"/>
					<tr align="left" bgcolor="#CCCCCC">
						<xsl:if test="position() mod 2 != 0">
							<xsl:attribute name="bgcolor">#EEEEEE</xsl:attribute>
						</xsl:if>
						<td><xsl:value-of select="@date"/></td>
						<td><xsl:value-of select="@stops"/></td>
						<td><xsl:value-of select="@name"/></td>
						<xsl:variable name="odds" select="Odds/Odd"/>
						<xsl:for-each select="$odds">
							<xsl:if test="count($odds)=2 and position()=2">
								<td/>
							</xsl:if>
							<td><xsl:value-of select="@val"/></td>
						</xsl:for-each>
					</tr>
				</xsl:for-each>
			</table>
		</body>
	</xsl:template>

</xsl:stylesheet>
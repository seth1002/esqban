<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


	<xsl:template match="/">
		<info>
			<version>
				<xsl:value-of select="system-property('xsl:version')"/>
			</version>
			<vendor>
				<xsl:value-of select="system-property('xsl:vendor')"/>
			</vendor>
			<vendor-url>
				<xsl:value-of select="system-property('xsl:vendor-url')"/>
			</vendor-url>
		</info>
	</xsl:template>
</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
		<!--xsl:template match="/">
   			<xsl:copy-of select="." />
		</xsl:template-->
		<xsl:template match="*[not(namespace-uri())] | xmlns:*">
			<xsl:element name="{local-name(.)}">
				<xsl:apply-templates/>
			</xsl:element>
		</xsl:template>
    </xsl:output>
</xsl:stylesheet>
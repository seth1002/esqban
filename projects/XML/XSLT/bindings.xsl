<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"><xsl:key name="services-by-user" match="USER" use="@id"/><xsl:template match="ROOT"><AUTHORIZEDUSERS><xsl:for-each select="USER[count(. | key('services-by-user', @id)[1]) = 1]"><xsl:sort select="SERVICE/@id"/><USER><xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute><xsl:attribute name="firstName"><xsl:value-of select="@firstName"/></xsl:attribute><xsl:attribute name="lastName"><xsl:value-of select="@lastName"/></xsl:attribute><STATUS>ORIGINAL</STATUS><SERVICES><xsl:for-each select="key('services-by-user', @id)"><xsl:sort select="@id"/><SERVICE><xsl:attribute name="id"><xsl:value-of select="SERVICE/@id"/></xsl:attribute><xsl:attribute name="name"><xsl:value-of select="SERVICE/@name"/></xsl:attribute></SERVICE></xsl:for-each></SERVICES></USER></xsl:for-each></AUTHORIZEDUSERS></xsl:template></xsl:stylesheet>

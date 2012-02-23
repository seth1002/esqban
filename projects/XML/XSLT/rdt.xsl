<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:var='urn:var' xmlns:user='urn:user' version='1.0'>
<xsl:output method='xml' indent='yes' omit-xml-declaration='yes' />
<xsl:template match='/'>
	<xsl:apply-templates select='BOData/EntityData/TableData/TableRecord'/>
</xsl:template>
<xsl:template match='BOData/EntityData/TableData/TableRecord'>
<RDT_StockInfo>
<xsl:for-each select='Product_Id'>
	<ProductID>
		<!-- Connection from Source Node "Product_Id" to Destination Node "ProductID" -->
		<xsl:value-of select='./text()'/>
	</ProductID>
</xsl:for-each>
<xsl:for-each select='Free_Stock_Qty'>
	<FreeStockQTY>
		<!-- Connection from Source Node "Free_Stock_Qty" to Destination Node "FreeStockQTY" -->
		<xsl:value-of select='./text()'/>
	</FreeStockQTY>
</xsl:for-each>
<xsl:for-each select='Manufacturer_Name_Eng'>
	<Manufacturer>
		<!-- Connection from Source Node "Manufacturer_Name_Eng" to Destination Node "Manufacturer" -->
		<xsl:value-of select='./text()'/>
	</Manufacturer>
</xsl:for-each>
</RDT_StockInfo>
</xsl:template>
</xsl:stylesheet>
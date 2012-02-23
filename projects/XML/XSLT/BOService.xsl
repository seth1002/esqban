<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' 
		xmlns:msxsl='urn:schemas-microsoft-com:xslt' 
		xmlns:var='urn:var' 
		xmlns:user='urn:user' 
		xmlns:DM='http://data-mall.com/dmxsl'
		version='1.0'>
	<xsl:output method='xml' indent='yes' omit-xml-declaration='yes' />
		<xsl:template match='/'>
		<xsl:apply-templates select='ServiceData'/>
	</xsl:template>
	<xsl:template match='ServiceData'>
		<TB_CATALOG>
			<xsl:for-each select='Payment'>
				<SERVICEID></SERVICEID>
				<TRANSFER_IDENTIFIER></TRANSFER_IDENTIFIER>
				<DATA_STATUS></DATA_STATUS>
				<STATUS></STATUS>
				<CATEGORY_1></CATEGORY_1>
				<CATEGORY_2></CATEGORY_2>
				<CATEGORY_3></CATEGORY_3>
				<CATEGORY_4></CATEGORY_4>
				<CATEGORY_5></CATEGORY_5>
				<CATEGORY_6></CATEGORY_6>
				<CATEGORY_7></CATEGORY_7>
				<CATEGORY_8></CATEGORY_8>
				<CATEGORY_9></CATEGORY_9>
				<CATEGORY_10></CATEGORY_10>
				<INDEX_1></INDEX_1>
				<INDEX_2></INDEX_2>
				<INDEX_3></INDEX_3>
				<INDEX_4></INDEX_4>
				<INDEX_5></INDEX_5>
				<INDEX_6></INDEX_6>
				<INDEX_7></INDEX_7>
				<INDEX_8></INDEX_8>
				<INDEX_9></INDEX_9>
				<INDEX_10></INDEX_10>
				<CUSTID></CUSTID>
				<UNIQUE_KEY></UNIQUE_KEY>
				<CATALOG_ITEM><xsl:value-of select='DM:GetXML(.)'/></CATALOG_ITEM>
			</xsl:for-each>
		</TB_CATALOG>
	</xsl:template>
	<msxsl:script language='VBScript' implements-prefix='DM'>
    		function GetXML(Nodelist)
			if not Nodelist is nothing then
				GetXML=nodelist.Item(0).xml
			else
				GetXML=""
			end if
    		End Function
  	</msxsl:script>
</xsl:stylesheet>
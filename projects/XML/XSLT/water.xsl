<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" xmlns:var="urn:var" xmlns:user="urn:user" version="1.0">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
	<xsl:template match="/">
		<xsl:apply-templates select="ServiceData/MasterSelect"/>
	</xsl:template>
	<xsl:template match="ServiceData/MasterSelect">
		<WaterBill>
			<UserKey>
				<!--Connection from Source Node "@Orders.OrderID" to Destination Node "UserKey"-->
				<xsl:value-of select="@Orders.OrderID"/>
			</UserKey>
			<Period>
				<!--Connection from Source Node "@Orders.CustomerID" to Destination Node "Period"-->
				<xsl:value-of select="@Orders.CustomerID"/>
			</Period>
			<Amount>
				<!--Connection from Source Node "@Orders.EmployeeID" to Destination Node "Amount"-->
				<xsl:value-of select="@Orders.EmployeeID"/>
			</Amount>
			<Sum>
				<!--Connection from Source Node "@Orders.OrderDate" to Destination Node "Sum"-->
				<xsl:value-of select="@Orders.OrderDate"/>
			</Sum>
			<DateDue>
				<!--Connection from Source Node "@Orders.RequiredDate" to Destination Node "DateDue"-->
				<xsl:value-of select="@Orders.RequiredDate"/>
			</DateDue>
			<Remark>
				<xsl:variable name="var:v1" select="user:zenize(string(@Orders.ShippedDate))"/>
				<xsl:value-of select="$var:v1"/>
			</Remark>
		</WaterBill>
		<msxsl:script language="VBScript" implements-prefix="user">
			<![CDATA[function zenize(param1)
  zenize=param1 & "_Zen"
end function


]]>
		</msxsl:script>
	</xsl:template>
</xsl:stylesheet>

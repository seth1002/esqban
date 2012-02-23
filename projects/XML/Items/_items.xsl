<?xml version='1.0' ?>
<!--  Generated using sqba's Java XML Mapper -->
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:var='urn:var' xmlns:user='urn:user' version='1.0'>
<xsl:output method='xml' indent='yes' omit-xml-declaration='yes'/>
<xsl:template match='/'>
 <xsl:apply-templates/>
</xsl:template>
<xsl:template match='/items'>
 <html>
  <head>
   <title>
   </title>
  </head>
  <body>
   <table border='' cellpadding='' width=''>
    <tr align=''>
     <th>
     </th>
     <th>
     </th>
     <th>
     </th>
    </tr>
    <xsl:for-each select='item'>
     <tr align=''>
      <td>
       <xsl:value-of select='text()'/>
      </td>
      <td>
       <xsl:value-of select='@name'/>
      </td>
      <td>
       <xsl:value-of select='@value'/>
      </td>
     </tr>
    </xsl:for-each>
   </table>
  </body>
 </html>
</xsl:template>
</xsl:stylesheet>

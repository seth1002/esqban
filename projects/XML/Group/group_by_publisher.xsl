<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
 <xsl:output method="xml"/>
 <xsl:output omit-xml-declaration="no"/>
 <xsl:output version="1.0"/>
 <xsl:output standalone="yes"/>
 <xsl:output media-type="text/xml"/>
 <xsl:key name="services-by-publisher" match="SERVICE" use="PUBLISHER_ID" />
 <xsl:template match="SERVICES">
  <PUBLISHERS>
   <xsl:for-each select="SERVICE[count(. | key('services-by-publisher', PUBLISHER_ID)[1]) = 1]">
    <xsl:sort select="SERVICE_ID" />
    <PUBLISHER>
     <PUBLISHER_ID><xsl:value-of select="PUBLISHER_ID" /></PUBLISHER_ID>
     <SERVICES>
      <xsl:for-each select="key('services-by-publisher', PUBLISHER_ID)">
       <xsl:sort select="PUBLISHER_ID" />
       <SERVICE>
        <SERVICE_ID><xsl:value-of select="SERVICE_ID"/></SERVICE_ID>
       </SERVICE>
      </xsl:for-each>
     </SERVICES>
    </PUBLISHER>
   </xsl:for-each>
  </PUBLISHERS>
 </xsl:template>
</xsl:stylesheet>


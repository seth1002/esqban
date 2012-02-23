<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method='xml' indent='yes' omit-xml-declaration='yes'/>

  <xsl:template match="/">
    <xsl:variable name='type'/>
    <xsl:variable name='date'/>
    <xsl:variable name='sport'/>

    <Events>

     <xsl:for-each select='/html/body/div/html/body/span[@class='pagehead']'>

      <xsl:if test=".='span'">
       <xsl:choose>
        <xsl:when test="@class='pagehead'">
        </xsl:when>
        <xsl:when test='@class='head''>
        </xsl:when>
       </xsl:choose>
      </xsl:if>
 
     </xsl:for-each>

    </Events>

  </xsl:template>

  <xsl:template match="/root/html/body/span[@class='pagehead']">
    <xsl:variable name='type' value='./text()'/>
    <Events>
     <xsl:for-each select="root/html/body/div/html/body">
      <xsl:apply-templates/>
     </xsl:for-each>
    </Events>
  </xsl:template>





  <xsl:template match="/">
    <Events>
     <xsl:for-each select="/html/body/div/html/body/table[@width='470']">
      <xsl:call-template name="eventTemplate">
       <xsl:with-param name="type" select="."/>
      </xsl:call-template>
     </xsl:for-each>
    </Events>
  </xsl:template>

  <xsl:key name="type_key" match="/html/body/div/html/body/span[@class='pagehead']"/>

  <xsl:template match="/">
    <Events>
     <xsl:for-each select='/html/body/div/html/body/span[@class='pagehead']'>
      <xsl:call-template name="typeTemplate">
       <xsl:with-param name="type" select="." tunnel="yes"/>
      </xsl:call-template>
     </xsl:for-each>
    </Events>
  </xsl:template>

  <xsl:template name="typeTemplate">
   <xsl:param name="type"/>
   <xsl:for-each select='following-sibling::*'>
    
   </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>



<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:ext="http://exslt.org/common">

  <xsl:output indent="yes" encoding="iso-8859-1"/>


  <!--***************************************************************-->
  <!-- Ignore other content -->
  <xsl:template match="text()" />
  <!--***************************************************************-->

  
  <!--***************************************************************-->
  <!-- Main loop -->
  <xsl:template match="/">
    <html>
      <head>
        <title>Select Choices</title>
      </head>
      <body>
        <xsl:apply-templates/>
      </body>
    </html>
    </xsl:template>
  <!--***************************************************************-->


  <!--***************************************************************-->
  <!-- Results -->
  <xsl:template match="rss">
    <Results id="Results">
      <xsl:apply-templates/>
    </Results>
  </xsl:template>
  <!--***************************************************************-->


  <!--***************************************************************-->
  <!-- Assembly -->
  <xsl:template match="item">
    <a>
      <xsl:attribute name="href">
        <xsl:value-of select="enclosure/@url"/>
      </xsl:attribute>
      <xsl:value-of select="category"/>-<xsl:value-of select="pubDate"/>
      </a>
    <br/>
  </xsl:template>
  <!--***************************************************************-->

</xsl:stylesheet>

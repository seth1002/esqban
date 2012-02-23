<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:vn="http://dyomedea.com/ns/variable-names">
	
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	
	<xsl:variable name="class"	select="'Album'"/>





	<xsl:include href="recursive-split.xsl"/>


	<!-- just a shortcut... -->
	<xsl:variable name="varnames" select="document('')/xsl:stylesheet/vn:templates"/>

	
	<!-- hooks -->
	<vn:templates>
		<vn:lyrics/>
	</vn:templates>






	<!-- Entry point -->
	<xsl:template match="/">
		<items class="{$class}">
			<item class="{$class}">
				<xsl:call-template name="basic"/>
				<xsl:call-template name="tracks"/>
			</item>
		</items>
	</xsl:template>


	<!-- Basic properties -->
	<xsl:template name="basic">
		<property name="Name">
			<xsl:variable name="tmp" select="html/html/body/table[2]/tr/td/table/tr/td[2]/table/div[@id='TopID']/table/tr/td/table/tr/td/table/tr[3]/td/table/tr[1]/td/text()"/>
			<xsl:value-of select="substring-after($tmp,'אמן:')"/>
			<!--<xsl:value-of select="html/html/body/table[2]/tr/td/table/tr/td[2]/table/div[@id='TopID']/table/tr/td/table/tr/td/table/tr[3]/td/table/tr[1]/td/text()"/>-->
		</property>
		<property name="Labels">
			<items class="Label">
				<item class="Label">
					<property name="Name">
						<xsl:variable name="tmp" select="html/html/body/table[2]/tr/td/table/tr/td[2]/table/div[@id='TopID']/table/tr/td/table/tr/td/table/tr[3]/td/table/tr[2]/td/text()"/>
						<xsl:value-of select="substring-after($tmp,'שם ההוצאה: ')"/>
						<!--<xsl:value-of select="html/html/body/table[2]/tr/td/table/tr/td[2]/table/div[@id='TopID']/table/tr/td/table/tr/td/table/tr[3]/td/table/tr[2]/td/text()"/>-->
					</property>
				</item>
			</items>
		</property>
	</xsl:template>


	<!-- Tracks -->
	<xsl:template name="tracks">
		<property name="Tracks">
			<items class="Tracks">
				<!--<xsl:for-each select="html/html/body/table/tr/td[2]/table[@width='574']/tr/td[@height='860']/table/div[@id='TopID']/table[@width='90%']/tr/td/table[2]/tr/td[@align='right']/table/tr/td[2]/table/tr/td[@width='40%' and @class='text7666']/table[@width='280']/tr/td[@class='text8']">-->
				<xsl:for-each select="html/html/body/table/tr/td[2]/table[@width='574']/tr/td[@height='860']/table/div[@id='TopID']/table[@width='90%']/tr/td/table[2]/tr/td[@align='right']/table/tr/td[2]/table/tr[td/@width='40%' and td/@class='text7666' and td/table/@width='280' and td/table/tr/td/@class='text8']">
					<item class="Track">
						<xsl:variable name="name" select="td[@width='40%' and @class='text7666']/table[@width='280']/tr/td[@class='text8']/text()"/>
						<property name="Name">
							<xsl:value-of select="$name"/>
						</property>
						<property name="Track Number">
							<xsl:value-of select="position()"/>
						</property>
						
						<xsl:variable name="tmp" select="td[2]/table[@width='150']/tr[1]/td[@class='text7666']/text()"/>
						<xsl:variable name="words" select="substring-after($tmp, ':')"/>
						<xsl:call-template name="song">
							<xsl:with-param name="name" select="$name"/>
							<xsl:with-param name="words" select="$words"/>
						</xsl:call-template>

						<!-- Not in the class, yet
						<xsl:variable name="tmp2" select="td[2]/table[@width='150']/tr[2]/td[@class='text7666']/text()"/>
						<xsl:variable name="tune" select="substring-after($tmp, ':')"/>
						<property name="Tune">
							<xsl:value-of select="$tune"/>
						</property>

						<xsl:variable name="tmp3" select="td[2]/table[@width='150']/tr[3]/td[@class='text7666']/text()"/>
						<xsl:variable name="processing" select="substring-after($tmp, ':')"/>
						<property name="Processing">
							<xsl:value-of select="$processing"/>
						</property>
						-->
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>







	<!-- Song -->
	<xsl:template name="song">
		<xsl:param name="name"/>
		<xsl:param name="words"/>
		
		<property name="Songs">
			<items class="Song">
				<item class="Song">
					<property name="Name">
						<xsl:value-of select="$name"/>
					</property>
					<property name="Musicians">
						<items class="Musician">
							<xsl:variable name="templateName" select="$varnames/*[local-name()='lyrics']"/>
							<xsl:call-template name="recursive-split">
								<xsl:with-param name="value"			select="$words"/>
								<xsl:with-param name="delimiter"		select="','"/>
								<xsl:with-param name="templateName" 	select="$templateName"/>
							</xsl:call-template>
						</items>
					</property>
				</item>
			</items>
		</property>

	</xsl:template>


	<xsl:template match="vn:lyrics">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>
		<item class="Musician">
			<property name="Name">
				<xsl:value-of select="$param"/>
			</property>
		</item>
	</xsl:template>

</xsl:stylesheet>



<!--
html/html/body/table[@width='774']/tr/td[2]/table/div[@id='TopID']/table[@width='90%']/tr/td/table[@width='95%']/tr[4]/td/table[@width='100%']/tr[4]/td/table[@width='98%']/tr/td[@class='text10']
-->
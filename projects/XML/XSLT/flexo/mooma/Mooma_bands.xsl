<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:vn="http://dyomedea.com/ns/variable-names">
	
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


	<xsl:include href="recursive-split.xsl"/>

	
	<xsl:variable name="class"	select="'Band'"/>




	<!-- just a shortcut... -->
	<xsl:variable name="varnames" select="document('')/xsl:stylesheet/vn:templates"/>

	
	<!-- hooks -->
	<vn:templates>
		<vn:member/>
		<vn:genre/>
		<vn:established/>
	</vn:templates>




	<!-- Entry point: Album -->
	<xsl:template match="/">
		<items class="{$class}">
			<item class="{$class}">
				<xsl:call-template name="basic"/>
				<xsl:call-template name="genres"/>
				<xsl:call-template name="established"/>
				<xsl:call-template name="members"/>
				<xsl:call-template name="albums"/>
			</item>
		</items>
	</xsl:template>


	<!-- Basic properties -->
	<xsl:template name="basic">
		<property name="Name">
			<xsl:value-of select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr/td[2]"/>
		</property>
		<!--<property name="Established">
			<xsl:value-of select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[5]/td[@class='text12bold']/table/tr/td[@class='text8']/text()"/>
		</property>
		<property name="Broke apart">
			<xsl:value-of select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[6]/td[@class='text12bold']/table/tr/td[@class='text8']/text()"/>
		</property>-->
	</xsl:template>


	<!-- Members -->
	<xsl:template name="members">
		<property name="Members">
			<items class="Musician">
				<!--<xsl:variable name="tmp" select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[4]/td/table/tr/td[@class='text10']"/>
				<td>
					<xsl:value-of select="$tmp"/>
				</td>-->
				<xsl:for-each select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[4]/td/table/tr/td[@class='text10']/a">
					<item class="Musician">
						<property name="Name">
							<xsl:value-of select="text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Albums -->
	<xsl:template name="albums">
		<property name="Albums">
			<items class="Album">
				<xsl:for-each select="html/table/tr/td[2]/table/tr[@height='10']">
					<item class="Album">
						<property name="Name">
							<xsl:value-of select="td[@class='text7666']/a[@class='albumTrack']/text()"/>
						</property>
						<property name="Label">
							<xsl:value-of select="td[@class='text7666' and @width='20%']/text()"/>
						</property>
						<property name="Year">
							<xsl:value-of select="td[@class='text7666' and @width='6%']/text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>








	<!-- Members 
	<xsl:template name="members2">
		<xsl:variable name="templateName" select="$varnames/*[local-name()='member']"/>
		<xsl:call-template name="recursive-split">
			<xsl:with-param name="value"			select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[4]/td/table/tr/td[@class='text10']"/>
			<xsl:with-param name="delimiter"		select="','"/>
			<xsl:with-param name="templateName" 	select="$templateName"/>
		</xsl:call-template>
	</xsl:template>


	<xsl:template match="vn:member">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2" value=""/>

		<xsl:variable name="characterName" select="$param"/>
		<xsl:variable name="actorName" select="$param1"/>

		<item class="Member">
			<property name="Name">
				<xsl:value-of select="text()"/>
			</property>
		</item>
	</xsl:template>-->










	<!-- Genres -->
	<xsl:template name="genres">
		<xsl:variable name="templateName" select="$varnames/*[local-name()='genre']"/>
		<property name="Music Genres">
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[7]/td[@class='text12bold']/table/tr/td[@class='text10']/text()"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<!--<xsl:with-param name="delimiter2"		select="'/'"/>-->
				<xsl:with-param name="templateName" 	select="$templateName"/>
				<!--<xsl:with-param name="templateParam1"	select="'xxx'"/>-->
			</xsl:call-template>
		</property>
	</xsl:template>


	<xsl:template match="vn:genre">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>

		<item class="Music Genre">
			<property name="Name">
				<xsl:value-of select="$param"/>
			</property>
		</item>
	</xsl:template>






	<!-- Established -->
	<xsl:template name="established">
		<xsl:variable name="templateName" select="$varnames/*[local-name()='established']"/>
		<xsl:call-template name="recursive-split">
			<xsl:with-param name="value"			select="html/table/tr/td[2]/table/tr/td[4]/table/tr/td/table/tr/td/table/tr[5]/td[@class='text12bold']/table/tr/td[@class='text8']/text()"/>
			<xsl:with-param name="delimiter"		select="','"/>
			<!--<xsl:with-param name="delimiter2"		select="'/'"/>-->
			<xsl:with-param name="templateName" 	select="$templateName"/>
			<!--<xsl:with-param name="templateParam1"	select="'xxx'"/>-->
		</xsl:call-template>
	</xsl:template>


	<xsl:template match="vn:established">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="Year">
					<xsl:value-of select="$param"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<!-- Place -->
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="Countries">
					<item class="Country">
						<property name="Name">
							<xsl:value-of select="$param"/>
						</property>
					</item>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>



</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


<!--
	<xsl:include href="../main.xsl"/>
	<xsl:include href="../recursive-split.xsl"/>
-->

	
	<xsl:variable name="class"	select="'Actor'"/>


	<!-- Entry point: Actor -->
	<xsl:template match="/">
		<items class="{$class}">
			<item class="{$class}">
				<xsl:call-template name="actorBasic"/>
				<xsl:call-template name="person"/>
				<!--<xsl:call-template name="movies"/>-->
			</item>
		</items>
	</xsl:template>


	<!-- Basic actor's properties -->
	<xsl:template name="actorBasic">
		<property name="Name">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/h1/strong[@class='title']"/>
		</property>
		<property name="Passport Picture">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[1]/a[@name='headshot']/img/@src"/>
		</property>
	</xsl:template>


	<!-- Person -->
	<xsl:template name="person">
		<property name="Person">
			<items class="Persons">
				<item class="Person">
					<xsl:call-template name="personBasic"/>
				</item>
			</items>
		</property>
	</xsl:template>


	<!-- Basic person's properties -->
	<xsl:template name="personBasic">
		<property name="Name">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/h1/strong[@class='title']"/>
		</property>
		<property name="Passport Picture">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[1]/a[@name='headshot']/img/@src"/>
		</property>


		<property name="Birth Date">
			<xsl:variable name="day"  select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[@width='90%']/dl/dd[contains((preceding-sibling::div)[position()=last()]/text(), 'Date of birth')]/a[contains(@href, 'OnThisDay')]"/>
			<xsl:variable name="year" select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[@width='90%']/dl/dd[contains((preceding-sibling::div)[position()=last()]/text(), 'Date of birth')]/a[contains(@href, 'BornInYear')]"/>
			<xsl:variable name="year_space" select="concat(' ', $year)"/>
			<xsl:value-of select="concat($day, $year_space)"/>
		</property>
		<property name="Birth place">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[@width='90%']/dl/dd[contains((preceding-sibling::div)[position()=last()]/text(), 'Date of birth')]/a[contains(@href, 'BornWhere')]"/>
		</property>
		<property name="Year">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[@width='90%']/dl/dd[contains((preceding-sibling::div)[position()=last()]/text(), 'Date of birth')]/a[contains(@href, 'BornInYear')]"/>
		</property>

		<property name="Died on">
			<xsl:variable name="day"  select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[@width='90%']/dl/dd[contains((preceding-sibling::div)[position()=last()]/text(), 'Date of death')]/a[contains(@href, 'OnThisDay')]"/>
			<xsl:variable name="year" select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/table/tr/td[@width='90%']/dl/dd[contains((preceding-sibling::div)[position()=last()]/text(), 'Date of death')]/a[contains(@href, 'DiedInYear')]"/>
			<xsl:variable name="year_space" select="concat(' ', $year)"/>
			<xsl:value-of select="concat($day, $year_space)"/>
		</property>
	</xsl:template>


	<!-- Movies -->
	<!--<xsl:template name="movies">
		<property name="Movies">
			<items class="Movies">
				<!-<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/ol[contains((preceding-sibling::p)[position()=last()]/b/text(), 'filmography')]/li/a">->
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%']/table/tr/td[@width='90%']/ol[1]/li/a">
					<item class="Movie">
						<property name="Name">
							<xsl:value-of select="text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>-->
	

</xsl:stylesheet>

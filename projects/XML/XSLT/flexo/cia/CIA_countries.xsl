<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:vn="http://dyomedea.com/ns/variable-names">
	
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


	<!-- Start of recursive-split stuff -->
	<xsl:include href="recursive-split.xsl"/>

	<!-- just a shortcut... -->
	<xsl:variable name="varnames" select="document('')/xsl:stylesheet/vn:templates"/>
	
	<!-- hooks -->
	<vn:templates>
		<vn:coordinates/>
		<vn:area/>
		<vn:landBoundaries/>
		<vn:borderCountries/>
		<vn:elevationExtremes/>
		<vn:naturalResources/>
		<vn:landUse/>
		<vn:environmentCurrentIssues/>
		<vn:environmentInternationalAgreements/>
		<vn:environmentInternationalAgreements3/>
		<vn:medianAge/>
		<vn:lifeExpectancyAtBirth/>
		<vn:nationality/>
		<vn:ethnicGroups/>
		<vn:religions/>
		<vn:languages/>
		<vn:literacy/>
		<vn:countryName/>
		<vn:capital/>
		<vn:administrativeDivisions/>
		<vn:administrativeDivisions2/>
		<vn:executiveBranch/>
		<vn:legislativeBranch/>
		<vn:politicalPartiesAndLeaders/>
		<vn:internationalOrganizationParticipation/>
	</vn:templates>
	<!-- End of recursive-split stuff -->



	<xsl:variable name="LF" select="'
'"/>


	<xsl:variable name="class"	select="'Country'"/>


	<!-- Entry point -->
	<xsl:template match="/">
		<items class="{$class}">
			<item class="{$class}">
				<xsl:call-template name="basic"/>

				<xsl:call-template name="geography"/>
				<xsl:call-template name="people"/>
				<xsl:call-template name="government"/>
				<xsl:call-template name="economy"/>
				<xsl:call-template name="communications"/>
				<xsl:call-template name="transportation"/>
				<xsl:call-template name="military"/>
				<xsl:call-template name="transnational_issues"/>
			</item>
		</items>
	</xsl:template>


	<!-- Basic properties -->
	<xsl:template name="basic">
		<property name="Name">
			<xsl:value-of select="html/body/div[@align='center']/table[@width='596' and @height='400']/tr[3]/td[2]/div[@align='left']/table/tr[2]/td/table/tr/td[2]/text()"/>
		</property>
		<property name="FlagURL">
			<xsl:value-of select="html/body/div[@align='center']/table[@width='596' and @height='400']/tr[3]/td[2]/div[@align='left']/table/tr[2]/td/table/tr/td[4]/div/a/img/@src"/>
		</property>
	</xsl:template>
		



	<xsl:template name="GeographicCoordinates">
		<xsl:variable name="group" select="'Geography'"/>
		<!--<property name="Geographic coordinates">-->
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Geographic coordinates'"/>
				</xsl:call-template>
			</xsl:variable>
			
			<xsl:variable name="templateName" select="$varnames/*[local-name()='coordinates']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		<!--</property>-->
	</xsl:template>

	<xsl:template name="Area">
		<xsl:variable name="group" select="'Geography'"/>
		<!--<property name="Area">-->
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Area'"/>
				</xsl:call-template>
			</xsl:variable>
			
			<xsl:variable name="templateName" select="$varnames/*[local-name()='area']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		<!--</property>-->
	</xsl:template>


	<xsl:template name="Boundaries">
		<xsl:variable name="group" select="'Geography'"/>
		<!--<property name="Land boundaries">-->
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Land boundaries'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='landBoundaries']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		<!--</property>-->
	</xsl:template>

	<xsl:template name="geography">
		<xsl:variable name="group" select="'Geography'"/>

		<!--<property name="Location">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Location'"/>
			</xsl:call-template>
		</property>-->
		<!--<property name="Geographic coordinates">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Geographic coordinates'"/>
				</xsl:call-template>
			</xsl:variable>
			
			<xsl:variable name="templateName" select="$varnames/*[local-name()='coordinates']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>-->
		<xsl:call-template name="GeographicCoordinates"/>
		<!--<property name="Map references">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Map references'"/>
			</xsl:call-template>
		</property>-->
		<!--<property name="Area">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Area'"/>
				</xsl:call-template>
			</xsl:variable>
			
			<xsl:variable name="templateName" select="$varnames/*[local-name()='area']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>-->
		<xsl:call-template name="Area"/>
		<property name="Comparative area">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Area - comparative'"/>
			</xsl:call-template>
		</property>
		<!--<property name="Land boundaries">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Land boundaries'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='landBoundaries']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>-->
		<xsl:call-template name="Boundaries"/>
		<property name="Coastline">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Coastline'"/>
			</xsl:call-template>
		</property>
		<property name="Maritime claims">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Maritime claims'"/>
			</xsl:call-template>
		</property>
		<property name="Climate">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Climate'"/>
			</xsl:call-template>
		</property>
		<property name="Terrain">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Terrain'"/>
			</xsl:call-template>
		</property>
		<property name="Elevation extremes">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Elevation extremes'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='elevationExtremes']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Natural resources">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Natural resources'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='naturalResources']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Land use">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Land use'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='landUse']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Irrigated land">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Irrigated land'"/>
			</xsl:call-template>
		</property>
		<property name="Natural hazards">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name" select="'Natural hazards'"/>
			</xsl:call-template>
		</property>
		<property name="Environment - current issues">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Environment - current issues'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='environmentCurrentIssues']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="';'"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Environment - international agreements">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name" select="'Environment - international agreements'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='environmentInternationalAgreements']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Geography - note">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Geography - note'"/>
			</xsl:call-template>
		</property>
	</xsl:template>




	<xsl:template name="people">
		<xsl:variable name="group" select="'People'"/>

		<property name="Population">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Population'"/>
			</xsl:call-template>
		</property>
		<property name="Median age">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Median age'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='medianAge']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Life expectancy at birth">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Life expectancy at birth'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='lifeExpectancyAtBirth']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Total fertility rate">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Total fertility rate'"/>
			</xsl:call-template>
		</property>
		<property name="Nationality">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Nationality'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='nationality']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Ethnic groups">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Ethnic groups'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='ethnicGroups']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Religions">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Religions'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='religions']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Languages">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Languages'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='languages']"/>
			<xsl:call-template name="recursive-split-new">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="';'"/>
				<xsl:with-param name="delimiter2"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Literacy">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Literacy'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='literacy']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
	</xsl:template>




	<xsl:template name="government">
		<xsl:variable name="group" select="'Government'"/>

		<property name="Country name">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Country name'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='countryName']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Government type">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Government type'"/>
			</xsl:call-template>
		</property>
		<property name="Capital">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Capital'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='capital']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Administrative divisions">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Administrative divisions'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='administrativeDivisions']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Independence">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Independence'"/>
			</xsl:call-template>
		</property>
		<property name="National holiday">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'National holiday'"/>
			</xsl:call-template>
		</property>
		<property name="Constitution">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Constitution'"/>
			</xsl:call-template>
		</property>
		<property name="Legal system">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Legal system'"/>
			</xsl:call-template>
		</property>
		<property name="Suffrage">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Suffrage'"/>
			</xsl:call-template>
		</property>
		<property name="Executive branch">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Executive branch'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='executiveBranch']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Legislative branch">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Legislative branch'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='legislativeBranch']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="$LF"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Judicial branch">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Judicial branch'"/>
			</xsl:call-template>
		</property>
		<property name="Political parties and leaders">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'Political parties and leaders'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='politicalPartiesAndLeaders']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="';'"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="International organization participation">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetGroupValue">
					<xsl:with-param name="group" select="$group"/>
					<xsl:with-param name="name"  select="'International organization participation'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="templateName" select="$varnames/*[local-name()='internationalOrganizationParticipation']"/>
			<xsl:call-template name="recursive-split">
				<xsl:with-param name="value"			select="$tmp"/>
				<xsl:with-param name="delimiter"		select="','"/>
				<xsl:with-param name="templateName" 	select="$templateName"/>
			</xsl:call-template>
		</property>
		<property name="Diplomatic representation in the US">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Diplomatic representation in the US'"/>
			</xsl:call-template>
		</property>
		<property name="Diplomatic representation from the US">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Diplomatic representation from the US'"/>
			</xsl:call-template>
		</property>
		<property name="Flag description">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Flag description'"/>
			</xsl:call-template>
		</property>
	</xsl:template>


	<xsl:template name="economy">
		<xsl:variable name="group" select="'Economy'"/>

		<property name="Economy - overview">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Economy - overview'"/>
			</xsl:call-template>
		</property>
		<property name="GDP (purchasing power parity)">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'GDP (purchasing power parity)'"/>
			</xsl:call-template>
		</property>
		<property name="GDP (official exchange rate)">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'GDP (official exchange rate)'"/>
			</xsl:call-template>
		</property>
		<property name="GDP - real growth rate">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'GDP - real growth rate'"/>
			</xsl:call-template>
		</property>
		<property name="GDP - per capita (PPP)">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'GDP - per capita (PPP)'"/>
			</xsl:call-template>
		</property>
		<property name="GDP - composition by sector">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'GDP - composition by sector'"/>
			</xsl:call-template>
		</property>
		<property name="Labor force">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Labor force'"/>
			</xsl:call-template>
		</property>
		<property name="Labor force - by occupation">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Labor force - by occupation'"/>
			</xsl:call-template>
		</property>
		<property name="Unemployment rate">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Unemployment rate'"/>
			</xsl:call-template>
		</property>
		<property name="Population below poverty line">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Population below poverty line'"/>
			</xsl:call-template>
		</property>
		<property name="Inflation rate (consumer prices)">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Inflation rate (consumer prices)'"/>
			</xsl:call-template>
		</property>
		<property name="Investment (gross fixed)">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Investment (gross fixed)'"/>
			</xsl:call-template>
		</property>
		<property name="Budget">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Budget'"/>
			</xsl:call-template>
		</property>
		<property name="Public debt">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Public debt'"/>
			</xsl:call-template>
		</property>
		<property name="Agriculture - products">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Agriculture - products'"/>
			</xsl:call-template>
		</property>
		<property name="Industries">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Industries'"/>
			</xsl:call-template>
		</property>
		<property name="Industrial production growth rate">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Industrial production growth rate'"/>
			</xsl:call-template>
		</property>
		<property name="Electricity - production">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Electricity - production'"/>
			</xsl:call-template>
		</property>
		<property name="Electricity - consumption">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Electricity - consumption'"/>
			</xsl:call-template>
		</property>
		<property name="Electricity - exports">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Electricity - exports'"/>
			</xsl:call-template>
		</property>
		<property name="Electricity - imports">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Electricity - imports'"/>
			</xsl:call-template>
		</property>
		<property name="Oil - production">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Oil - production'"/>
			</xsl:call-template>
		</property>
		<property name="Oil - consumption">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Oil - consumption'"/>
			</xsl:call-template>
		</property>
		<property name="Oil - proved reserves">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Oil - proved reserves'"/>
			</xsl:call-template>
		</property>
		<property name="Natural gas - production">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Natural gas - production'"/>
			</xsl:call-template>
		</property>
		<property name="Natural gas - consumption">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Natural gas - consumption'"/>
			</xsl:call-template>
		</property>
		<property name="Natural gas - proved reserves">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Natural gas - proved reserves'"/>
			</xsl:call-template>
		</property>
		<property name="Current account balance">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Current account balance'"/>
			</xsl:call-template>
		</property>
		<property name="Exports">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Exports'"/>
			</xsl:call-template>
		</property>
		<property name="Exports - commodities">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Exports - commodities'"/>
			</xsl:call-template>
		</property>
		<property name="Imports">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Imports'"/>
			</xsl:call-template>
		</property>
		<property name="Debt - external">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Debt - external'"/>
			</xsl:call-template>
		</property>
		<property name="Economic aid - recipient">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Economic aid - recipient'"/>
			</xsl:call-template>
		</property>
		<property name="Currency (code)">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Currency (code)'"/>
			</xsl:call-template>
		</property>
		<property name="Exchange rates">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Exchange rates'"/>
			</xsl:call-template>
		</property>
	</xsl:template>


	<xsl:template name="communications">
		<xsl:variable name="group" select="'Communications'"/>

		<property name="Telephones - main lines in use">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Telephones - main lines in use'"/>
			</xsl:call-template>
		</property>
		<property name="Telephones - mobile cellular">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Telephones - mobile cellular'"/>
			</xsl:call-template>
		</property>
		<property name="Telephone system">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Telephone system'"/>
			</xsl:call-template>
		</property>
		<property name="Radio broadcast stations">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Radio broadcast stations'"/>
			</xsl:call-template>
		</property>
		<property name="Internet country code">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Internet country code'"/>
			</xsl:call-template>
		</property>
		<property name="Internet hosts">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Internet hosts'"/>
			</xsl:call-template>
		</property>
		<property name="Internet users">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Internet users'"/>
			</xsl:call-template>
		</property>
	</xsl:template>




	<xsl:template name="transportation">
		<xsl:variable name="group" select="'Transportation'"/>

		<property name="Airports">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Airports'"/>
			</xsl:call-template>
		</property>
		<property name="Airports - with paved runways">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Airports - with paved runways'"/>
			</xsl:call-template>
		</property>
		<property name="Airports - with unpaved runways">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Airports - with unpaved runways'"/>
			</xsl:call-template>
		</property>
		<property name="Heliports">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Heliports'"/>
			</xsl:call-template>
		</property>
		<property name="Pipelines">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Pipelines'"/>
			</xsl:call-template>
		</property>
		<property name="Railways">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Railways'"/>
			</xsl:call-template>
		</property>
		<property name="Roadways">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Roadways'"/>
			</xsl:call-template>
		</property>
		<property name="Waterways">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Waterways'"/>
			</xsl:call-template>
		</property>
		<property name="Merchant marine">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Merchant marine'"/>
			</xsl:call-template>
		</property>
	</xsl:template>





	<xsl:template name="military">
		<xsl:variable name="group" select="'Military'"/>

		<property name="Military service age and obligation">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Military service age and obligation'"/>
			</xsl:call-template>
		</property>
	</xsl:template>



	<xsl:template name="transnational_issues">
		<xsl:variable name="group" select="'Transnational Issues'"/>

		<property name="Disputes - international">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Disputes - international'"/>
			</xsl:call-template>
		</property>
		<property name="Refugees and internally displaced persons">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Refugees and internally displaced persons'"/>
			</xsl:call-template>
		</property>
		<property name="Illicit drugs">
			<xsl:call-template name="GetGroupValue">
				<xsl:with-param name="group" select="$group"/>
				<xsl:with-param name="name"  select="'Illicit drugs'"/>
			</xsl:call-template>
		</property>
	</xsl:template>



	<xsl:template name='GetGroupValue'>
		<xsl:param name="group"/>
		<xsl:param name="name"/>
		<xsl:variable name="tmp" select="concat($name, ':')"/>
		<xsl:value-of select="html/body/table[(preceding-sibling::table)[position()=last()]/tr/td[2]/font/b/a/text()=$group]/tr/td[(preceding-sibling::td)[position()=last()]/div/text() = $tmp]"/>
	</xsl:template>














	<!-- Coordinates -->
	<xsl:template match="vn:coordinates">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="Latitude">
					<xsl:value-of select="$param"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="Longitude">
					<xsl:value-of select="$param"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!-- Area -->
	<xsl:template match="vn:area">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="totalArea">
					<xsl:value-of select="substring-after($param,':')"/>
					<!--<xsl:call-template name="regexTest">
						<xsl:with-param name="value" select="$param"/>
					</xsl:call-template>-->
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="landArea">
					<xsl:value-of select="substring-after($param,':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="waterArea">
					<xsl:value-of select="substring-after($param,':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--<xsl:template name="regexTest">
		<xsl:param name="value"/>
		<xsl:analyze-string select="$value" regex="\s*(.*)\s*,\s*([A-Z]{{2}})\s+(\d{{5}}(\-\d{{4}})?)\s*">
			<xsl:matching-substring>
				<city><xsl:value-of select="regex-group(1)"/></city>
				<state><xsl:value-of select="regex-group(2)"/></state>
				<zip><xsl:value-of select="regex-group(3)"/></zip>
			</xsl:matching-substring>
			
			<xsl:non-matching-substring>
				<addrLine2>
					<xsl:value-of select="$elValue"/>
				</addrLine2>
			</xsl:non-matching-substring>
		</xsl:analyze-string>
	</xsl:template>-->


	<!-- Land Boundaries -->
	<xsl:template match="vn:landBoundaries">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="totalBoundaries">
					<xsl:value-of select="substring-after($param,':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="Border Countries">
					<items class="Border Country">
						<xsl:variable name="templateName" select="$varnames/*[local-name()='borderCountries']"/>
						<xsl:call-template name="recursive-split">
							<xsl:with-param name="value"			select="substring-after($param,':')"/>
							<xsl:with-param name="delimiter"		select="','"/>
							<xsl:with-param name="templateName" 	select="$templateName"/>
						</xsl:call-template>
					</items>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	<xsl:template match="vn:borderCountries">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<item class="Border Country">
			<property name="Country">
				<items class="Country">
					<item class="Country">
						<property name="Name">
							<xsl:value-of select="$param"/>
						</property>
					</item>
				</items>
			</property>
			<property name="Border Length">
				<!--<xsl:if test="matches($param,'.*\d+\.\d{3}.*')">-->
				<xsl:if test="contains($param,' km')">
					<xsl:variable name="tmp" select="substring-before($param,' km')"/>
					<xsl:value-of select="substring-after($param,$tmp)"/>
				</xsl:if>
			</property>
		</item>
	</xsl:template>
	


	<!-- Elevation Extremes -->
	<xsl:template match="vn:elevationExtremes">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="lowest point">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="highest point">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- Natural Resources -->
	<xsl:template match="vn:naturalResources">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="resource">
			<xsl:value-of select="$param"/>
		</property>
	</xsl:template>


	<!-- Land use -->
	<xsl:template match="vn:landUse">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="arable land">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="permanent crops">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="other">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!-- Environment - current issues -->
	<xsl:template match="vn:environmentCurrentIssues">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="issue">
			<xsl:value-of select="$param"/>
		</property>
	</xsl:template>





	<!-- Environment - international agreements -->
	<xsl:template match="vn:environmentInternationalAgreements">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<xsl:call-template name="environmentInternationalAgreements2">
					<xsl:with-param name="value" select="$param"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="environmentInternationalAgreements2">
		<xsl:param name="value"/>

		<xsl:variable name="templateName" select="$varnames/*[local-name()='environmentInternationalAgreements3']"/>
		<xsl:call-template name="recursive-split">
			<xsl:with-param name="value"			select="$value"/>
			<xsl:with-param name="delimiter"		select="','"/>
			<xsl:with-param name="templateName" 	select="$templateName"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="vn:environmentInternationalAgreements3">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="party to">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
				<property name="party to">
					<xsl:value-of select="$param"/>
				</property>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!-- Median age -->
	<xsl:template match="vn:medianAge">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="total">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="male">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="female">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!-- Life expectancy at birth -->
	<xsl:template match="vn:lifeExpectancyAtBirth">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="total population">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="male">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="female">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>



	<!-- Nationality -->
	<xsl:template match="vn:nationality">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="noun">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="adjective">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- Ethnic groups -->
	<xsl:template match="vn:ethnicGroups">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="ethnicGroup">
			<xsl:value-of select="$param"/>
		</property>
	</xsl:template>



	<!-- Religions -->
	<xsl:template match="vn:religions">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="religion">
			<xsl:value-of select="$param"/>
		</property>
	</xsl:template>



	<!-- Literacy -->
	<xsl:template match="vn:literacy">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="definition">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="total population">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="male">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 3">
				<property name="female">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!-- Country Name -->
	<xsl:template match="vn:countryName">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="conventional long form">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="conventional short form">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="local long form">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 3">
				<property name="local short form">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 4">
				<property name="former">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!-- Capital -->
	<xsl:template match="vn:capital">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="name">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="geographic coordinates">
					<xsl:variable name="templateName" select="$varnames/*[local-name()='coordinates']"/>
					<xsl:call-template name="recursive-split">
						<xsl:with-param name="value"			select="substring-after($param, ':')"/>
						<xsl:with-param name="delimiter"		select="','"/>
						<xsl:with-param name="templateName" 	select="$templateName"/>
					</xsl:call-template>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="time difference">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 3">
				<property name="daylight saving time">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	
	
	
	<!-- Administrative Divisions -->
	<xsl:template match="vn:administrativeDivisions">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
			</xsl:when>
			<xsl:when test="$index = 1">
			</xsl:when>
			<xsl:when test="$index = 2">
			</xsl:when>
			<xsl:otherwise>
				<property name="Administrative Divisions">
					<property name="Name">
						<xsl:value-of select="substring-before($param, ':')"/>
					</property>
					
					<property name="Cities">
						<xsl:variable name="templateName" select="$varnames/*[local-name()='administrativeDivisions2']"/>
						<xsl:call-template name="recursive-split">
							<xsl:with-param name="value"			select="substring-after($param, ':')"/>
							<xsl:with-param name="delimiter"		select="','"/>
							<xsl:with-param name="templateName" 	select="$templateName"/>
						</xsl:call-template>
					</property>
				</property>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="vn:administrativeDivisions2">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="City">
			<xsl:choose>
				<xsl:when test="contains($param,'*')">
					<!--<xsl:value-of select="replace($param,'*','')"/>-->
					<xsl:value-of select="substring-before($param,'*')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$param"/>
				</xsl:otherwise>
			</xsl:choose>
		</property>
	</xsl:template>




	<!-- Executive branch -->
	<xsl:template match="vn:executiveBranch">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
				<property name="chief of state">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 1">
				<property name="head of government">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="cabinet">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 3">
				<property name="elections">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 4">
				<property name="election results">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>





	<!-- Legislative Branch -->
	<xsl:template match="vn:legislativeBranch">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<xsl:choose>
			<xsl:when test="$index = 0">
			</xsl:when>
			<xsl:when test="$index = 1">
			</xsl:when>
			<xsl:when test="$index = 2">
				<property name="">
					<xsl:value-of select="$param"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 3">
				<property name="elections">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:when test="$index = 4">
				<property name="election results">
					<xsl:value-of select="substring-after($param, ':')"/>
				</property>
			</xsl:when>
			<xsl:otherwise>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>





	<!-- Political parties and leaders -->
	<xsl:template match="vn:politicalPartiesAndLeaders">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="party">
			<xsl:value-of select="$param"/>
		</property>
	</xsl:template>





	<!-- International organization participation -->
	<xsl:template match="vn:internationalOrganizationParticipation">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="organization">
			<xsl:value-of select="$param"/>
		</property>
	</xsl:template>





	<!-- Languages -->
	<xsl:template match="vn:languages">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2"/>
		<xsl:param name="index"/>

		<property name="language">
			<!--<xsl:variable name="tmp">
				<xsl:choose>
					<xsl:when test="contains($param, 'and')">
						<xsl:value-of select="substring-after($param, 'and')"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$param"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="contains($tmp, '(')">
					<xsl:value-of select="substring-before($tmp, '(')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$tmp"/>
				</xsl:otherwise>
			</xsl:choose>-->
			<xsl:call-template name="removeCrap">
				<xsl:with-param name="value" select="$param"/>
			</xsl:call-template>
		</property>
	</xsl:template>
	
	<xsl:template name="removeCrap">
		<xsl:param name="value"/>
		<xsl:variable name="tmp">
			<xsl:choose>
				<xsl:when test="contains($value, 'and')">
					<xsl:value-of select="substring-after($value, 'and')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$value"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="contains($tmp, '(')">
				<xsl:value-of select="substring-before($tmp, '(')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$tmp"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>



</xsl:stylesheet>

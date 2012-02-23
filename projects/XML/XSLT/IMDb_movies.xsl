<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:vn="http://dyomedea.com/ns/variable-names">
	
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


<!--
	<xsl:include href="../main.xsl"/>
	<xsl:include href="../recursive-split.xsl"/>
-->

	
	<xsl:variable name="class"	select="'Film'"/>


	<!-- just a shortcut... -->
	<xsl:variable name="varnames" select="document('')/xsl:stylesheet/vn:templates"/>

	
	<!-- hooks -->
	<vn:templates>
		<vn:character/>
	</vn:templates>


	<xsl:variable name="characterTemplate" select="$varnames/*[local-name()='character']"/>


	<!-- Entry point -->
	<xsl:template match="/">
		<items class="{$class}">
			<item class="{$class}">
				<xsl:call-template name="basic"/>
				<xsl:call-template name="directors"/>
				<xsl:call-template name="writingCredits"/>
				<xsl:call-template name="genres"/>
				<xsl:call-template name="actors"/>
				<xsl:call-template name="characters"/>
				<xsl:call-template name="countries"/>
				<xsl:call-template name="languages"/>
			</item>
		</items>
	</xsl:template>


	<!-- Basic properties -->
	<xsl:template name="basic">
		<property name="Name">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[1]/tr/td/h1/strong[@class='title']/text()"/>
		</property>
		<property name="Year">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[1]/tr/td/h1/strong[@class='title']/small/a"/>
		</property>
		<property name="DVD Cover">
			<xsl:value-of select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[1]/tr/td/table[1]/tr/td/a[@name='poster']/img/@src"/>
		</property>
	</xsl:template>


	<!-- Directors -->
	<xsl:template name="directors">
		<property name="Directors">
			<items class="Director">
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[1]/tr/td/table/tr/td[2]/a[(preceding-sibling::b)[position()=last()]/text() = 'Directed by']">
					<item class="Director">
						<property name="Name">
							<xsl:value-of select="text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Writing credits -->
	<xsl:template name="writingCredits">
		<property name="writing Credits">
			<items class="Writer">
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[1]/tr/td/table/tr/td[2]/a[(preceding-sibling::b)[position()=last()]/text() = 'Writing credits']">
					<item class="Writer">
						<property name="Name">
							<xsl:value-of select="text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Genres -->
	<xsl:template name="genres">
		<property name="Genres">
			<items class="Genre">
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table/tr/td/a[(preceding-sibling::b)[position()=last()]/text() = 'Genre:']">
					<xsl:if test="not(contains(text(), '(more)'))">
						<item class="Genre">
							<property name="Name">
								<xsl:value-of select="text()"/>
							</property>
						</item>
					</xsl:if>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Actors -->
	<xsl:template name="actors">
		<property name="Actors">
			<items class="Actor">
				<!--<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[2]/tr[count(td)=4]">-->
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%' and @valign='top' and @align='left']/table[@cellpadding='1' and @cellspacing='0']/tr[count(td)=4]">
					<item class="Actor">
						<property name="Name">
							<xsl:value-of select="td[2]/a/text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Characters -->
	<xsl:template name="characters">
		<property name="Characters">
			<items class="Character">
				<!--<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/table[2]/tr[count(td)=4]">-->
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td[@width='100%' and @valign='top' and @align='left']/table[@cellpadding='1' and @cellspacing='0']/tr[count(td)=4]">
					<xsl:call-template name="recursive-split">
						<xsl:with-param name="value"			select="td[4]/text()"/>
						<xsl:with-param name="delimiter"		select="','"/>
						<xsl:with-param name="delimiter2"		select="'/'"/>
						<xsl:with-param name="templateName" 	select="$characterTemplate"/>
						<xsl:with-param name="templateParam1"	select="td[2]/a/text()"/>
					</xsl:call-template>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Countries -->
	<xsl:template name="countries">
		<property name="Countries">
			<items class="Country">
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/a[(preceding-sibling::b)[position()=last()]/text() = 'Country:']">
					<item class="Country">
						<property name="Name">
							<xsl:value-of select="text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Languages -->
	<xsl:template name="languages">
		<property name="Languages">
			<items class="Language">
				<xsl:for-each select="html/body/div[@id='root']/layer[@name='root']/table[3]/tr/td/a[(preceding-sibling::b)[position()=last()]/text() = 'Language:']">
					<item class="Language">
						<property name="Name">
							<xsl:value-of select="text()"/>
						</property>
					</item>
				</xsl:for-each>
			</items>
		</property>
	</xsl:template>


	<!-- Recursive split -->
	<xsl:template name="recursive-split">
		<xsl:param name="value"/>
		<xsl:param name="delimiter"/>
		<xsl:param name="delimiter2" value=""/>
		<xsl:param name="templateName"/>
		<xsl:param name="templateParam1" value=""/>
		<xsl:param name="templateParam2" value=""/>

		<xsl:choose>
			<xsl:when test="contains($value, $delimiter)">
				<xsl:variable name="val1" select="substring-before($value, $delimiter)"/>
				<xsl:variable name="val2" select="substring-after($value, $delimiter)"/>
		
				<xsl:apply-templates select="$templateName">
					<xsl:with-param name="param"  select="$val1"/>
					<xsl:with-param name="param1" select="$templateParam1"/>
					<xsl:with-param name="param2" select="$templateParam2"/>
				</xsl:apply-templates>
		
				<xsl:choose>
					<xsl:when test="contains($val2, $delimiter)">
						<xsl:call-template name="recursive-split">
							<xsl:with-param name="value"			select="$val2"/>
							<xsl:with-param name="delimiter"		select="$delimiter"/>
							<xsl:with-param name="templateName"		select="$templateName"/>
							<xsl:with-param name="templateParam1"	select="$templateParam1"/>
							<xsl:with-param name="templateParam2"	select="$templateParam2"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="$templateName">
							<xsl:with-param name="param"  select="$val2"/>
							<xsl:with-param name="param1" select="$templateParam1"/>
							<xsl:with-param name="param2" select="$templateParam2"/>
						</xsl:apply-templates>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="contains($value, $delimiter2)">
				<xsl:call-template name="recursive-split">
					<xsl:with-param name="value"			select="$value"/>
					<xsl:with-param name="delimiter"		select="$delimiter2"/>
					<xsl:with-param name="templateName"		select="$templateName"/>
					<xsl:with-param name="templateParam1"	select="$templateParam1"/>
					<xsl:with-param name="templateParam2"	select="$templateParam2"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="$templateName">
					<xsl:with-param name="param"  select="$value"/>
					<xsl:with-param name="param1" select="$templateParam1"/>
					<xsl:with-param name="param2" select="$templateParam2"/>
				</xsl:apply-templates>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<xsl:template match="vn:character">
		<xsl:param name="param"/>
		<xsl:param name="param1"/>
		<xsl:param name="param2" value=""/>

		<xsl:variable name="characterName" select="$param"/>
		<xsl:variable name="actorName" select="$param1"/>

		<item class="Character">
			<xsl:variable name="name">
				<xsl:variable name="val" select="$characterName"/>
				<xsl:choose>
					<xsl:when test="contains($val, '(')">
						<xsl:value-of select="substring-before($val, '(')"/>
					</xsl:when>
					<xsl:when test="contains($val, ' as ')">
						<xsl:value-of select="substring-before($val, ' as ')"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$val"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<property name="Name">
				<xsl:choose>
					<xsl:when test="contains($name, 'himself')">
						<xsl:value-of select="$actorName"/>
					</xsl:when>
					<xsl:when test="contains($name, 'Himself')">
						<xsl:value-of select="$actorName"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$name"/>
					</xsl:otherwise>
				</xsl:choose>
			</property>

			<property name="Actors">
				<items class="Actor">
					<item class="Actor">
						<property name="Name">
							<xsl:value-of select="$actorName"/>
						</property>
					</item>
				</items>
			</property>
		</item>
	</xsl:template>
	

</xsl:stylesheet>

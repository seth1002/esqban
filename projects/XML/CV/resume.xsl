<?xml version="1.0"  encoding="ISO-8859-1" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html"/>


<xsl:template match = "*">
   <xsl:apply-templates />
</xsl:template>

<!-- Grouping by where attribute -->
<xsl:key name="Courses-by-where" match="Course" use="@where" />
<xsl:template match = "Courses">
	<TABLE cellspacing="1" cellpadding="0" border="0" style="width:640;">
	<TR><TH colspan="2">Courses</TH></TR>
	<xsl:for-each select="Course[count(. | key('Courses-by-where', @where)[1]) = 1]">
		<xsl:sort select="Course" />
		<TR><TH colspan="2"><xsl:value-of select="@where"/></TH></TR>
		<xsl:for-each select="key('Courses-by-where', @where)">
			<xsl:sort select="@where" />
			<TR>
				<TD><xsl:value-of select="@date"/></TD>
				<TD><xsl:value-of select="@name"/></TD>
			</TR>
		</xsl:for-each>
	</xsl:for-each>
	</TABLE>
</xsl:template>

<xsl:template match = "_Courses">
	<TABLE cellspacing="1" cellpadding="0" border="0" style="width:640;">
	<TR><TH colspan="3">Courses</TH></TR>
	<xsl:for-each select="Course">
		<TR>
			<TD><xsl:value-of select="@date"/></TD>
			<TD><xsl:value-of select="@name"/></TD>
			<TD><xsl:value-of select="@where"/></TD>
		</TR>
	</xsl:for-each>
	</TABLE>
</xsl:template>

<xsl:template match = "Schools">
	<TABLE cellspacing="1" cellpadding="0" border="0" style="width:640;">
	<TR><TH colspan="2">Schools</TH></TR>
	<xsl:for-each select="School">
		<TR>
			<TD><xsl:value-of select="@date"/></TD>
			<TD><xsl:value-of select="@name"/></TD>
		</TR>
	</xsl:for-each>
	</TABLE>
</xsl:template>

<xsl:template match = "Education">
	<xsl:apply-templates select = "Schools" />
	<xsl:apply-templates select = "Courses" />
</xsl:template>

<xsl:template match = "Organization">
	<TABLE cellspacing="1" cellpadding="0" border="0" style="width:640;">
		<TR>
			<TH><xsl:value-of select = "@fromDate" /> - <xsl:value-of select = "@toDate" /></TH>
			<TH><xsl:value-of select = "@name"/> - <xsl:value-of select = "Location"/></TH>
		</TR>
		<TR><TH colspan="2" align="left"><xsl:value-of select = "Post/@name" /></TH></TR>
		<xsl:for-each select="Post/Responsibilities/Responsibility">
			<TR><TD colspan="2">- <xsl:value-of select="./text()"/></TD></TR>
		</xsl:for-each>
	</TABLE>
</xsl:template>

<xsl:template match = "Experience">
	<BODY>
		<xsl:apply-templates select = "Organization" />
	</BODY>
</xsl:template>

<xsl:template match = "Skills">
	<TABLE cellspacing="1" cellpadding="0" border="0" style="width:640;">
		<TR><TH colspan="2">Programming languages</TH></TR>
		<TR><TH align="left">Name</TH><TH>Years</TH></TR>
		<xsl:for-each select="ProgrammingLanguages/Skill">
			<TR>
				<TD><xsl:value-of select="@name"/></TD>
				<TD align="center"><xsl:value-of select="@years"/></TD>
			</TR>
		</xsl:for-each>
		<TR><TH colspan="2">Technologies</TH></TR>
		<TR><TH align="left">Name</TH><TH>Years</TH></TR>
		<xsl:for-each select="Technologies/Skill">
			<TR>
				<TD><xsl:value-of select="@name"/></TD>
				<TD align="center"><xsl:value-of select="@years"/></TD>
			</TR>
		</xsl:for-each>
	</TABLE>
</xsl:template>


<xsl:template match = "Languages">
	<TABLE border="0" style="width:640;">
		<TR><TH colspan="2">Languages</TH></TR>
		<xsl:for-each select="Language">
			<TR>
				<TD><xsl:value-of select="@name"/></TD>
				<TD><xsl:value-of select="./text()"/></TD>
			</TR>
		</xsl:for-each>
	</TABLE>
</xsl:template>

<xsl:template match = "Resume">
	<TITLE><xsl:value-of select = "FirstName"/> <xsl:value-of select = "LastName"/>'s Resume</TITLE>
	<H1><xsl:value-of select = "FirstName"/> <xsl:value-of select = "LastName"/></H1>
	<TABLE border="0" style="width:640;">
		<TR>
			<TD>Date of birth:</TD>
			<TD><xsl:value-of select = "BirthDate" /></TD>
			<TD rowspan="6">
				<img src="photo.bmp" width="115" height="139" alt="Photo"/>
			</TD>
		</TR>
		<TR>
			<TD>Place of birth:</TD>
			<TD><xsl:value-of select = "BirthPlace" /></TD>
		</TR>
		<TR>
			<TD>Citizenship:</TD>
			<TD><xsl:value-of select = "Citizenship" /></TD>
		</TR>
		<TR>
			<TD>Address:</TD>
			<TD><xsl:value-of select = "Address" /></TD>
		</TR>
		<TR>
			<TD>Telephone:</TD>
			<TD><xsl:value-of select = "Telephone" /></TD>
		</TR>
		<TR>
			<TD>Email:</TD>
			<TD><xsl:value-of select = "Email" /></TD>
		</TR>
	</TABLE>
	<xsl:apply-templates select = "Languages" />
	<H2>Experience</H2>
	<xsl:apply-templates select = "Experience" />
	<H2>Education</H2>
	<xsl:apply-templates select = "Education" />
	<H2>Skills</H2>
	<xsl:apply-templates select = "Skills" />
</xsl:template>

<xsl:template match = "/">
	<HTML>
		<HEAD>
			<link type="text/css" rel="stylesheet" href="styles.css"/>
			<meta http-equiv="Content-Type" content="text/html; charset=us-ascii"/>
			<BODY>
			<xsl:apply-templates select = "Resume" />
			</BODY>
			<STYLE>th { background-color: #CCCCCC }</STYLE>
			<!-- <BASE href='http://localhost/sqba/'></BASE>    -->
		</HEAD>
	</HTML>
</xsl:template>
</xsl:stylesheet>


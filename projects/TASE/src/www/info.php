<?php
	$user_name = "sqba";
	$password = "crl2688";
	$database = "tase";
	$server = "localhost";

	$db_handle = mysql_connect($server, $user_name, $password);
	$db_found = mysql_select_db($database, $db_handle);

	if ($db_found) {

	$SQL = "SELECT * FROM companies where symbol='" . $_GET["symbol"] . "'";
	$result = mysql_query($SQL);

	while ( $db_field = mysql_fetch_assoc($result) ) {
		print "Name: ";
		print $db_field['name'];

		print "</br>";
		print "Category: ";
		print $db_field['category'];

		print "</br>";
		print "Sector: ";
		print $db_field['sector'];

		print "</br>";
		print "Subsector: ";
		print $db_field['subsector'];

		if (!empty($db_field['url'])) {
			print "</br>";
			print "<a href='";
			print $db_field['url'];
			print "'>URL</a>";
		}

		print "</br>";
		print "<a href='";
		print $db_field['tase_url'];
		print "'>TASE URL</a>";
	}

	mysql_close($db_handle);
}
else {

print "Database NOT Found ";
mysql_close($db_handle);


}
?>

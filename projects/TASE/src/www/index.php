<html><body><h1>TASE</h1>
<?php
	$user_name = "sqba";
	$password = "crl2688";
	$database = "tase";
	$server = "localhost";

	$db_handle = mysql_connect($server, $user_name, $password);
	$db_found = mysql_select_db($database, $db_handle);

	if ($db_found) {

	$SQL = "SELECT * FROM portfolio_status";
	$result = mysql_query($SQL);

	while ( $db_field = mysql_fetch_assoc($result) ) {

		print $db_field['ticker'] . " ";
		print $db_field['buy_date'] . " ";
		print $db_field['buy_price'] . " ";
		print $db_field['quantity'] . " ";
		print $db_field['buy_value'] . " ";
		print $db_field['last_price'] . " ";
		print $db_field['current_value'] . " ";
		print $db_field['yield'] . "<BR>";

	}

	mysql_close($db_handle);
}
else {

print "Database NOT Found ";
mysql_close($db_handle);

}
?>
</body></html>

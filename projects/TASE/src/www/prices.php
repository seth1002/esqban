<?php
	//header('Content-Type: application/json');
	print $_GET["callback"] . "(" . "[";
	$user_name = "sqba";
	$password = "crl2688";
	$database = "tase";
	$server = "localhost";

	$db_handle = mysql_connect($server, $user_name, $password);
	$db_found = mysql_select_db($database, $db_handle);

	if ($db_found) {

	$SQL = "SELECT * FROM company_prices where symbol='" . $_GET["symbol"] . "'";
	$result = mysql_query($SQL);

	while ( $db_field = mysql_fetch_assoc($result) ) {
		print "[";
		print $db_field['date_'];
		print ", ";
		print $db_field['closing_price'];
		print "],";
	}
print "]);";

	mysql_close($db_handle);
}
else {

print "Database NOT Found ";
mysql_close($db_handle);


}
?>

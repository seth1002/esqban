<?php
	header('Content-Type: application/json');
	$user_name = "sqba";
	$password = "crl2688";
	$database = "tase";
	$server = "localhost";

	$db_handle = mysql_connect($server, $user_name, $password);
	$db_found = mysql_select_db($database, $db_handle);

	if ($db_found) {
		$SQL = "SELECT date_ as date, volume FROM all_prices WHERE symbol='" . $_GET["symbol"] . "'";
		$result = mysql_query($SQL);

		print $_GET["callback"] . "(";
		$start = true;
		$output = array();	
		while ( $db_field = mysql_fetch_assoc($result) ) {
			$output[]=$db_field;
		}
		print(json_encode($output));
		print(")");

		mysql_close($db_handle);
	} else {
		print "Database NOT Found ";
		mysql_close($db_handle);
	}
?>

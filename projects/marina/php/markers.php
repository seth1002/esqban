<?php
	header('Content-Type: application/json');
	$user_name = "sqba";
	$password = "crl2688";
	$database = "marinas";
	$server = "localhost";

	$db_handle = mysql_connect($server, $user_name, $password);
	$db_found = mysql_select_db($database, $db_handle);

	if ($db_found) {
		$SQL = "SELECT name, latitude, longitude FROM marinas WHERE latitude < " . $_GET["a"] . " AND latitude > " . $_GET["b"] . " AND longitude < " . $_GET["c"] . " AND longitude > " . $_GET["d"] . " LIMIT 500";

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

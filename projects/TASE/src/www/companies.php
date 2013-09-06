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

	$SQL = "SELECT name, symbol FROM companies";
	$result = mysql_query($SQL);

	$start = true;
	
	while ( $db_field = mysql_fetch_assoc($result) ) {
		if($start == false)
			print "<br>";
		else
			$start = false;
		print "<a href='graph2.php?symbol=";
		print $db_field['symbol'];
		print "'>";
		print $db_field['name'];
		print "</a>";
	}
print "]);";

	mysql_close($db_handle);
}
else {

print "Database NOT Found ";
mysql_close($db_handle);


}
?>

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

	$SQL = "select UNIX_TIMESTAMP(n.date_)*1000 as date_, @rownum := @rownum + 1 AS row_num, n.headline from company_news n, (SELECT @rownum := 0) r where n.symbol='" . $_GET["symbol"] . "'";
	$result = mysql_query($SQL);

	$start = true;
	
	while ( $db_field = mysql_fetch_assoc($result) ) {
		if($start == false)
			print ",\n";
		else
			$start = false;
		print "[";
		print $db_field['date_'];
		print ", ";
		print $db_field['row_num'];
		print ", \"";
		print str_replace("\"", "", $db_field['headline']);
		print "\"]";
	}
print "]);";

	mysql_close($db_handle);
}
else {

print "Database NOT Found ";
mysql_close($db_handle);


}
?>

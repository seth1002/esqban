<html><body><h1>TASE</h1>
<?php
$con=mysqli_connect("localhost","sqba","crl2688","tase");

// Check connection
if (mysqli_connect_errno($con))
  {
	echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }
  else
  {
	$SQL = "SELECT * FROM portfolio_status";
	$result = mysql_query($SQL);

	while ( $db_field = mysql_fetch_assoc($result) ) {
		print $db_field['ticker'] . "<BR>";
		print $db_field['buy_date'] . "<BR>";
		print $db_field['buy_price'] . "<BR>";
		print $db_field['quantity'] . "<BR>";
		print $db_field['buy_value'] . "<BR>";
		print $db_field['last_price'] . "<BR>";
		print $db_field['current_value'] . "<BR>";
		print $db_field['yield'] . "<BR>";
	}
  }

mysqli_close($con);
?>
</body></html>

<html><body><h1>TASE</h1>
<p>This is the default web page for this server.</p>
<p>The web server software is running but no content has been added, yet.</p>
<?php
echo "test";

$con=mysqli_connect("localhost","sqba","crl2688","tase");

// Check connection
if (mysqli_connect_errno($con))
  {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }
  else
  {
  echo "Connected";
  }

mysqli_close($con);
?>
</body></html>

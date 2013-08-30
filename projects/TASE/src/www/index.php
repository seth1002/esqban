<!DOCTYPE HTML>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Highstock Example</title>

		<script type="text/javascript" src="jquery.min.js"></script>
		<script type="text/javascript">
$(function() {

	var str_symbol = '<?php echo $_GET["symbol"]; ?>';
	
	var str_sql = "prices.php?symbol=" + str_symbol + "&callback=?"

	$.getJSON(str_sql, function(data) {
		// Create the chart
		$('#container').highcharts('StockChart', {
			

			rangeSelector : {
				selected : 1
			},

			title : {
				text : 'AAPL Stock Price'
			},
			
			series : [{
				name : 'AAPL',
				data : data,
				tooltip: {
					valueDecimals: 2
				}
			}]
		});
	});

});

		</script>
	</head>
	<body>
<script src="highstock.js"></script>
<script src="exporting.js"></script>

<div id="container" style="height: 500px; min-width: 500px"></div>
	</body>
</html>

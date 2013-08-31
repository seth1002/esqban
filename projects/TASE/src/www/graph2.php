<!DOCTYPE HTML>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Highstock Example</title>

		<script type="text/javascript" src="jquery.min.js"></script>
		<script type="text/javascript">
$(function() {

	var str_symbol = '<?php echo $_GET["symbol"]; ?>';
	
	var str_query1 = "json.php?symbol=" + str_symbol + "&callback=?"
	var str_query2 = "news_json.php?symbol=" + str_symbol + "&callback=?"
	
	var data2;

	$.getJSON(str_query1, function(data_prices) {
		$.getJSON(str_query2, function(data_news) {
			// Create the chart
			$('#container').highcharts('StockChart', {
				

				rangeSelector : {
					selected : 1
				},

				title : {
					text : str_symbol + ' Stock Price'
				},
				
				series : [{
					name : str_symbol + ' Stock Price',
					data : data_prices,
					type : 'area',
					threshold : null,
					tooltip: {
						valueDecimals: 2
					},
					fillColor : {
						linearGradient : {
							x1: 0, 
							y1: 0, 
							x2: 0, 
							y2: 1
						},
						stops : [[0, Highcharts.getOptions().colors[0]], [1, 'rgba(0,0,0,0)']]
					}
				},
				// the event marker flags
				{
					type : 'flags',
					data : data_news,
					onSeries : 'dataseries',
					shape : 'circlepin',
					width : 16
				}]
			});
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

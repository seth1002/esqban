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

	$.getJSON(str_query2, function(data) {
		data2 = data;
	});
	$.getJSON(str_query1, function(data) {
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
				data : data,
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
				data : [{
					x : Date.UTC(2011, 3, 25),
					title : 'H',
					text : 'Euro Contained by Channel Resistance'
				}, {
					x : Date.UTC(2011, 3, 28),
					title : 'G',
					text : 'EURUSD: Bulls Clear Path to 1.50 Figure'
				}, {
					x : Date.UTC(2011, 4, 4),
					title : 'F',
					text : 'EURUSD: Rate Decision to End Standstill'
				}, {
					x : Date.UTC(2011, 4, 5),
					title : 'E',
					text : 'EURUSD: Enter Short on Channel Break'
				}, {
					x : Date.UTC(2011, 4, 6),
					title : 'D',
					text : 'Forex: U.S. Non-Farm Payrolls Expand 244K, U.S. Dollar Rally Cut Short By Risk Appetite'
				}, {
					x : Date.UTC(2011, 4, 6),
					title : 'C',
					text : 'US Dollar: Is This the Long-Awaited Recovery or a Temporary Bounce?'
				}, {
					x : Date.UTC(2011, 4, 9),
					title : 'B',
					text : 'EURUSD: Bearish Trend Change on Tap?'
				}],
				onSeries : 'dataseries',
				shape : 'circlepin',
				width : 16
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

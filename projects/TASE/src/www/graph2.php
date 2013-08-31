<!DOCTYPE HTML>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Highstock Example</title>

		<script type="text/javascript" src="jquery.min.js"></script>
		<script type="text/javascript">
$(function() {

	var str_symbol = '<?php echo $_GET["symbol"]; ?>';
	
	var query_prices = "json.php?symbol=" + str_symbol + "&callback=?"
	var query_news = "news_json.php?symbol=" + str_symbol + "&callback=?"
	
	
	function create_chart(prices, news)
	{
		$('#container').highcharts('StockChart', {
			rangeSelector : {
				selected : 1
			},

			title : {
				text : str_symbol + ' Stock Price'
			},
			
			series : [{
				name : str_symbol + ' Stock Price',
				data : prices,
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
				data : news,
				onSeries : 'dataseries',
				shape : 'circlepin',
				width : 16
			}]
		});
	}

	var news_json="";
	$.getJSON(query_news, function(data) {
		news_json = data;
	}).success(function() {
		$.getJSON(query_prices, function(data) {
			create_chart(data, news_json);
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

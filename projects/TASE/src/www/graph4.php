<!DOCTYPE HTML>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Highstock Example</title>

		<script type="text/javascript">
$(function() {

	var str_symbol = '<?php echo $_GET["symbol"]; ?>';
	
	var query_prices = "prices.json.php?symbol=" + str_symbol + "&callback=?"
	var query_volume = "volume.json.php?symbol=" + str_symbol + "&callback=?"
	var query_news = "news_json.php?symbol=" + str_symbol + "&callback=?"
	
	
	function create_chart(prices, volume, news)
	{
		// set the allowed units for data grouping
		var groupingUnits = [[
			'week',                         // unit name
			[1]                             // allowed multiples
		], [
			'month',
			[1, 2, 3, 4, 6]
		]];

		// create the chart
		$('#graph_container').highcharts('StockChart', {
			rangeSelector : {
				selected : 1
			},

			title : {
				text : str_symbol + ' Stock Price'
			},

		    yAxis: [{
		        title: {
		            text: 'OHLC'
		        },
		        height: 200,
		        lineWidth: 2
		    }, {
		        title: {
		            text: 'Volume'
		        },
		        top: 300,
		        height: 100,
		        offset: 0,
		        lineWidth: 2
		    }],

			series : [{
				id : 'dataseries',
				type : 'candlestick',
				name : str_symbol + ' Stock Price',
				data : prices,
		        dataGrouping: {
					units: groupingUnits
		        },
				threshold : null,
				tooltip: {
					valueDecimals: 2
				}
			},
			// volume
			{
		        type: 'column',
		        name: 'Volume',
		        data: volume,
		        yAxis: 1,
		        dataGrouping: {
					units: groupingUnits
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

	$.getJSON(query_prices, function(prices) {
		$.getJSON(query_volume, function(volume) {
			$.getJSON(query_news, function(news) {
				create_chart(prices, volume, news);
			});
		});
	});

});

		</script>
	</head>
	<body>
		<div id="graph_container" style="height: 500px; min-width: 500px"></div>
	</body>
</html>

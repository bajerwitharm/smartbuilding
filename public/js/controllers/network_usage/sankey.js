salwatorskaControllers.controller('networkUsageSankeyController', [
	'$scope',
	'$rootScope',
	'$filter',
	'orderByFilter',
	'databaseProvider',
	function($scope, $rootScope, $filter, orderByFilter, databaseProvider) {
	    var getConnectionsByAP = function() {
		databaseProvider.getConnectionsByAP().success(function(data) {
		    connectionsBy = data;
		    prepareConnectionsByAPChart();

		});
	    };

	    var connectionsBy;

	    var prepareConnectionsByAPChart = function() {
		var data = new google.visualization.DataTable();
		data.addColumn('string', 'From');
		data.addColumn('string', 'To');
		data.addColumn('number', 'Weight');
		connectionsBy.forEach(function(entry) {
		    data.addRow([ entry.name, entry.ap,
			    Number(entry.connections) / 1000 ]);
		});

		// Set chart options
		var options = {
		    // width : 100%,
		    height : 1000,
		    sankey: {
			      node: {
			        label: {
			          fontName: 'Times-Roman',
			          fontSize: 12,
			          color: '#000',
			          bold: true,
			          italic: false
			        },
			        labelPadding: 6, // Horizontal distance between the label and the node.
			        nodePadding: 10, // Vertical distance between nodes.
			        width: 6         // Thickness of the node.
			      }
			    }
		};

		// Instantiate and draw our chart, passing in some options.
		var chart = new google.visualization.Sankey(document
			.getElementById('sankey_basic'));
		chart.draw(data, options);
	    }
	    
	    getConnectionsByAP();
	    
	    $scope.changeSelectedView = function(view) {
		$scope.selectedView = view;
	    }
	    
	    $scope.selectedView='connections';
	} ]);
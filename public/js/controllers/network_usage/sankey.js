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
		var sankey = new Sankey();
		var data = new google.visualization.DataTable();
		var users = [];
		var data = [];
		$rootScope.filteredUsersInfo.forEach(function(entry) {
		    users.push(entry.name);
		});
	//	sankey.right_margin = 160;
	//	sankey.left_margin = 150;
		sankey.box_width = 10;
		sankey.stack(0, users);
		sankey.stack(1, [ "main@salwatorska6", "parter1@salwatorska6",
			"parter2@salwatorska6" ]);

		connectionsBy.forEach(function(entry) {
		    if (users.indexOf(entry.name) >= 0) {
			data.push([ entry.name, entry.connections / 30,
				entry.ap ])
		    }

		});


		sankey.setData(data);
		sankey.draw();
	    }

	    getConnectionsByAP();

	    $scope.changeSelectedView = function(view) {
		$scope.selectedView = view;
	    }

	    $scope.selectedView = 'connections';
	} ]);
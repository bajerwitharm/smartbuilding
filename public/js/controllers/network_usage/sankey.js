salwatorskaControllers.controller('networkUsageSankeyController', [
	'$scope',
	'$rootScope',
	'$filter',
	'orderByFilter',
	'databaseProvider',
	function($scope, $rootScope, $filter, orderByFilter, databaseProvider) {
	    var getConnectionsByAP = function() {
		databaseProvider.getConnectionsByAP().success(function(data) {
		    byAP = data;
		    prepareConnectionsByAPChart();

		});
	    };

	    var getUsageByAP = function() {
		databaseProvider.getUsageByAP().success(function(data) {
		    byAP = data;
		    prepareUsageByAPChart();
		});
	    };

	    var getByAP = function() {
		$("#sankey").empty();
		if ($scope.selectedView == 'connections')
		    getConnectionsByAP();
		if ($scope.selectedView == 'usage')
		    getUsageByAP();
	    };

	    var byAP;

	    var prepareConnectionsByAPChart = function() {
		var users = [];
		var data = [];
		var sankey = new Sankey();
		$rootScope.filteredUsersInfo.forEach(function(entry) {
		    users.push(entry.user_id);
		});
		sankey.box_width = 10;
		sankey.stack(0, users);
		sankey.stack(1, [ "main@salwatorska6", "parter1@salwatorska6",
			"parter2@salwatorska6" ]);

		sankey.convert_flow_values_callback = function(flow) {
		    return flow / 30; // Pixels per TWh
		};

		byAP.forEach(function(entry) {
		    if (users.indexOf(entry.user_id) >= 0) {
			data.push([ entry.user_id, entry.connections, entry.ap ])
		    }

		});

		sankey.convert_box_description_labels_callback = function(
			user_id) {
		    var found = $.grep($rootScope.filteredUsersInfo, function(e) {
			return e.user_id == user_id;
		    });
		    if (found[0]!=null) {
			return found[0].name;
		    }
		    else
		    {
			return user_id;
		    }
		};

		sankey.setData(data);
		sankey.draw();
	    }

	    var prepareUsageByAPChart = function() {
		var users = [];
		var data = [];
		sankey = new Sankey();
		$rootScope.filteredUsersInfo.forEach(function(entry) {
		    users.push(entry.user_id);
		});
		sankey.box_width = 10;
		sankey.stack(0, users);
		sankey.stack(1, [ "main@salwatorska6", "parter1@salwatorska6",
			"parter2@salwatorska6" ]);

		sankey.convert_flow_values_callback = function(flow) {
		    return flow / 1000000; // Pixels per TWh
		};

		sankey.convert_box_value_labels_callback = function(flow) {
		    return $filter('bytesFilter')(flow);
		};

		sankey.convert_flow_labels_callback = function(flow) {
		    return $filter('bytesFilter')(flow);
		};

		byAP.forEach(function(entry) {
		    if (users.indexOf(entry.user_id) >= 0) {
			data.push([ entry.user_id, entry.usage, entry.ap ])
		    }
		});

		sankey.convert_box_description_labels_callback = function(
			user_id) {
		    var found = $.grep($rootScope.filteredUsersInfo, function(e) {
			return e.user_id == user_id;
		    });
		    if (found[0]!=null) return found[0].name;
		    else
		    {
			return user_id;
		    }
		};

		sankey.setData(data);
		sankey.draw();
	    }

	    $scope.changeSelectedView = function(view) {
		$scope.selectedView = view;
		getByAP();
	    }

	    $scope.$on('networkUsersGetAllDataAgain', getByAP);

	    $scope.selectedView = 'usage';
	} ]);
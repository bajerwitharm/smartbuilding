salwatorskaControllers
	.controller(
		'networkUsageConnectionsChartController',
		[
			'$scope',
			'$rootScope',
			'$filter',
			'orderByFilter',
			'databaseProvider',
			function($scope, $rootScope, $filter, orderByFilter,
				databaseProvider) {
			    var chartToShow = "weekday";

			    var getConnectionsBy = function() {
				if ($scope.selectedView=='monthdays') getConnectionsByMonthday();
				if ($scope.selectedView=='weekdays') getConnectionsByWeekday();
				if ($scope.selectedView=='hours') getConnectionsByHour();
				if ($scope.selectedView=='months') getConnectionsByMonth();
			    };
			    
			    var getConnectionsByHour = function() {
				databaseProvider.getConnectionsByHour()
					.success(function(data) {
					    connectionsBy = data;
					    prepareConnectionsByHourChart();
					});
			    };

			    var getConnectionsByWeekday = function() {
				databaseProvider.getConnectionsByWeekday().success(
					function(data) {
					    connectionsBy = data;
					    prepareConnectionsByWeekdayChart();
					});
			    };
			    
			    var getConnectionsByMonthday = function() {
				databaseProvider.getConnectionsByMonthday().success(
					function(data) {
					    connectionsBy = data;
					    prepareConnectionsByMonthdayChart();
					});
			    };
			    
			    var getConnectionsByMonth = function() {
				databaseProvider.getConnectionsByMonth().success(
					function(data) {
					    connectionsBy = data;
					    prepareConnectionsByMonthChart();
					});
			    };
			    

			    var prepareConnectionsByHourChart = function() {

				$scope.connectionsByChart = [];

				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.connectionsByChart
					    .push({
						"key" : entry.name,
						"values" : [ [ 00, 0 ],
							[ 01, 0 ], [ 02, 0 ],
							[ 03, 0 ], [ 04, 0 ],
							[ 05, 0 ], [ 06, 0 ],
							[ 07, 0 ], [ 08, 0 ],
							[ 09, 0 ], [ 10, 0 ],
							[ 11, 0 ], [ 12, 0 ],
							[ 13, 0 ], [ 14, 0 ],
							[ 15, 0 ], [ 16, 0 ],
							[ 17, 0 ], [ 18, 0 ],
							[ 19, 0 ], [ 20, 0 ],
							[ 21, 0 ], [ 22, 0 ],
							[ 23, 0 ] ]
					    });
				});

				connectionsBy
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "name", entry.name);
					    if (element_index >= 0) {
						$scope.connectionsByChart[element_index].values[entry.hour] = [
							entry.hour,
							entry.connections ];
					    }
					});
			    }

			    var prepareConnectionsByWeekdayChart = function() {

				$scope.connectionsByChart = [];

				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.connectionsByChart.push({
					"key" : entry.name,
					"values" : [ [ 0, 0 ], [ 1, 0 ],
						[ 2, 0 ], [ 3, 0 ], [ 4, 0 ],
						[ 5, 0 ], [ 6, 0 ] ]
				    });
				});

				connectionsBy
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "name", entry.name);
					    if (element_index >= 0) {
						$scope.connectionsByChart[element_index].values[entry.weekday] = [
							entry.weekday,
							entry.connections ];
					    }
					});
			    }

			    var prepareConnectionsByMonthdayChart = function() {

				$scope.connectionsByChart = [];

				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.connectionsByChart.push({
					"key" : entry.name,
					"values" : [	[ 1, 0 ], [ 2, 0 ],
							[ 3, 0 ], [ 4, 0 ],
							[ 5, 0 ], [ 6, 0 ],
							[ 7, 0 ], [ 8, 0 ],
							[ 9, 0 ], [ 10, 0 ],
							[ 11, 0 ], [ 12, 0 ],
							[ 13, 0 ], [ 14, 0 ],
							[ 15, 0 ], [ 16, 0 ],
							[ 17, 0 ], [ 18, 0 ],
							[ 19, 0 ], [ 20, 0 ],
							[ 21, 0 ], [ 22, 0 ],
							[ 23, 0 ], [ 24, 0 ],
							[ 25, 0 ], [ 26, 0 ],
							[ 27, 0 ], [ 28, 0 ],
							[ 29, 0 ], [ 30, 0 ],
							[ 31, 0 ]]
				    });
				});
				

				
				connectionsBy
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "name", entry.name);
					    if (element_index >= 0) {
						$scope.connectionsByChart[element_index].values[Number(entry.monthday)-1] = [
							Number(entry.monthday),
							entry.connections ];
					    }
					});
			    }
			    
			    var prepareConnectionsByMonthChart = function() {

				$scope.connectionsByChart = [];

				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.connectionsByChart.push({
					"key" : entry.name,
					"values" : [	[ 1, 0 ], [ 2, 0 ],
							[ 3, 0 ], [ 4, 0 ],
							[ 5, 0 ], [ 6, 0 ],
							[ 7, 0 ], [ 8, 0 ],
							[ 9, 0 ], [ 10, 0 ],
							[ 11, 0 ], [ 12, 0 ]]
				    });
				});
				

				
				connectionsBy
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "name", entry.name);
					    if (element_index >= 0) {
						$scope.connectionsByChart[element_index].values[Number(entry.month)-1] = [
							Number(entry.month),
							entry.connections ];
					    }
					});
			    }

			    
			    $scope.xAxisTickFormat = function() {
				return function(d) {
				    weekdays = ['Niedziela','Poniedziałek','Wtorek','Środa','Czwartek','Piątek','Sobota'];
				    if ($scope.selectedView == 'weekdays') return weekdays[d];
				    return d;
				}
			    };

			    $scope.yAxisTickFormat = function() {
				return function(d) {
				    return Number(d).toFixed(0);
				}
			    };

			    $scope.toolTipContentFunction = function() {
				return function(key, x, y, e, graph) {
				    return '<h1>' + key + '</h1>' + '<p>'
					    + Number(y).toFixed(0)
					    + ' połączeń o ' + x + '</p>'
				}
			    };
			    
			    $scope.changeSelectedView = function(view) {
				$scope.selectedView = view;
				getConnectionsBy();
			    }
			    

			    $scope.$on('networkUsersGetAllDataAgain',
				    getConnectionsBy);

			} ])

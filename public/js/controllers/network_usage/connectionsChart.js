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
				if ($scope.selectedView=='weekdays') getConnectionsByDay();
				if ($scope.selectedView=='hours') getConnectionsByHour();
			    };
			    
			    var getConnectionsByHour = function() {
				databaseProvider.getConnectionsByHour()
					.success(function(data) {
					    connectionsByHour = data;
					    prepareConnectionsByHourChart();
					});
			    };

			    var getConnectionsByDay = function() {
				databaseProvider.getConnectionsByDay().success(
					function(data) {
					    connectionsByDay = data;
					    prepareConnectionsByDayChart();
					});
			    };

			    var prepareConnectionsByHourChart = function() {

				$scope.connectionsByChart = [];

				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.connectionsByChart
					    .push({
						"key" : entry.name,
						"values" : [ [ 0, 0 ],
							[ 1, 0 ], [ 2, 0 ],
							[ 3, 0 ], [ 4, 0 ],
							[ 5, 0 ], [ 6, 0 ],
							[ 7, 0 ], [ 8, 0 ],
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

				connectionsByHour
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

			    var prepareConnectionsByDayChart = function() {

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

				connectionsByDay
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

			    $scope.xAxisTickFormat = function() {
				return function(d) {
				    weekdays = ['Niedziela','Poniedziałek','Wtorek','Środa','Czwartek','Piątek','Sobota'];
				    if ($scope.selectedView == 'weekdays') return weekdays[d];
				    return d+":00";
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

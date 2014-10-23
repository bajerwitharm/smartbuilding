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
			    var emptyHourRecord = [ [ 0, 0 ], [ 1, 0 ],
				    [ 2, 0 ], [ 3, 0 ], [ 4, 0 ], [ 5, 0 ],
				    [ 6, 0 ], [ 7, 0 ], [ 8, 0 ], [ 09, 0 ],
				    [ 10, 0 ], [ 11, 0 ], [ 12, 0 ], [ 13, 0 ],
				    [ 14, 0 ], [ 15, 0 ], [ 16, 0 ], [ 17, 0 ],
				    [ 18, 0 ], [ 19, 0 ], [ 20, 0 ], [ 21, 0 ],
				    [ 22, 0 ], [ 23, 0 ] ];
			    var connectionsByHour = [];
			    var getConnectionsByHour = function() {
				databaseProvider
					.getConnectionsByHour()
					.success(
						function(data) {
						    connectionsByHour = data;
						    prepareConnectionsByHourChart();
						});
			    };

			    var prepareConnectionsByHourChart = function() {

				$scope.connectionsByHourChart = [];

				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.connectionsByHourChart.push({
					"key" : entry.name,
					"values" : [ [ 0, 0 ], [ 1, 0 ],
							    [ 2, 0 ], [ 3, 0 ], [ 4, 0 ], [ 5, 0 ],
							    [ 6, 0 ], [ 7, 0 ], [ 8, 0 ], [ 09, 0 ],
							    [ 10, 0 ], [ 11, 0 ], [ 12, 0 ], [ 13, 0 ],
							    [ 14, 0 ], [ 15, 0 ], [ 16, 0 ], [ 17, 0 ],
							    [ 18, 0 ], [ 19, 0 ], [ 20, 0 ], [ 21, 0 ],
							    [ 22, 0 ], [ 23, 0 ] ]
				    });
				});

				connectionsByHour.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "name", entry.name);
					    if (element_index >= 0) {
						$scope.connectionsByHourChart[element_index].values[entry.hour] = [
							entry.hour,
							entry.connections ];
					    }
					});
			    }
			    
			    $scope.xAxisTickFormat = function() {
				return function(d) {
				    return Number(d).toFixed(0);
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

			    $scope.$on('networkUsersGetAllDataAgain',
				    getConnectionsByHour);

			} ])

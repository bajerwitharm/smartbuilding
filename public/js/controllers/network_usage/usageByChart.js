salwatorskaControllers
	.controller(
		'networkUsageByChartController',
		[
			'$scope',
			'$rootScope',
			'$filter',
			'orderByFilter',
			'databaseProvider',
			function($scope, $rootScope, $filter, orderByFilter,
				databaseProvider) {

			    var usageByHour = [];
			    var emptyHourRecord = [ [ 0, 0 ], [ 1, 0 ],
				    [ 2, 0 ], [ 3, 0 ], [ 4, 0 ], [ 5, 0 ],
				    [ 6, 0 ], [ 7, 0 ], [ 8, 0 ], [ 09, 0 ],
				    [ 10, 0 ], [ 11, 0 ], [ 12, 0 ], [ 13, 0 ],
				    [ 14, 0 ], [ 15, 0 ], [ 16, 0 ], [ 17, 0 ],
				    [ 18, 0 ], [ 19, 0 ], [ 20, 0 ], [ 21, 0 ],
				    [ 22, 0 ], [ 23, 0 ] ];

			    var getUsageByHour = function() {
				databaseProvider.getUsageByHour().success(
					function(data) {
					    usageByHour = data;
					    prepareUsageByHourChart();
					}).error(function() {
				});
			    }

			    var prepareUsageByHourChart = function() {
				$scope.usageByHourChart = [];
				$rootScope.filteredUsersInfo.forEach(function(
					entry) {
				    $scope.usageByHourChart
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

				usageByHour
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "name", entry.name);
					    if (element_index >= 0) {
						$scope.usageByHourChart[element_index].values[Number(entry.hour)] = [
							Number(entry.hour),
							entry.data_in ];
					    }
					});
			    }

			    $scope.xAxisTickFormat = function() {
				return function(d) {
				    return d;
				}
			    };
			    
			    $scope.yAxisTickFormat = function() {
				return function(d) {
				    return $filter('bytesFilter')(d);
				}
			    };

			    $scope.toolTipContentFunction = function() {
				return function(key, x, y, e, graph) {
				    return '<h1>' + key + '</h1>' + '<p>'
					    + y
					    + ' danych o ' + x + '</p>'
				}
			    };

			    $scope.$on('networkUsersGetAllDataAgain',
				    getUsageByHour);

			} ]);
salwatorskaControllers.controller('networkUsageSummaryChartController',
	[
		'$scope',
		'$rootScope',
		'$filter',
		'orderByFilter',
		'databaseProvider',
		function($scope, $rootScope, $filter, orderByFilter,
			databaseProvider) {
		    var prepareUsageSummaryChart = function() {
			$scope.usageChart = [ {
			    "key" : "Dane ściągnięte",
			    "color" : "#FFEA00",
			    "values" : []
			}, {
			    "key" : "Dane wysłane",
			    "color" : "#4f99b4",
			    "values" : []
			} ];

			$rootScope.filteredUsersInfo.forEach(function(entry) {
			    if (entry.name == '') {
				entry.name = entry.accounts.split(',')[0];
			    }
			    $scope.usageChart[0].values.push([ entry.name,
				    entry.data_in ]);
			    $scope.usageChart[1].values.push([ entry.name,
				    entry.data_out ]);
			});
		    }

		    $scope.usageChartValueFormat = function() {
			return function(d) {
			    return $filter('bytesFilter')(d);
			}
		    }

		    $rootScope.$on('networkUsersGetAllDataAgain',
			    prepareUsageSummaryChart);

		} ]);
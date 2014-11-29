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
				if (entry.accounts!=null) {
				    entry.name = entry.accounts.split(',')[0];
				} else {
				    entry.name = '';
				}
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
		    
		    $scope.toolTipContentFunction = function(){
			return function(key, x, y, e, graph) {
		    	return '<h1>' + key + '</h1>' + '<p>' +  $filter('bytesFilter')(y.replace(/,/g,'')) + ' przez urządzenie ' + x +'<h1>Użyte hasła:</h1><p>' +  $rootScope.filteredUsersInfo[e.pointIndex].accounts+'</p>'
			}
		}

		    $rootScope.$on('networkUsersGetAllDataAgain',
			    prepareUsageSummaryChart);

		} ]);
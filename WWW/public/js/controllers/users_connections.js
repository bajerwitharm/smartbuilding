salwatorskaControllers
	.controller(
		'usersConnectionsController',
		[
			'$scope',
			'$rootScope',
			'$filter',
			'orderByFilter',
			'filterFilter',
			'filterFilter',
			'databaseProvider',

			function($scope, $rootScope, $filter, orderByFilter,
				listFilter, bytesFilter, databaseProvider) {
			    
			    $scope.filteredUsersInfo = [];
			    $scope.filteredConnectionsByHour = [];
			    $scope.filteredUsageByHour = [];

			    $scope.xAxisTickFormat = function() {
				return function(d) {
				    return d;
				}
			    };

			    $scope.toolTipContentFunction = function() {
				return function(key, x, y, e, graph) {
				    return '<h1>' + key + '</h1>' + '<p>'
					    + Number(y).toFixed(0)
					    + ' połączeń o ' + x + '</p>'
				}
			    };

			    $scope.prepareFilteredData = function(predicate,
				    reverse) {
				$scope.prepareFilteredUsersInfo(predicate,
					reverse);
				$scope.prepareFilteredConnectionsByHour(
					'data_in', true);
				$scope.prepareFilteredUsageByHour('data_in',
					true);
			    }

			    
			    $scope.getUsersInfo();
			    $scope.getConnectionsByHour();
			    $scope.getUsageByHour();



			    /* example Data taken from nvd3 example */

			} ]);

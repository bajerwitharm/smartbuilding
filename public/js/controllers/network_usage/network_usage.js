salwatorskaControllers.controller('networkUsageController', [
	'$scope',
	'$rootScope',
	'$filter',
	'orderByFilter',
	'databaseProvider',
	function($scope, $rootScope, $filter, orderByFilter,
		databaseProvider) {
	    $rootScope.filteredUsersInfo = [];
	    $rootScope.filterForm = {
	    };
	    
	    $rootScope.findItem = function(arr, key, value) {
		for (var i = 0; i < arr.length; i++) {
		    if (arr[i][key] === value) {
			return (i);
		    }
		}
		return (-1);
	    }
	    
	    $rootScope.prepareFilteredData = function(predicate,
		    reverse) {
		$rootScope.filterForm = $scope.filterForm;
		$rootScope.$broadcast('networkUsersRefreshFilter');
	    }

	    $scope.prepareFilteredData = $rootScope.prepareFilteredData;

	} ]);
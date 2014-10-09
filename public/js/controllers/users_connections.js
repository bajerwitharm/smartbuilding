salwatorskaControllers.controller('usersConnectionsController', [
	'$scope',
	'$rootScope',
	'$filter',
	'orderByFilter',
	'filterFilter',
	'databaseProvider',

	function($scope, $rootScope, $filter, orderByFilter, listFilter,
		databaseProvider) {
	    $scope.getUsersInfo = function() {
		databaseProvider.getUsersInfo().success(function(data) {
		    $scope.usersInfo = data;
		    $scope.prepareFilteredData();
		}).error(function() {
		});
	    };

	    $scope.filterForm = {
		filterText : ''
	    };

	    $scope.prepareFilteredData = function(predicate, reverse) {
		 $scope.filteredUsersInfo =
		 listFilter(orderByFilter($scope.usersInfo,
		 predicate, reverse),$scope.filterForm.filterText );
		//$scope.filteredUsersInfo = listFilter($scope.usersInfo,
		//	$scope.filterForm.filterText);
		$scope.barChart = [ {
		    "key" : "Dane ściągnięte",
		    "color" : "#FFEA00",
		    "values" : []
		}, {
		    "key" : "Dane wysłane",
		    "color" : "#4f99b4",
		    "values" : []
		} ];
		$scope.filteredUsersInfo.forEach(function(entry) {
		    $scope.barChart[0].values.push({
			label : entry.name,
			value : entry.data_in / 1048576
		    });
		    $scope.barChart[1].values.push({
			label : entry.name,
			value : entry.data_out / 1048576
		    });
		});
	    }

	    $scope.predicate = 'data_in';

	    $scope.getUsersInfo();

	    /* example Data taken from nvd3 example */

	} ]);

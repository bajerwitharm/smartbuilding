salwatorskaControllers.controller('networkUsersTableController', [
	'$scope',
	'$rootScope',
	'$filter',
	'orderByFilter',
	'databaseProvider',
	function($scope, $rootScope, $filter, orderByFilter, databaseProvider) {
	    $scope.predicate = 'data_in';
	    $scope.reverse = true;
	    var usersInfo = [];
	    var getUsersInfo = function() {
		databaseProvider.getUsersInfo().success(function(data) {
		    usersInfo = data;
		    $scope.prepareFilteredUsersInfo('data_in', true);
		}).error(function() {
		});
	    };

	    $scope.prepareFilteredUsersInfo = function(predicate, reverse) {
		$rootScope.filteredUsersInfo = $filter('filter')(orderByFilter(usersInfo, predicate, reverse),
			$rootScope.filterForm.filterText);

		$rootScope.$broadcast('networkUsersGetAllDataAgain');
	    };

	    $rootScope.$on('networkUsersRefreshFilter',function() {
		$scope.prepareFilteredUsersInfo($scope.predicate,$scope.reverse);
	    });

	    getUsersInfo();

	} ]);
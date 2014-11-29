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
		var allAndFilters = $rootScope.filterForm.filterText.split("&");
		var toFilter = orderByFilter(usersInfo, predicate, reverse);
		allAndFilters.forEach(function(singleAndTextFilter) {
		    var allOrFilters = singleAndTextFilter.split("|");
		    var filtered = [];
		    allOrFilters.forEach(function(singleTextFilter) {
			filtered=filtered.concat($filter('filter')(toFilter,singleTextFilter));
			});
		    toFilter = filtered;
		});
		$rootScope.filteredUsersInfo = toFilter;

		$rootScope.$broadcast('networkUsersGetAllDataAgain');
	    };

	    $rootScope.$on('networkUsersRefreshFilter', function() {
		$scope.prepareFilteredUsersInfo($scope.predicate,
			$scope.reverse);
	    });

	    getUsersInfo();

	} ]);
salwatorskaControllers.controller('networkUsersTableController', [
	'$scope',
	'$rootScope',
	'$filter',
	'orderByFilter',
	'databaseProvider',
	function($scope, $rootScope, $filter, orderByFilter, databaseProvider) {
	    var predicate = 'data_in';
	    var reverse = true;
	    var usersInfo = [];
	    var getUsersInfo = function() {
		databaseProvider.getUsersInfo().success(
			function(data) {
			    usersInfo = data;
			    $scope.prepareFilteredUsersInfo($scope.predicate,
				    $scope.reverse, "");
			}).error(function() {
		});
	    };

	    $scope.prepareFilteredUsersInfo = function(predicate, reverse,
		    filterText) {
		$rootScope.filteredUsersInfo = $filter('filter')(
			orderByFilter(usersInfo, predicate, reverse),
			filterText);

		$rootScope.$broadcast('networkUsersGetAllDataAgain');
	    };
	    
	    $rootScope.$on('networkUsersRefreshFilter',
		    getUsersInfo);

	    getUsersInfo();

	} ]);
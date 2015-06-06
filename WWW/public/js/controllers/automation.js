salwatorskaControllers.controller('automationController', [
	'$scope',
	'$rootScope',
	'databaseProvider',

	function($scope, $rootScope, databaseProvider) {
	    $http.get('/getRelayBoardInfo').success(
		    function(data, status, headers, config) {
			$scope.state = data
		    });
	} ])

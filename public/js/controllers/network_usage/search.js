salwatorskaControllers.controller('networkSearchController', [ '$scope',
	'$rootScope', '$filter', 'orderByFilter', 'databaseProvider',
	function($scope, $rootScope, $filter, orderByFilter, databaseProvider) {
    		$scope.setAllView = function(){
    		    $scope.selectedView = 'all';
    		}
    		$scope.setMonthsView = function(){
    		    $scope.selectedView = 'months';
    		}
    		$scope.selectedView = 'all';
	} ]);
salwatorskaControllers
	.controller(
		'eventsTableController',
		[
			'$scope',
			'$rootScope',
			'$filter',
			'orderByFilter',
			'databaseProvider',
			function($scope, $rootScope, $filter, orderByFilter,
				databaseProvider) {
			    
			    $scope.getUrlsLast = function() {
				databaseProvider.getUrlsLast().success(
					function(data) {
					    prepareUrlsLastTable(data);
					});
			    }
			    
			    var prepareUrlsLastTable = function(
				    urlsLast) {
				$scope.urlsLastTable = [];
				urlsLast
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "user_id", entry.user_id);
					    if (element_index >= 0) {
						$scope.urlsLastTable
							.push({
							    'timestamp' : entry.timestamp,
							    'host' : $rootScope.filteredUsersInfo[element_index].name,
							    'url' : entry.url
							});
					    }
					});
				$scope.selectedView = 'urls';
			    }
			    
			    $scope.getOtherLogsLast = function() {
				databaseProvider.getOtherLogsLast().success(
					function(data) {
					    $scope.otherLogsLastTable = data;
					    $scope.selectedView = 'others';
					});
			    }
			    
			    $scope.getConnectionsLast = function() {
				databaseProvider.getConnectionsLast().success(
					function(data) {
					    prepareConnectionsLastTable(data);
					});
			    };
			    var prepareConnectionsLastTable = function(
				    connectionsLast) {
				$scope.connectionsLastTable = [];
				connectionsLast
					.forEach(function(entry) {
					    var element_index = $rootScope
						    .findItem(
							    $rootScope.filteredUsersInfo,
							    "user_id", entry.user_id);
					    if (element_index >= 0) {
						$scope.connectionsLastTable
							.push({
							    'timestamp' : entry.timestamp,
							    'host' : $rootScope.filteredUsersInfo[element_index].name,
							    'account' : entry.account,
							    'ap' : entry.ap,
							    'result' : entry.result
							});
					    }
					});
				$scope.selectedView = 'connections';
			    }
			    $scope.getConnectionsLast();
			} ]);
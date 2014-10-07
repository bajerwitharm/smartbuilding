salwatorskaControllers.controller('usersConnectionsController', [ '$scope',
	'$rootScope', 'databaseProvider',

	function($scope, $rootScope, databaseProvider) {
	    $scope.getUsersInfo = function() {
		databaseProvider.getUsersInfo().success(function(data) {
		    $scope.usersInfo = data;
		    data.forEach(function(entry) {
			console.log(entry);
			$scope.barChart[0].values.push({
			    label : entry.name,
			    value : entry.data_in
			});
			$scope.barChart[1].values.push({
			    label : entry.name,
			    value : entry.data_out
			});
		    });

		}).error(function() {
		});
	    };
	    $scope.getUsersInfo();
	    /* example Data taken from nvd3 example */
	    $scope.barChart = [ {
		"key" : "Dane ściągnięte",
		"color" : "#FFEA00",
		"values" : []
	    }, {
		"key" : "Dane wysłane",
		"color" : "#4f99b4",
		"values" : []
	    } ];
	} ]);

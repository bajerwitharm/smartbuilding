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
		    $scope.prepareFilteredUsersInfo();
		}).error(function() {
		});
	    };
	    $scope.getConnectionsByHour = function() {
		databaseProvider.getConnectionsByHour().success(function(data) {
		    $scope.connectionsByHour = data;
		    $scope.prepareFilteredConnectionsByHour();
		}).error(function() {
		});
	    };
	    $scope.getUsageByHour = function() {
		databaseProvider.getUsageByHour().success(function(data) {
		    $scope.usageByHour = data;
		    $scope.prepareFilteredUsageByHour();
		}).error(function() {
		});
	    };

	    $scope.filterForm = {
		filterText : ''
	    };
	    
	    $scope.filteredUsersInfo = [];   
	    $scope.filteredConnectionsByHour = [];
	    $scope.filteredUsageByHour = [];
	    
	  
	    $scope.xAxisTickFormat = function() {
		return function(d) {
		    return d+":00";
		}
	    };

	    $scope.toolTipContentFunction = function() {
		return function(key, x, y, e, graph) {
                    return '<h1>' + key + '</h1>'+ 
                    '<p>' +  Number(y).toFixed(0) + ' połączeń o ' + x + '</p>'
		}
	    };
	    
	    $scope.prepareFilteredData = function(predicate, reverse) {
		$scope.prepareFilteredUsersInfo(predicate, reverse);
		$scope.prepareFilteredConnectionsByHour(predicate, reverse);
		$scope.prepareFilteredUsageByHour(predicate, reverse);
	    }

	    $scope.prepareFilteredUsersInfo = function(predicate, reverse) {
		$scope.filteredUsersInfo = listFilter(orderByFilter(
			$scope.usersInfo, predicate, reverse),
			$scope.filterForm.filterText);

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
	    
	    $scope.prepareFilteredConnectionsByHour = function(predicate, reverse) {
		$scope.filteredConnectionsByHour = listFilter(orderByFilter(
			$scope.connectionsByHour, predicate, reverse),
			$scope.filterForm.filterText);
		
		$scope.connectionsByHourChart=[];

		$scope.filteredConnectionsByHour.forEach(function(entry) {
		    var element_index = findItem($scope.connectionsByHourChart, "key", entry.name);
		    if (element_index<0) {
			$scope.connectionsByHourChart.push( {
			    "key" : entry.name,
			    "values" : [[0,0],[1,0],[2,0],[3,0],[4,0],[5,0],[6,0],[7,0],[8,0],[9,0],[10,0],[11,0],[12,0],[13,0],[14,0],[15,0],[16,0],[17,0],[18,0],[19,0],[20,0],[21,0],[22,0],[23,0]]
			});
			element_index = $scope.connectionsByHourChart.length-1;
		    }
		    {
			$scope.connectionsByHourChart[element_index].values[entry.hour]=[entry.hour,entry.connections];
		    }
		});
	    }
	    
	    $scope.prepareFilteredUsageByHour = function(predicate, reverse) {
		$scope.filteredUsageByHour = listFilter($scope.usageByHour,$scope.filterForm.filterText);
		
		$scope.usageByHourChart=[];

		$scope.filteredUsageByHour.forEach(function(entry) {
		    var element_index = findItem($scope.usageByHourChart, "key", entry.mac);
		    if (element_index<0) {
			$scope.usageByHourChart.push( {
			    "key" : entry.mac,
			    "values" : [[0,0],[1,0],[2,0],[3,0],[4,0],[5,0],[6,0],[7,0],[8,0],[09,0],[10,0],[11,0],[12,0],[13,0],[14,0],[15,0],[16,0],[17,0],[18,0],[19,0],[20,0],[21,0],[22,0],[23,0]]
			});
			element_index = $scope.usageByHourChart.length-1;
		    }
		    {
			$scope.usageByHourChart[element_index].values[Number(entry.hour)]=[Number(entry.hour),entry.data_in];
		    }
		});
	    }

	    $scope.predicate = 'data_in';

	    $scope.getUsersInfo();
	    $scope.getConnectionsByHour();
	    $scope.getUsageByHour();
	    
	    function findItem(arr, key, value) {
		    for (var i = 0; i < arr.length; i++) {
		       if (arr[i][key] === value) {
		           return(i);
		       }
		    }
		    return(-1);
		}

	    /* example Data taken from nvd3 example */

	} ]);

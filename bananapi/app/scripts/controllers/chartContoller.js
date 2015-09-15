'use strict';
/**
 * @ngdoc function
 * @name smartBuildingApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the smartBuildingApp
 */
angular.module('smartBuildingApp')
  .controller('ChartCtrl', ['$scope', '$timeout', 'mqttProvider', function ($scope, $timeout, mqttProvider) {
    $scope.temp1 = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.humidity1 = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.awsomes = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.Temperature = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.Humidity = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.Light = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.Sound = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    $scope.Dust = {
	    labels: [],
	    series: ['Series A'],
	    data: [
	      []
	    ],
        options: {
            animation: false
        },
	    onClick: function (points, evt) {
	      console.log(points, evt);
	    }
    };
    mqttProvider.on('mqtt',function(data) {
        if (data.topic == 'click_senzations/temp1') {
            $scope.temp1.data[0].push(parseInt(data.message));
            $scope.temp1.labels.push((new Date()).toLocaleTimeString());
            if ($scope.temp1.data[0].length > 8) {
               $scope.temp1.data[0].shift();
               $scope.temp1.labels.shift();
            }
        }
        if (data.topic == 'click_senzations/humidity1') {
            $scope.humidity1.data[0].push(parseInt(data.message));
            $scope.humidity1.labels.push((new Date()).toLocaleTimeString());
            if ($scope.humidity1.data[0].length > 8) {
               $scope.humidity1.data[0].shift();
               $scope.humidity1.labels.shift();
            }
        }
        if (data.topic == 'click_senzations/awesomes') {
            $scope.awsomes.data[0].push(parseInt(data.message));
            $scope.awsomes.labels.push((new Date()).toLocaleTimeString());
            if ($scope.awsomes.data[0].length > 20) {
               $scope.awsomes.data[0].shift();
               $scope.awsomes.labels.shift();
            }
        }

        if (data.topic == 'click_senzations/sensors') {
            var json = JSON.parse(data.message)
            $scope.Temperature.data[0].push(parseInt(json.Temperature));
            $scope.Temperature.labels.push((new Date()).toLocaleTimeString());
            if ($scope.Temperature.data[0].length > 8) {
               $scope.Temperature.data[0].shift();
               $scope.Temperature.labels.shift();
            }
            $scope.Humidity.data[0].push(parseInt(json.Humidity));
            $scope.Humidity.labels.push((new Date()).toLocaleTimeString());
            if ($scope.Humidity.data[0].length > 8) {
               $scope.Humidity.data[0].shift();
               $scope.Humidity.labels.shift();
            }
            $scope.Light.data[0].push((json.Light));
            $scope.Light.labels.push((new Date()).toLocaleTimeString());
            if ($scope.Light.data[0].length > 8) {
               $scope.Light.data[0].shift();
               $scope.Light.labels.shift();
            }
            $scope.Dust.data[0].push((json.Dust));
            $scope.Dust.labels.push((new Date()).toLocaleTimeString());
            if ($scope.Dust.data[0].length > 8) {
               $scope.Dust.data[0].shift();
               $scope.Dust.labels.shift();
            }
        }
    });


}]);
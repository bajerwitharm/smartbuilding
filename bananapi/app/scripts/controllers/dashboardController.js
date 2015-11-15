'use strict';
/**
 * @ngdoc function
 * @name smartBuildingApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the smartBuildingApp
 */
angular.module('smartBuildingApp')
    .controller('DashboardCtrl', ['$scope', '$timeout', 'mqttProvider' , function ($scope, $timeout, mqttProvider) {
        var subscriptionId;
        //mqttProvider.emit('subscribe', {topic: 'smartbuidling/firstfloor/status'});
        $scope.state = {};
        $scope.events = [];
        $scope.negate = function(name,topic) {
           mqttProvider.emit('salwatorska6/'+topic+'/control','{"header": {"start": 126,"source": 10,"destination": 11,"fc": 223,"size": 10},"actuator": {"output_toggle":{"'+name+'": true}},"crc":171}');
          };
        mqttProvider.connect(function() {
            subscriptionId = mqttProvider.subscribe('salwatorska6/#', function (data) {
                if (data.destinationName.endsWith('/status')) {
                    $scope.state = JSON.parse(data.payloadString).info;
                }
                if (data.destinationName.endsWith('/event')) {
                    $scope.events.push(JSON.parse(data.payloadString));
                }
            });
            $scope.$on("$destroy", function() {
                mqttProvider.unsubscribe(subscriptionId);
            });

        })

    }]);
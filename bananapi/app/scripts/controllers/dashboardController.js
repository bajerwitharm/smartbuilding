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
        $scope.negate = function(name) {
           mqttProvider.emit('smartbuidling/firstfloor/control','{"header": {"start": 126,"source": 10,"destination": 11,"fc": 223,"size": 10},"actuator": {"output_toggle":{"'+name+'": true}},"crc":171}');
          };
        mqttProvider.connect(function() {
            subscriptionId = mqttProvider.subscribe('smartbuidling/firstfloor/#', function (data) {
                switch(data.destinationName) {
                    case 'smartbuidling/firstfloor/status':
                        $scope.state = JSON.parse(data.payloadString).info;
                        break;
                    case 'smartbuidling/firstfloor/event':
                        $scope.events.push(JSON.parse(data.payloadString));
                        break;
                }
            });
            $scope.$on("$destroy", function() {
                mqttProvider.unsubscribe(subscriptionId);
            });

        })

    }]);
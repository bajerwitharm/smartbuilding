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
        $scope.state = {"inputs":{},"outputs":{},"states":{}};
        $scope.events = [];
        $scope.negate = function(name,topic) {
           mqttProvider.emit('salwatorska6/'+topic+'/control','{"header": {"start": 126,"source": 10,"destination": 11,"fc": 223,"size": 10},"actuator": {"output_toggle":{"'+name+'": true}},"crc":171}');
          };
        $scope.negate_state = function(name,topic) {
           mqttProvider.emit('salwatorska6/'+topic+'/state','{"header": {"start": 126,"source": 10,"destination": 11,"fc": 223,"size": 10},"actuator": {"state_toggle":{"'+name+'": true}},"crc":171}');
        };
        mqttProvider.connect(function() {
            subscriptionId = mqttProvider.subscribe('salwatorska6/#', function (data) {
                try {
                    if (data.destinationName.endsWith('/status')) {
                        jQuery.extend($scope.state.inputs,JSON.parse(data.payloadString).info.inputs);
                        jQuery.extend($scope.state.outputs,JSON.parse(data.payloadString).info.outputs);
                        jQuery.extend($scope.state.states,JSON.parse(data.payloadString).info.states);
                    }
                    if (data.destinationName.endsWith('/event')) {
                        $scope.events.push(JSON.parse(data.payloadString));
                    }
                } catch(err) {

                }

            });
            $scope.$on("$destroy", function() {
                mqttProvider.unsubscribe(subscriptionId);
            });

        })


    }]);
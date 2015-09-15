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
        $scope.state = {'outputs':{"bulb_2room":true}};
        $scope.negate = function(name) {
           mqttProvider.emit('smartbuidling/firstfloor/control','"actuator":{"output_toggle":{"'+name+'":true}}')
            $scope.state.outputs[name] = !($scope.state.outputs[name]);
        };
        mqttProvider.connect(function() {
            subscriptionId = mqttProvider.subscribe('smartbuidling/firstfloor/status', function (data) {
                console.log(data.payloadString);
                $scope.state = JSON.parse(data.payloadString).info;
            });
            $scope.$on("$destroy", function() {
                mqttProvider.unsubscribe(subscriptionId);
            });
        })

    }]);
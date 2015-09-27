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
        $scope.events = [];
        $scope.negate = function(name) {
           mqttProvider.emit('smartbuidling/firstfloor/control','{"output_toggle":{"'+name+'":true}}')
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
'use strict';
/**
 * @ngdoc function
 * @name smartBuildingApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the smartBuildingApp
 */
angular.module('smartBuildingApp')
    .controller('FormCtrl', function ($scope) {
        mqttProvider.on('mqtt', function (data) {
            console.log(data);
            if (data.topic == 'click_senzations/relayEvent') {
                console.log(data);
            }
        });

    });
'use strict';

/**
 * @ngdoc directive
 * @name izzyposWebApp.directive:adminPosHeader
 * @description
 * # adminPosHeader
 */
angular.module('smartBuildingApp')
	.directive('secondfloor',function() {
    return {
        templateUrl:'scripts/directives/secondfloor/secondfloor.html',
        restrict: 'E',
        scope: {
        'state': '=',
        'negate': '&'
  		}
    }
  });

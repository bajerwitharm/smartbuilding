'use strict';

/**
 * @ngdoc directive
 * @name izzyposWebApp.directive:adminPosHeader
 * @description
 * # adminPosHeader
 */
angular.module('smartBuildingApp')
	.directive('groundfloor',function() {
    return {
        templateUrl:'scripts/directives/groundfloor/groundfloor.html',
        restrict: 'E',
        scope: {
        'state': '=',
        'negate': '&'
  		}
    }
  });

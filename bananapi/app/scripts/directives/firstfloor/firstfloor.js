'use strict';

/**
 * @ngdoc directive
 * @name izzyposWebApp.directive:adminPosHeader
 * @description
 * # adminPosHeader
 */
angular.module('smartBuildingApp')
	.directive('firstfloor',function() {
    return {
        templateUrl:'scripts/directives/firstfloor/firstfloor.html',
        restrict: 'E',
        scope: {
        'state': '=',
  		}
    }
  });

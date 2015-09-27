'use strict';

angular.module('smartBuildingApp')
	.directive('notifications',function(){
		return {
            templateUrl:'scripts/directives/notifications/notifications.html',
            restrict: 'E',
            scope: {
                'events': '='
  		    },
            controller: function($scope) {
                  $scope.stringToColour = function(str) {
                    if (str!=null) {
                        var hval = 0x811c9dc5;
                        for (var i = 0; i < str.length;
                            hval = hval ^ (str.charCodeAt(i++) & 0xFF),
                            hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24));
                        for (var i = 0, colour = "#"; i < 3; colour += ("00" + ((hval >> i++ * 8) & 0xFF).toString(16)).slice(-2));
                        return "background-color:"+colour;
                    }
                    return "";
                  }
            }
    	}
	});


angular.module('smartBuildingApp')
    .directive('longPress', function ($timeout) {
        return {
            restrict: 'A',
            link: function (scope, element, attrs, ctrl) {
                var timeout = null
                var longPress = false

                element.bind('mousedown', function (e) {
                    longPress = false;
                    timeout = $timeout(function () {
                        scope.$apply(function () {
                            scope.$eval(attrs.longPress)
                            longPress = true;
                        });
                    }, 600)
                });
                element.bind('mouseup', function (e) {
                    $timeout.cancel(timeout);
                    if (!longPress) {
                        scope.$apply(function () {
                            scope.$eval(attrs.shortPress)
                            timeout = null
                        });
                    }
                })
            }

        }
    });
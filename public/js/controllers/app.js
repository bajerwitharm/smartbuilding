var salwatorska_app = angular.module('salwatorska_app', [ 'ngRoute',
	'ngResource', 'ng-nvd3', 'nvd3ChartDirectives', 'salwatorskaControllers', 'onFinishRender', 'filters','gantt', 'angulartics', 'angulartics.google.analytics' ]);

salwatorska_app.config([ '$routeProvider', function($routeProvider, $analyticsProvider) {

    $routeProvider.when('/main', {
	templateUrl : '/partials/main/main.html',
	controller : 'mainController'
    }).when('/vwalk', {
	templateUrl : 'partials/vwalk/vwalk.html',
	controller : 'vwalkController'
    }).when('/camera', {
	templateUrl : 'partials/camera/camera.html',
	controller : 'cameraController'
    }).when('/users_connections', {
	templateUrl : 'partials/network/users_connections.html',
	controller : 'networkUsageController'
    }).otherwise({
	redirectTo : '/main'
    });
} ]);

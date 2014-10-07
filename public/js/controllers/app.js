var salwatorska_app = angular.module('salwatorska_app', [ 'ngRoute',
	'ngResource', 'ng-nvd3', 'salwatorskaControllers', 'onFinishRender' ]);

salwatorska_app.config([ '$routeProvider', function($routeProvider) {

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
	templateUrl : 'partials/users_connections/users_connections.html',
	controller : 'usersConnectionsController'
    }).otherwise({
	redirectTo : '/main'
    });
} ]);

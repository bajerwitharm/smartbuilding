var salwatorska_app = angular.module('salwatorska_app', [ 'ngRoute', 'ngResource', 'salwatorskaControllers','onFinishRender' ]);

salwatorska_app.config([ '$routeProvider', function($routeProvider) {

	$routeProvider.when('/partials/main', {
		templateUrl : '/partials/main/main.html',
		controller : 'mainController'
	}).when('/partials/vwalk', {
		templateUrl : 'partials/vwalk/vwalk.html',
		controller : 'vwalkController'
	}).when('/partials/camera', {
		templateUrl : 'partials/camera/camera.html',
		controller : 'cameraController'
	}).otherwise({
		redirectTo : '/partials/main'
	});
} ]);

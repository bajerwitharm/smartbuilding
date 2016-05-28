smartBuildingApp.factory('logService', ['$rootScope', function ($rootScope) {
    return {
        info: function (message) {
            console.log(message);
        },
        warning: function (message) {
            console.log(message);
        },
        error: function (message) {
            console.log(message);
        }
    }
}]);
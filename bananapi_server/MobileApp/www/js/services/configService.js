smartBuildingApp.factory('configService', ['$rootScope', function ($rootScope) {
    return {
        getMqttUser: function () {
            return "Administracja";
        },
        getMqttPass: function () {
            return "Administrator@main6";
        },
        getMqttBrokerUrl: function () {
            return "mqtt.salwatorska.pl";
        },
        getMqttBrokerPort: function () {
            return 8000;
        }
    }
}]);
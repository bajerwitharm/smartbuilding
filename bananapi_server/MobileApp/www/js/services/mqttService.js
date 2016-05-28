smartBuildingApp.factory('mqttProvider', ['$rootScope','logService','configService', function ($rootScope, logService, configService) {

    var wsbroker = configService.getMqttBrokerUrl();  //mqtt websocket enabled broker
    var wsport = configService.getMqttBrokerPort();

    var callbacks = [];
    var connected = false;

    var client = new Paho.MQTT.Client(wsbroker, wsport,"mobileAppClientid_" + parseInt(Math.random() * 100, 10));

    client.onConnectionLost = function (responseObject) {
        logService.warning("Connection lost: " + responseObject.errorMessage);
    };

    function isInTopic(element, index, array) {
        subscribed = element[0].split('#');
        destination = this.destinationName;
        subscribed.forEach(function (element) {
            if (destination.indexOf(element) == -1) return false
        })
        return true;
    };

    client.onMessageArrived = function (message) {
        callbacks.filter(isInTopic, message).forEach(
            function (element) {
                var args = arguments;
                element[1](message);
            }
        );
    };

    return {
        emit: function (topic, payload, callback) {
            client.send(topic, payload);
        },
        connect: function (callback) {
            var options = {
                useSSL: false,
                timeout: 3,
                onSuccess: function () {
                    connected = true;
                    logService.info("Mqtt connected");
                    var args = arguments;
                    $rootScope.$apply(function () {
                        callback.apply(client, args);
                    });
                },
                onFailure: function (message) {
                    connected = false;
                    logService.error("Connection failed: " + message.errorMessage);
                },
                mqttVersion: 3,
                userName: configService.getMqttUser(),
                password: configService.getMqttPass()
            };
            if (!connected) {
                client.connect(options);
            } else {
                callback();
            }
        },
        subscribe: function (topic, callback) {
            client.subscribe(topic, { qos: 1 });
            callbacks.push([topic, function () {
                var args = arguments;
                $rootScope.$apply(function () {
                    callback.apply(client, args);
                });
            }]);
            return (callback.length - 1)
        },
        unsubscribe: function (id) {
            client.unsubscribe(callbacks[id][0]);
            callbacks.splice(id, 1);
        }
    }

}])

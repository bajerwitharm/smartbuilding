smartBuildingApp.factory('mqttProvider', ['$rootScope',function($rootScope) {

    //sample HTML/JS script that will publish/subscribe to topics in the Google Chrome Console
    //by Matthew Bordignon @bordignon on twitter.

    var wsbroker = "192.168.0.102";  //mqtt websocket enabled broker
    var wsport = 8080 // port for above

    var callbacks = [];
    var connected = false;

    var client = new Paho.MQTT.Client(wsbroker, wsport,
        "myclientid_" + parseInt(Math.random() * 100, 10));

    client.onConnectionLost = function (responseObject) {
      console.log("connection lost: " + responseObject.errorMessage);
    };

    function isInTopic(element, index, array) {
        subscribed = element[0].split('#');
        destination = this.destinationName;
        subscribed.forEach(function(element) {
            if (destination.indexOf(element) == -1) return false
        })
        return true;
    };

    client.onMessageArrived = function (message) {
      console.log(message.destinationName, ' -- ', message.payloadString);
      callbacks.filter(isInTopic,message).forEach(
          function(element) {
              var args = arguments;
              element[1](message);
          }
      );
    };

	return {
        emit : function(topic, payload, callback) {
	        client.send(topic, payload);
		},
        connect : function(callback) {
            var options = {
            timeout: 3,
            onSuccess: function () {
                connected = true;
                console.log("mqtt connected");
                var args = arguments;
                $rootScope.$apply(function() {
					callback.apply(client, args);
			    });
            },
            onFailure: function (message) {
                connected = false;
                console.log("Connection failed: " + message.errorMessage);
            },
			mqttVersion: 3
            };
            if (!connected) {
              client.connect(options);
            } else {
                callback();
            }
        },
        subscribe : function(topic, callback) {
            client.subscribe(topic, {qos: 1});
            callbacks.push([topic,function() {
				var args = arguments;
				$rootScope.$apply(function() {
					callback.apply(client, args);
				});
			}]);
            return (callback.length-1)
		},
        unsubscribe : function(id) {
            client.unsubscribe(callbacks[id][0]);
            callbacks.splice(id,1);
        }
    }

}])

/*smartBuildingApp.factory('mqttProvider', ['$rootScope',function($rootScope) {
	var socket = io.connect();
	return {
		on : function(eventName, callback) {
			socket.on(eventName, function() {
				var args = arguments;
				$rootScope.$apply(function() {
					callback.apply(socket, args);
				});
			});
		},
		emit : function(eventName, data, callback) {
			socket.emit(eventName, data, function() {
				var args = arguments;
				$rootScope.$apply(function() {
					if (callback) {
						callback.apply(socket, args);
					}
				});
			})
		},
		send : function(eventName) {
			console.log(eventName);
			socket.send(eventName);
		}
	};

}]);*/

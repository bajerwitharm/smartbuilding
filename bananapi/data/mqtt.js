var mqtt = require('mqtt');
var mqtt_client = mqtt.connect('mqtt://192.168.0.102');
var database;
var socket;

module.exports.init = function (db) {
    database = db;
}

module.exports.relayEvent = function (data) {
    mqtt_client.publish("smartbuidling/firstfloor/status", data, {'qos':1,'retain':true}, function () {
    });

}

module.exports.relayControl = function (callback) {
    mqtt_client.subscribe("smartbuidling/firstfloor/control")
    mqtt_client.on('message', function (topic, message) {
        callback(message.toString());
        database.insertNewEvent(topic,message, function (result){
            mqtt_client.publish("smartbuidling/firstfloor/events", JSON.stringify(result[0].insert_event), {'qos':1,'retain':true}, function () {});
        });
    });
}


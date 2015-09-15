var mqtt = require('mqtt');
var mqtt_client = mqtt.connect();
var database;
var socket;

module.exports.connect = function (db, soc) {
    database = db;
    socket = soc;
    socket.on('connection', function (socket) {
        socket.on('subscribe', function (data) {
            socket.join(data.topic);
            socket.on('mqtt_event', function (data) {
                console.log('event: '+data.topic)
                mqtt_client.publish(data.topic, data.payload, {'qos':1,'retain':true});
            })
            console.log('subscription:'+data.topic)
            mqtt_client.subscribe(data.topic);
        });
    });
}

mqtt_client.on('message', function (topic, payload) {
    console.log(topic);
    socket.in(topic).emit('mqtt_event',{'topic':String(topic),'payload':String(payload)});
});

module.exports.relayEvent = function (data) {
    console.log("Trying to publish");
    mqtt_client.publish("smartbuidling/firstfloor/status", data, {'qos':1,'retain':true}, function () {
        console.log("Message is published");
    });
}


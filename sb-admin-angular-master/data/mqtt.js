var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://test.mosquitto.org');
var database;

module.exports.connect = function(db) {
	database = db;
	client.on('connect', function() {
		console.log('Connected');
		client.subscribe('nullo/#');
	});
}

var extractPlace = function(topic, device) {
	return topic.replace("/"+device,"");
}


client.on('message', function(topic, message) {
	try {
		dht_data = JSON.parse(message.toString());
		database.insertDHTRecord(extractPlace(topic,'dht11'),dht_data.temp,dht_data.humidity);
	}
	catch(err) {
		console.log("Error processing MQTT message:"+message);		
	}
});



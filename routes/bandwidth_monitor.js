var database;
var Client = require('node-rest-client').Client;
var restClient = new Client();
    
module.exports.initBandwidthMonitor = function(db) {
	database=db;
	setInterval(function() {
		restClient.get('http://192.168.1.1/cgi-bin/usage', function(data) {
			database.insertNewUsage(data);
		})
    }, 900000);
}


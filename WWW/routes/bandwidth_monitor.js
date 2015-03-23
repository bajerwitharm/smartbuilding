var database;
var Client = require('node-rest-client').Client;
var restClient = new Client();
var router_ip = "192.168.1.1";
var path_to_usage = "/cgi-bin/usage"
var check_frequency = 90000;	// 15 minutes
    
module.exports.initBandwidthMonitor = function(db) {
	database=db;
	setInterval(function() {
		//restClient.get('http://'+router_ip+path_to_usage, function(data) {
			//database.insertNewUsage(data);
		//})
    }, check_frequency);
}


/*
 * 
 */

var database;
var Client = require('node-rest-client').Client;
var restClient = new Client();
var async = require('async');
var wan_interface = "eth0.3";

module.exports.initStatsMonitor = function(db, ip) {
    database = db;
    ip_addr = ip;
    setInterval(function() {
	login(function(auth, stok) {
		getStats(auth, stok, function(stats){
		    database.insertNewStats(stats);
		});
	});
    }, 60000);
}

/*
 * login = function(callback) { var args = { data : { jsonrpc : "1.0", method :
 * "login", params : { "username" : "root", "password" : "x" }, id : 0 } };
 * restClient.post('http://192.168.1.1/cgi-bin/luci/rpc/auth', args,
 * function(data, response) { callback(data.result); } ); }
 */

/*
 * Login to remote system and get session auth cookie and token
 * 
 * Calls callback after geting response from remote system
 */
login = function(callback) {
    var args = {
	data : "username=root&password=Administrator%40main6",
	headers : {
	    "Content-Type" : "application/x-www-form-urlencoded"
	}
    };
    restClient.post('http://' + ip_addr + '/cgi-bin/luci', args, function(data,
	    response) {
	var auth = response.headers['set-cookie'][0];
	var stok = response.headers['set-cookie'][0].split('stok=')[1]
		.split(";")[0]
	callback(auth, stok);
    });
}

/*
 * Network traffic
 * 
 * Returned data format: time, rx bytes, rx packets, tx bytes, tx packets
 * 
 * Data sample: [ [1358095466, 1482617413, 101161697, 1420821587, 62541131],
 * [1358095467, 1482626173, 101161703, 1420830431, 62541137], ... ]
 */
getNetworkTraffic = function(callback, auth, stok) {
    var args = {
	headers : {
	    "Cookie" : auth
	}
    };
    restClient.get('http://' + ip_addr + '/cgi-bin/luci/;stok=' + stok
	    + '/admin/status/realtime/bandwidth_status/' + wan_interface, args,
	    function(data) {
		callback(null, data);
	    })
}

/*
 * System load
 * 
 * Returned data format: time, 1min load, 5min load, 15min load
 * 
 * Data sample: [ [1358098335, 36, 40, 56], [1358098336, 36, 40, 56], ... ]
 */
getSystemLoad = function(callback, auth, stok) {
    var args = {
	headers : {
	    "Cookie" : auth
	}
    };
    restClient.get('http://' + ip_addr + '/cgi-bin/luci/;stok=' + stok
	    + '/admin/status/realtime/load_status', args, function(data) {
	callback(null, data);
    })
}

/*
 * Returned keys ["conncount", "leases", "wan", "membuffers", "connmax",
 * "memfree", "uptime", "wifinets", "memtotal", "localtime", "memcached",
 * "loadavg" ]
 */
getSystemStatus = function(callback, auth, stok) {
    var args = {
	headers : {
	    "Cookie" : auth
	}
    };
    restClient.get('http://' + ip_addr + '/cgi-bin/luci/;stok=' + stok
	    + '/admin/status?status=1', args, function(data) {
	callback(null, data);
    })
}

getStats = function(auth, stok, callback) {
    async.series({
	traffic : function(callback) {
	    getNetworkTraffic(callback, auth, stok);
	},
	load : function(callback) {
	    getSystemLoad(callback, auth, stok);
	},
	//status : function(callback) {
	//    getSystemStatus(callback, auth, stok);
	//}
    }, function(err, stats) {
	
	callback(stats);	
    });
}

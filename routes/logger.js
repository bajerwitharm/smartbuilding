var database;

var parseLogEntry = function(logentry) {
	switch(logentry.program) {
	case "dnsmasq":
		database.insertNewQuery(logentry);
		break;
	case "dnsmasq-dhcp":
		database.insertNewUser(logentry);
		break;
	case "radiusd":
		database.insertNewConnection(logentry);
		break;	
	default:
		database.insertNewLogEntry(logentry);
		break;
	}
}
    
module.exports.initLogger = function(db) {
	database=db;
	var datagram;
	var server = require('net').createServer(function (stream) {
        stream.setEncoding('utf8');
 //       stream.addListener("data", function (slice) {
        	//datagram=datagram+slice;
        //});
        stream.addListener("data", function (slice) {
        	datagram = datagram + slice;
        	if (slice.length<3472){
        		var logentries = JSON.parse("["+slice.slice(0,-1)+"]");
        		database.startTransaction();
                for (var logentry in logentries) {
                  logentry = logentries[logentry];
                  if (logentry.length == 0) {
                    continue;
                  }
                  parseLogEntry(logentry);
                 	        		
                };
                database.endTransaction();
                datagram = "";
        	}        	
        });
      }); 
    server.listen(514, '0.0.0.0');
}


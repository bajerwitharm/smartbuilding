var database;

var parseLogEntry = function(logentry) {
	if (logentry.program == "dnsmasq")
	{
		database.insertNewQuery(logentry);
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
        	var logentries = JSON.parse("["+slice.slice(0,-1)+"]")
            for (var logentry in logentries) {
              logentry = logentries[logentry];
              if (logentry.length == 0) {
                continue;
              }
             database.insertNewLogEntry(logentry,parseLogEntry(logentry));
            }
        });
      }); 
    server.listen(514, '0.0.0.0');
}


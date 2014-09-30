var sqlite3 = require('sqlite3').verbose();
var fs = require('fs');
var sqliteDbContext = new sqlite3.Database('database.db');

module.exports.getDbContext = function () {
    return sqliteDbContext;
};

module.exports.createDatabase = function () {
	var query = readQuery("CreateDatabase")
    console.log(query);
	sqliteDbContext.serialize(function() {
		sqliteDbContext.run(query);
	});
};

module.exports.insertNewUsage = function (row,callback) {
	var query = format(readQuery("InsertNewUsage"), 
			row.mac, 
			row.data_in, 
			row.data_out
			);
	sqliteDbContext.serialize(function() {
		return sqliteDbContext.run(query,callback);
    });
};

module.exports.insertNewLogEntry = function (row,callback) {
	var query = format(readQuery("InsertNewLogEntry"), 
			row.timestamp, 
			row.host, 
			row.program,
			row.msg
			);
	console.log(query);
	sqliteDbContext.serialize(function() {
		return sqliteDbContext.run(query,callback);
    });
};

module.exports.insertNewUser = function (row,callback) {
	var messageparts = row.msg.split(" ");
	var name = messageparts[4];
	var mac = messageparts[3];
	var ip = messageparts[2];
	var bridge = messageparts[1].slice(-8,-1);
	
	var query = format(readQuery("InsertNewUser"), 
			row.timestamp, 
			name, 
			mac,
			ip,
			bridge
			);
	console.log(query);
	sqliteDbContext.serialize(function() {
		return sqliteDbContext.run(query,callback);
    });
};

module.exports.insertNewConnection = function (row,callback) {
//	var regex = new RegExp(".{6}.{2,9}\:.{2}.{1,20}\/.{36}[^\s]*.*\s*\)");
//	var array = regex.exec(row.msg);
	var message = row.msg;
    var result = message.substring(6,message.indexOf(":"));
    var account = message.substring(message.indexOf("[")+1,message.indexOf("/"));
    var ap = message.substring(message.indexOf("client")+7,message.indexOf(" port"));
    var mac = message.substring(message.indexOf("cli ")+4,message.indexOf(")"));
	var query = format(readQuery("InsertNewConnection"), 
			row.timestamp, 
			result, 
			account,
			ap,
			mac
			);
	console.log(query);
	sqliteDbContext.serialize(function() {
		return sqliteDbContext.run(query,callback);
    });
};

module.exports.insertNewQuery = function (row,callback) {
	var messageparts = row.msg.split(" ");
	var query = format(readQuery("InsertNewQuery"), 
			row.timestamp, 
			messageparts[1], 
			messageparts[3]
	);
	console.log(query);
	sqliteDbContext.serialize(function() {
		return sqliteDbContext.run(query,callback);
    });
};

function readQuery(queryName) {
    return fs.readFileSync("./data/queries/"+queryName, 'utf-8');
};

function format(str){
    for(var i = 0; i < arguments.length; i++) {
        var markerToBeReplaced = '{' + (i) + '}';
        str = str.replace(markerToBeReplaced, arguments[i]);
    }
    return str;
}

process.on('SIGTERM', function () {
    console.log('Closing database');
    sqliteDbContext.close();
    console.log('Database has been closed');
});
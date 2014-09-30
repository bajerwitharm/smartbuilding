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
var sqlite3 = require('sqlite3').verbose();
var fs = require('fs');
var sqliteDbContext = new sqlite3.Database('database.db');

module.exports.getDbContext = function() {
    return sqliteDbContext;
};

module.exports.createDatabase = function(callback) {
    var query = readQuery("CreateDatabase")
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, callback);
    });
};

module.exports.startTransaction = function(callback) {
    var query = readQuery("StartTransaction")
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, callback);
    });
};

module.exports.endTransaction = function(callback) {
    var query = readQuery("EndTransaction")
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, callback);
    });
};

module.exports.insertNewUsage = function(rows, callback) {
    var queries = "";
    for (row in rows) {
	var query = format(readQuery("InsertNewUsage"), rows[row].mac,
		rows[row].data_in, rows[row].data_out);
	queries = queries + query;
    }
    sqliteDbContext.serialize(function() {
	return sqliteDbContext.exec(queries, callback);
    });
};

module.exports.insertNewLogEntry = function(row, callback) {
    var query = format(readQuery("InsertNewLogEntry"), row.timestamp, row.host,
	    row.program, row.msg);
    sqliteDbContext.serialize(function() {
	return sqliteDbContext.run(query, callback);
    });
};

module.exports.insertNewUser = function(row, callback) {
    var messageparts = row.msg.split(" ");
    var name = messageparts[4];
    var mac = messageparts[3];
    var ip = messageparts[2];
    var bridge = messageparts[1].slice(-9, -1);

    var query = format(readQuery("InsertNewUser"), row.timestamp, name, mac,
	    ip, bridge);
    // console.log(query);
    sqliteDbContext.serialize(function() {
	return sqliteDbContext.exec(query, callback);
    });
};

module.exports.insertNewConnection = function(row, callback) {
    // var regex = new RegExp(".{6}.{2,9}\:.{2}.{1,20}\/.{36}[^\s]*.*\s*\)");
    // var array = regex.exec(row.msg);
    var message = row.msg;
    var result = message.substring(6, message.indexOf(":"));
    var account = message.substring(message.indexOf("[") + 1, message
	    .indexOf("/<"));
    var ap = message.substring(message.indexOf("client") + 7, message
	    .indexOf(" port"));
    var mac = message.substring(message.indexOf("cli ") + 4,
	    message.indexOf(")")).replace(/-/g, ":").toLowerCase();
    var query = format(readQuery("InsertNewConnection"), row.timestamp, result,
	    account, ap, mac);
    sqliteDbContext.serialize(function() {
	return sqliteDbContext.run(query, callback);
    });
};

module.exports.insertNewQuery = function(row, callback) {
    var messageparts = row.msg.split(" ");
    var query = format(readQuery("InsertNewQuery"), row.timestamp,
	    messageparts[1], messageparts[3]);
    sqliteDbContext.serialize(function() {
	return sqliteDbContext.run(query, callback);
    });
};

module.exports.insertNewStats = function(stats, callback) {
    var queries = "";
    for (second in stats.traffic) {
	var query = format(readQuery("InsertNewStats"), stats.load[second][0], // timestamp
	"0", // router name
	stats.traffic[second][1],// rx bytes
	stats.traffic[second][2],// rx packets
	stats.traffic[second][3],// tx bytes
	stats.traffic[second][4],// tx packets
	stats.load[second][1]// 1 minute load
	);
	// console.log(query);
	queries = queries + query;
    }
    sqliteDbContext.serialize(function() {
	return sqliteDbContext.exec(queries, callback);
    });
};

module.exports.getUsersInfo = function(callback) {
    var query = readQuery("GetUsersInfo");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getConnectionsByHour = function(callback) {
    var query = readQuery("GetConnectionsByHour");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getConnectionsByWeekday = function(callback) {
    var query = readQuery("GetConnectionsByWeekday");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getConnectionsByMonthday = function(callback) {
    var query = readQuery("GetConnectionsByMonthday");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getConnectionsByMonth = function(callback) {
    var query = readQuery("GetConnectionsByMonth");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getConnectionsByAP = function(callback) {
    var query = readQuery("GetConnectionsByAP");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getUsageByHour = function(callback) {
    var query = readQuery("InsertNewUsagePerHour");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, function(err, rows) {
	    query = readQuery("GetUsageByHour");
	    sqliteDbContext.all(query, function(err, rows) {
		callback(rows);
	    });
	});
    });
};

module.exports.getUsageByWeekday = function(callback) {
    var query = readQuery("InsertNewUsagePerWeekday");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, function(err, rows) {
	    query = readQuery("GetUsageByWeekday");
	    sqliteDbContext.all(query, function(err, rows) {
		callback(rows);
	    });
	});
    });
};

module.exports.getUsageByMonthday = function(callback) {
    var query = readQuery("InsertNewUsagePerMonthday");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, function(err, rows) {
	    query = readQuery("GetUsageByMonthday");
	    sqliteDbContext.all(query, function(err, rows) {
		callback(rows);
	    });
	});
    });
};

module.exports.getUsageByMonth = function(callback) {
    var query = readQuery("InsertNewUsagePerMonth");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, function(err, rows) {
	    query = readQuery("GetUsageByMonth");
	    sqliteDbContext.all(query, function(err, rows) {
		callback(rows);
	    });
	});
    });
};

module.exports.getUsageByAP = function(callback) {
    var query = readQuery("InsertNewUsagePerAP");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, function(err, rows) {
	    query = readQuery("GetUsageByAP");
	    sqliteDbContext.all(query, function(err, rows) {
		callback(rows);
	    });
	});
    });
};

module.exports.getConnectionsInTime = function(callback) {
    var query = readQuery("InsertNewConnectionsInTime");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.exec(query, function(err, rows) {
	    query = readQuery("GetConnectionsInTime");
	    sqliteDbContext.all(query, function(err, rows) {
		callback(rows);
	    });
	});
    });
};

module.exports.getConnectionsLast = function(callback) {
    var query = readQuery("GetConnectionsLast");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getUrlsLast = function(callback) {
    var query = readQuery("GetUrlsLast");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

module.exports.getOtherLogsLast = function(callback) {
    var query = readQuery("GetOtherLogsLast");
    sqliteDbContext.serialize(function() {
	sqliteDbContext.all(query, function(err, rows) {
	    callback(rows);
	});
    });
};

function readQuery(queryName) {
    return fs.readFileSync("./data/queries/" + queryName, 'utf-8');
};

function escapeRegExp(string) {
    return string.replace(/([.*+?^=!:${}()|\[\]\/\\])/g, "\\$1");
};

function format(str) {
    for (var i = 0; i < arguments.length; i++) {
	var markerToBeReplaced = escapeRegExp('{' + (i) + '}');
	str = str.replace(new RegExp(markerToBeReplaced, 'g'), arguments[i]);
    }
    return str;
}

process.on('SIGTERM', function() {
    console.log('Closing database');
    sqliteDbContext.close();
    console.log('Database has been closed');
});
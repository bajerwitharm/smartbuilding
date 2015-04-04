var mysql = require('mysql');
var fs = require('fs');
var sqliteDbContext = mysql.createConnection({
    host : '192.168.2.101',
    user : 'salwatorska',
    password : 'Administracja6',
    database : 'SMART_HOME',
    multipleStatements : true
});

sqliteDbContext.connect(function(err) {
    if (err) {
	console.error('Error connecting to database: ' + err.stack);
	return;
    }
});

module.exports.getDbContext = function() {
    return sqliteDbContext;
};

module.exports.createDatabase = function() {
    var query = readQuery("CreateDatabase")
    sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result);
    });
    query = readQuery("CreateTables")
    sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result);
    });
};

module.exports.startTransaction = function(callback) {
    var query = readQuery("StartTransaction")
    sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result, callback);
    });
};

module.exports.endTransaction = function(callback) {
    var query = readQuery("EndTransaction")
    sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result, callback);
    });
};

module.exports.insertNewUsage = function(rows, callback) {
    var queries = "";
    for (row in rows) {
	var query = format(readQuery("InsertNewUsage"), rows[row].mac,
		rows[row].data_in, rows[row].data_out);
	queries = queries + query;
    }
    console.log(queries);
    return sqliteDbContext.query(queries, function(err, result) {
    });
};

module.exports.insertNewLogEntry = function(row, callback) {
    var query = format(readQuery("InsertNewLogEntry"), row.timestamp, row.host,
	    row.program, row.msg);
    return sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result, callback);
    });
};

module.exports.insertNewUser = function(row, callback) {
    var messageparts = row.msg.split(" ");
    var name = messageparts[4];
    var mac = messageparts[3];
    var ip = messageparts[2];
    var bridge = messageparts[1].substring(messageparts[1].lastIndexOf("("+1),
	    messageparts[1].indexOf(")"));

    var query = format(readQuery("InsertNewUser"), row.timestamp, name, mac,
	    ip, bridge);
    console.log(query);
    return sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result, callback);
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
    return sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result, callback);
    });
};

module.exports.insertNewQuery = function(row, callback) {
    var messageparts = row.msg.split(" ");
    var query = format(readQuery("InsertNewQuery"), row.timestamp,
	    messageparts[1], messageparts[3]);
    return sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result, callback);
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
    return sqliteDbContext.query(queries, function(err, result) {
	executeQuery(err, result, callback);
    });
};

module.exports.getUsersInfo = function(callback, query) {
    var conditionUsage = prepareTimeCondition(query, 'where', 'usage_in_time');
    var conditionConnections = prepareTimeCondition(query, 'where',
	    'connections');
    var sqlquery = format(readQuery("GetUsersInfo"), conditionUsage,
	    conditionConnections);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	executeQuery(err, rows, callback);
	;
    });
};

module.exports.getConnectionsByHour = function(callback, query) {
    var condition = prepareTimeCondition(query, 'and', 'connections');
    var sqlquery = format(readQuery("GetConnectionsByHour"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	executeQuery(err, rows, callback);
	;
    });
};

module.exports.getConnectionsByWeekday = function(callback, query) {
    var condition = prepareTimeCondition(query, 'and', 'connections');
    var sqlquery = format(readQuery("GetConnectionsByWeekday"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	if (callback) {
	    callback(rows);
	}
    });
};

module.exports.getConnectionsByMonthday = function(callback, query) {
    var condition = prepareTimeCondition(query, 'and', 'connections');
    var sqlquery = format(readQuery("GetConnectionsByMonthday"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	if (callback) {
	    callback(rows);
	}
    });
};

module.exports.getConnectionsByMonth = function(callback, query) {
    var condition = prepareTimeCondition(query, 'and', 'connections');
    var sqlquery = format(readQuery("GetConnectionsByMonth"), condition);
    sqliteDbContext.serialize(function() {
	sqliteDbContext.query(sqlquery, function(err, rows) {
	    if (callback) {
		callback(rows);
	    }
	});
    });
};

module.exports.getConnectionsByAP = function(callback, query) {
    var condition = prepareTimeCondition(query, 'where', 'connections');
    var sqlquery = format(readQuery("GetConnectionsByAP"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	if (callback) {
	    callback(rows);
	}
    });
};

module.exports.getUsageByHour = function(callback, query) {
    var sqlquery = readQuery("InsertNewUsagePerHour");
    sqliteDbContext.query(sqlquery, function(err, rows) {
	var condition = prepareTimeCondition(query, 'where', 'usage_by_hour');
	sqlquery = format(readQuery("GetUsageByHour"), condition);
	sqliteDbContext.query(sqlquery, function(err, rows) {
	    if (callback) {
		callback(rows);
	    }
	});
    });
};

module.exports.getUsageByWeekday = function(callback, query) {
    var sqlquery = readQuery("InsertNewUsagePerWeekday");
    sqliteDbContext.query(sqlquery,
	    function(err, rows) {
		var condition = prepareTimeCondition(query, 'where',
			'usage_by_weekday');
		sqlquery = format(readQuery("GetUsageByWeekday"), condition);
		sqliteDbContext.query(sqlquery, function(err, rows) {
		    executeQuery(err, rows, callback);
		    ;
		});
	    });
};

module.exports.getUsageByMonthday = function(callback, query) {
    var sqlquery = readQuery("InsertNewUsagePerMonthday");
    sqliteDbContext.query(sqlquery, function(err, rows) {
	var condition = prepareTimeCondition(query, 'where',
		'usage_by_monthday');
	sqlquery = format(readQuery("GetUsageByMonthday"), condition);
	sqliteDbContext.query(sqlquery, function(err, rows) {
	    executeQuery(err, rows, callback);
	    ;
	});
    });
};

module.exports.getUsageByMonth = function(callback, query) {
    var sqlquery = readQuery("InsertNewUsagePerMonth");
    sqliteDbContext.query(sqlquery, function(err, rows) {
	var condition = prepareTimeCondition(query, 'where', 'usage_by_month');
	sqlquery = format(readQuery("GetUsageByMonth"), condition);
	sqliteDbContext.query(sqlquery, function(err, rows) {
	    executeQuery(err, rows, callback);
	    ;
	});
    });
};

module.exports.getUsageByAP = function(callback, query) {
    var sqlquery = readQuery("InsertNewUsagePerAP");
    sqliteDbContext.query(sqlquery, function(err, rows) {
	var condition = prepareTimeCondition(query, 'where', 'usage_by_ap');
	sqlquery = format(readQuery("GetUsageByAP"), condition);
	sqliteDbContext.query(sqlquery, function(err, rows) {
	    executeQuery(err, rows, callback);
	    ;
	});
    });
};

module.exports.getConnectionsInTime = function(callback, query) {
    var sqlquery = readQuery("InsertNewConnectionsInTime");
    sqliteDbContext.query(sqlquery, function(err, rows) {
	var condition = prepareTimeCondition(query, 'where',
		'connections_in_time').replace('timestamp', 'start_timestamp')
		+ " "
		+ prepareTimeCondition(query, 'and', 'connections_in_time')
			.replace('timestamp', 'end_timestamp');
	sqlquery = format(readQuery("GetConnectionsInTime"), condition);
	sqliteDbContext.query(sqlquery, function(err, rows) {
	    executeQuery(err, rows, callback);
	    ;
	});
    });
};

module.exports.getConnectionsLast = function(callback, query) {
    var condition = prepareTimeCondition(query, 'where', 'connections');
    var sqlquery = format(readQuery("GetConnectionsLast"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	executeQuery(err, rows, callback);
	;
    });
};

module.exports.getUrlsLast = function(callback, query) {
    var condition = prepareTimeCondition(query, 'where', 'queries');
    var sqlquery = format(readQuery("GetUrlsLast"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	executeQuery(err, rows, callback);
	;
    });
};

module.exports.getOtherLogsLast = function(callback, query) {
    var condition = prepareTimeCondition(query, 'where', 'logs');
    var sqlquery = format(readQuery("GetOtherLogsLast"), condition);
    sqliteDbContext.query(sqlquery, function(err, rows) {
	executeQuery(err, rows, callback);
	;
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

function executeQuery(err, result, callback) {
    if (err) {
	console.error(err.stack)
    } else {
	if (callback) callback(result);
    }
}

var prepareTimeCondition = function(query, junction, table) {
    var condition = "";
    if (table != null) {
	table = junction + " " + table + '.';
    } else {
	table = junction;
    }
    if ((query.from != null) && (query.till != null)) {
	condition = table + "timestamp between '" + query.from + "' and '"
		+ query.till + "'";
    } else if (query.from != null) {
	condition = table + "timestamp>='" + query.from + "'";
    } else if (query.till != null) {
	condition = table + "timestamp<='" + query.till + "'";
    }
    return condition;
}

process.on('SIGTERM', function() {
    console.log('Closing database');
    sqliteDbContext.end();
    console.log('Database has been closed');
});

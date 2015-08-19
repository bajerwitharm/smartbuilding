var pg = require('pg');
var fs = require('fs');

var conString = "postgres://postgres:postgres@localhost/smart_home";

var sqliteDbContext = new pg.Client(conString);

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

module.exports.insertDHTRecord = function(place,temperature,humidity) {
    var query = format(readQuery("InsertDHTRecord"), place, temperature, humidity);
    sqliteDbContext.query(query, function(err, result) {
	executeQuery(err, result);
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
	console.error(err)
    } else {
	if (callback) callback(result.rows);
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

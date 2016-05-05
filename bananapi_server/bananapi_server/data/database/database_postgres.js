var pg = require('pg');
var fs = require('fs');

var conString = "postgres://postgres:postgres@localhost/smart_building";
var conStringDbCreation = "postgres://postgres:postgres@localhost/template1";

var sqliteDbContext;

module.exports.getDbContext = function () {
    return sqliteDbContext;
};

function createDatabase(callback) {
    var tmpDbContext = new pg.Client(conStringDbCreation);
    tmpDbContext.connect(function (err) {
        if (err) {
            console.log('Error connecting to database: ' + err.stack);
            return;
        }
    });
    var query = readQuery("CreateDatabase");
    tmpDbContext.query(query, function (err, result) {
        executeQuery(err, result, function () {
            callback();
            tmpDbContext.end();
        });
    });    
}

function createTables() {
    query = readQuery("CreateTables")
    sqliteDbContext.query(query, function (err, result) {
        executeQuery(err, result);
    });
}

function openDbContext() {
    sqliteDbContext = new pg.Client(conString);
    sqliteDbContext.connect(function (err) {
        if (err) {
            console.log('Error connecting to database: ' + err.stack);
            return;
        }
    });
}

module.exports.init = function () {
    createDatabase(function () {
        openDbContext();
        createTables();
    });
}

module.exports.insertNewEvent = function (topic, content, callback) {

    var query = format(readQuery("InsertNewEventType"), topic);
    sqliteDbContext.query(query, function (err, result) {
        executeQuery(err, result, function () {
            query = format(readQuery("InsertNewEvent"), topic, content);
            sqliteDbContext.query(query, function (err, result) {
                executeQuery(err, result, callback);
            });
        });
    });
};

function readQuery(queryName) {
    return fs.readFileSync("./data/database/queries/" + queryName, 'utf-8');
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
        console.log(err)
    } else {
        if (callback) callback(result.rows);
    }
}

var prepareTimeCondition = function (query, junction, table) {
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

process.on('SIGTERM', function () {
    console.log('Closing database');
    sqliteDbContext.end();
    console.log('Database has been closed');
});


/**
 * Module dependencies.
 */
var express = require('express')
  , bodyParser = require('body-parser')
  , http = require('http')
  , logger = require('morgan')
  , methodOverride = require('method-override')
  , errorHandler = require('errorhandler')
  , path = require('path');


var database = require('./data/database_postgres.js');//change to database_sqlite.js if needed
database.createDatabase();

var mqtt = require('./data/mqtt.js');
mqtt.connect(database);


var app = express();
app.use(methodOverride());
app.use(bodyParser.urlencoded({limit:'50mb'}));
app.use(bodyParser());
app.use('/bower_components',  express.static(__dirname + '/bower_components'));
app.set('PORT', process.env.PORT || 3000);

// development only
console.log(app.get('env'));
if ('development' == app.get('env')) {
  app.use(logger('dev'));
  app.use(errorHandler());
  app.use(express.static(path.join(__dirname, '/app')));
  console.log('Running node.js in development mode');
}
else {
	app.use(express.static(path.join(__dirname, '/dist')));	
	console.log('Running node.js in distribution mode');
}

http.createServer(app).listen(app.get('PORT'), '0.0.0.0', function(){
    // Find out which user used sudo through the environment variable
    var uid = parseInt(process.env.SUDO_UID);
    // Set our server's uid to that user
    if (uid) process.setuid(uid);
    console.log('Express server listening on port ' + app.get('PORT'));
});

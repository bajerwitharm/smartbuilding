
/**
 * Module dependencies.
 */

var express = require('express')
  , bodyParser = require('body-parser')
  , routes = require('./routes')
  , user = require('./routes/user')
  , http = require('http')
  , logger = require('morgan')
  , methodOverride = require('method-override')
  , errorHandler = require('errorhandler')
  , path = require('path');

var KameraWejscie = require('./routes/foscam.js');
var KameraParter = require('./routes/acti.js');
var sqliteDbContext = require('./data/database_postgres.js');//change to database_sqlite.js if needed
var database = require('./routes/database.js');
var logParser = require('./routes/logger.js');
var onvifCameras = require('./routes/onvif.js'); 
var bandwidthMonitor = require('./routes/bandwidth_monitor.js');
var statsMonitor = require('./routes/stats_monitor.js');
//var relayBoard = require('./routes/relay_board.js');

var app = express();

app.use(methodOverride());
 
app.use(logger('dev'));
 
app.use(bodyParser.urlencoded({limit:'50mb'}));

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('title','www.salwatorska.pl');
//app.use(express.favicon());
app.use(logger('dev'));
app.use(bodyParser());
app.use(methodOverride());
//app.use(app.router);
app.use(express.static(path.join(__dirname, '/public')));

// development only
if ('development' == app.get('env')) {
  app.use(errorHandler());
}


app.get('/users', user.list);
app.get('/recordKameraWejscie', KameraWejscie.recordCamera);
app.get('/recordKameraParter', KameraParter.recordCamera);
app.get('/getLiveKameraWejscie', KameraWejscie.getLiveCamera);
app.get('/getLiveKameraParter', KameraParter.getLiveCamera);
app.get('/getLiveKameraPietro1', onvifCameras.getLiveCamera);
app.get('/getLiveKameraPietro2', onvifCameras.getLiveCamera2);
app.get('/getUsersInfo', database.getUsersInfo);
app.get('/getConnectionsByHour', database.getConnectionsByHour);
app.get('/getConnectionsByWeekday', database.getConnectionsByWeekday);
app.get('/getConnectionsByMonthday', database.getConnectionsByMonthday);
app.get('/getConnectionsByMonth', database.getConnectionsByMonth);
app.get('/getConnectionsByAP', database.getConnectionsByAP);
app.get('/getConnectionsInTime', database.getConnectionsInTime);
app.get('/getConnectionsLast', database.getConnectionsLast);
app.get('/getUrlsLast', database.getUrlsLast);
app.get('/getOtherLogsLast', database.getOtherLogsLast);
app.get('/getUsageByHour', database.getUsageByHour);
app.get('/getUsageByWeekday', database.getUsageByWeekday);
app.get('/getUsageByMonthday', database.getUsageByMonthday);
app.get('/getUsageByMonth', database.getUsageByMonth);
app.get('/getUsageByAP', database.getUsageByAP);
//app.get('/getRelayBoardInfo', relayBoard.requestInfo);
//app.get('/writeTelegram', relayBoard.writeTelegram);

http.createServer(app).listen(app.get('port'), '0.0.0.0', function(){
    // Find out which user used sudo through the environment variable
    var uid = parseInt(process.env.SUDO_UID);
    // Set our server's uid to that user
    if (uid) process.setuid(uid);
    // console.log('Server\'s UID is now ' + process.getuid());
    console.log('Express server listening on port ' + app.get('port'));
});

sqliteDbContext.createDatabase();
logParser.initLogger(sqliteDbContext);
onvifCameras.initOnvif(sqliteDbContext);
bandwidthMonitor.initBandwidthMonitor(sqliteDbContext);
//statsMonitor.initStatsMonitor(sqliteDbContext,"192.168.1.1");
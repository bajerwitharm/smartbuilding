
/**
 * Module dependencies.
 */

var express = require('express')
  , routes = require('./routes')
  , user = require('./routes/user')
  , http = require('http')
  , path = require('path');

var ipcamera = require('./routes/foscam.js');
var ipcamera2 = require('./routes/acti.js');
var sqliteDbContext = require('./data/database.js');
var database = require('./routes/database.js');
var logParser = require('./routes/logger.js');
var bandwidthMonitor = require('./routes/bandwidth_monitor.js');
var statsMonitor = require('./routes/stats_monitor.js');

var app = express();



// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(app.router);
app.use(express.static(path.join(__dirname, '/public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/users', user.list);
app.get('/recordCamera', ipcamera.recordCamera);
app.get('/recordCamera2', ipcamera2.recordCamera)
app.get('/getLiveCamera', ipcamera.getLiveCamera);
app.get('/getLiveCamera2', ipcamera2.getLiveCamera);
app.get('/getUsersInfo', database.getUsersInfo);
app.get('/getConnectionsByHour', database.getConnectionsByHour);
app.get('/getConnectionsByWeekday', database.getConnectionsByWeekday);
app.get('/getConnectionsByMonthday', database.getConnectionsByMonthday);
app.get('/getConnectionsByMonth', database.getConnectionsByMonth);
app.get('/getConnectionsByAP', database.getConnectionsByAP);
app.get('/getUsageByHour', database.getUsageByHour);
app.get('/getUsageByWeekday', database.getUsageByWeekday);
app.get('/getUsageByMonthday', database.getUsageByMonthday);
app.get('/getUsageByMonth', database.getUsageByMonth);
app.get('/getUsageByAP', database.getUsageByAP);

http.createServer(app).listen(app.get('port'), '0.0.0.0', function(){
  console.log('Express server listening on port ' + app.get('port'));
});

sqliteDbContext.createDatabase();
logParser.initLogger(sqliteDbContext);
bandwidthMonitor.initBandwidthMonitor(sqliteDbContext);
//statsMonitor.initStatsMonitor(sqliteDbContext,"192.168.1.1");
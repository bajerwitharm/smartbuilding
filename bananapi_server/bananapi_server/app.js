
var express = require('express')
    , bodyParser = require('body-parser')
    , http = require('http')
    , logger = require('morgan')
    , methodOverride = require('method-override')
    , errorHandler = require('errorhandler')
    , path = require('path')
    , RED = require("node-red");


var Database = require('./data/database/database_postgres.js'); //change to database_sqlite.js if needed
var database = new Database();
//database.createDatabase();

var Mqtt = require('./data/mqtt.js');
var mqtt = new Mqtt(database);

//var OnvifCameras = require('./routes/onvif.js');
//var onvifCameras = OnvifCameras(mqtt);

var Authorization = require('./routes/auth')
var authorization = new Authorization(database);

var Middleware = require('./middlewares/validateRequest');
var middleware = Middleware(authorization.validateUser);

var app = express();
app.use(methodOverride());
app.use(bodyParser.urlencoded({ limit: '50mb' }));
app.use(bodyParser.json());
//app.all('/*', function (req, res, next) {
//    // CORS headers
//    res.header("Access-Control-Allow-Origin", "*"); // restrict it to the required domain
//    res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE,OPTIONS');
//    // Set custom headers for CORS
//    res.header('Access-Control-Allow-Headers', 'Content-type,Accept,X-Access-Token,X-Key');
//    if (req.method == 'OPTIONS') {
//        res.status(200).end();
//    } else {
//        next();
//    }
//});
app.post('/login', authorization.login);
// Auth Middleware - This will check if the token is valid
// Only the requests that start with /api/v1/* will be checked for the token.
// Any URL's that do not follow the below pattern should be avoided unless you 
// are sure that authentication is not needed
app.all('/api/v1/*', middleware.validate);

app.use('/', require('./routes'));
 
app.set('PORT', 3000);

if ('development' == app.get('env')) {
    app.use(logger('dev'));
    app.use(errorHandler());
    //app.use(express.static(path.join(__dirname, '/app')));
    console.log('Running node.js in development mode');
}
else {
    //app.use(express.static(path.join(__dirname, '/dist')));
    //console.log('Running node.js in distribution mode');
};






instance = http.createServer(app).listen(app.get('PORT'), '0.0.0.0', function () {
    var uid = parseInt(process.env.SUDO_UID);
    if (uid) process.setuid(uid);
    console.log('Express server listening on port ' + app.get('PORT'));
});

var isWin64 = process.env.hasOwnProperty('ProgramFiles(x86)');

var settings;

if (isWin64) {
    settings = {
        httpAdminRoot: "/red",
        httpNodeRoot: "/api",
        userDir: "c:\\temp\\nodered",
        functionGlobalContext: {}    // enables global context
    };
} else {
    settings = {
        httpAdminRoot: "/red",
        httpNodeRoot: "/api",
        userDir: "/root/.nodered/",
        functionGlobalContext: {}    // enables global context
    };
}

// Initialise the runtime with a server and settings
RED.init(instance, settings);

// Serve the editor UI from /red
app.use(settings.httpAdminRoot, RED.httpAdmin);

// Serve the http nodes UI from /api
app.use(settings.httpNodeRoot, RED.httpNode);

// If no route is matched by now, it must be a 404
app.use(function (req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});
RED.start();










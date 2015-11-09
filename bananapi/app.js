/**
 * Module dependencies.
 */
var express = require('express')
    , bodyParser = require('body-parser')
    , http = require('http')
    , logger = require('morgan')
    , methodOverride = require('method-override')
    , errorHandler = require('errorhandler')
    , path = require('path')
    , passport = require('passport')
    , LocalStrategy = require('passport-local').Strategy;
var isWin64 = process.env.hasOwnProperty('ProgramFiles(x86)');

// Define the strategy to be used by PassportJS
passport.use(new LocalStrategy(
    function (username, password, done) {
        if (username === "admin" && password === "admin") // stupid example
            return done(null, {name: "admin"});

        return done(null, false, { message: 'Incorrect username.' });
    }
));

// Serialized and deserialized methods when got from session
passport.serializeUser(function (user, done) {
    done(null, user);
});

passport.deserializeUser(function (user, done) {
    done(null, user);
});

// Define a middleware function to be used for every secured routes
var auth = function (req, res, next) {
    if (!req.isAuthenticated())
        res.send(401);
    else
        next();
};

var database = require('./data/database_postgres.js');//change to database_sqlite.js if needed
database.createDatabase();

var app = express();
app.use(methodOverride());
app.use(bodyParser.urlencoded({limit: '50mb'}));
app.use(bodyParser());
app.use('/bower_components', express.static(__dirname + '/bower_components'));
app.set('PORT', process.env.PORT || 3000);
app.use(passport.initialize());
app.use(passport.session());

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
;

var db_routes = require('./routes/db_routes.js');
db_routes.init(database);
app.get('/getAvgValues', db_routes.getAvgValues);


instance = http.createServer(app).listen(app.get('PORT'), '0.0.0.0', function () {
    // Find out which user used sudo through the environment variable
    var uid = parseInt(process.env.SUDO_UID);
    // Set our server's uid to that user
    if (uid) process.setuid(uid);
    console.log('Express server listening on port ' + app.get('PORT'));
});

var mqtt = require('./data/mqtt.js');
mqtt.init(database);

var RED = require("node-red");

// Create the settings object - see default settings.js file for other options

var settings;
if (isWin64) {
    settings = {
        httpAdminRoot: "/red",
        httpNodeRoot: "/api",
        userDir: "c:\\temp\\nodered",
        functionGlobalContext: { }    // enables global context
    };
} else {
    settings = {
        httpAdminRoot: "/red",
        httpNodeRoot: "/api",
        userDir: "/root/.nodered/",
        functionGlobalContext: { }    // enables global context
    };
}

// Initialise the runtime with a server and settings
RED.init(instance, settings);


// Serve the editor UI from /red
app.use(settings.httpAdminRoot, RED.httpAdmin);

// Serve the http nodes UI from /api
app.use(settings.httpNodeRoot, RED.httpNode);

RED.start();

console.log('Windows?:' + isWin64);

var relayBoard = require('./routes/relay_board.js');
relayBoard.init(mqtt);


app.get('/loggedin', function (req, res) {
    res.send(req.isAuthenticated() ? req.user : '0');
});

app.post('/login', passport.authenticate('local'), function (req, res) {
    res.send(req.user);
});

app.post('/logout', function (req, res) {
    req.logOut();
    res.send(200);
});

app.get('/getRelayBoardInfo', relayBoard.requestInfo);

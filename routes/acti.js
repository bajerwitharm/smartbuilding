var spawn = require('child_process').spawn;
var exec = require('child_process').exec;

var numberOfClients = 0;
var ffmpeg;
var EventEmitter = require('events').EventEmitter
var app = new EventEmitter;

module.exports = app

// defaults
app.settings = {
    host : '192.168.1.52',
    port : 81,
    user : 'admin',
    pass : '123456'
};

module.exports.recordCamera = function(req, res) {
    exec("ffmpeg -r 5 -t 60 -i 'http://"
		    + app.settings.host
		    + ":"
		    + app.settings.port
		    + "/cgi-bin/encoder?USER="
		    + app.settings.user
		    + "&PWD="
		    + app.settings.pass
		    + "&GET_STREAM' -acodec copy -vcodec mpeg4 -preset slow /home/salwatorska/`date +%#F_%H.%M.%S`BramaWejsciowa2.avi",
	    function puts(error, stdout, stderr) {
	    });
    res.send('done');
    console
	    .log("ffmpeg -r 5 -t 60 -i 'http://"
		    + app.settings.host
		    + ":"
		    + app.settings.port
		    + "/cgi-bin/encoder?USER="
		    + app.settings.user
		    + "&PWD="
		    + app.settings.pass
		    + "&GET_STREAM' -acodec copy -vcodec mpeg4 -preset slow /home/salwatorska/`date +%#F_%H.%M.%S`BramaWejsciowa2.avi");
}

module.exports.getLiveCamera = function(req, res) {
    if (numberOfClients == 0) {
	ffmpeg = spawn("ffmpeg", [
		'-i',
		"http://" + app.settings.host + ":" + app.settings.port
			+ "/cgi-bin/encoder?USER=" + app.settings.user
			+ "&PWD=" + app.settings.pass + "&GET_STREAM", '-f',
		'mpeg4', '-vcodec', 'mpeg4', '-acodec', 'copy', '-tune',
		'zerolatency', '-fflags', 'nobuffer', 'pipe:1' ]);
	console.log("http://" + app.settings.host + ":" + app.settings.port
		+ "/cgi-bin/encoder?USER=" + app.settings.user + "&PWD="
		+ app.settings.pass + "&GET_STREAM");
    }
    ;
    numberOfClients++;
    res.writeHead(200, {
	'Transfer-Encoding' : 'chunked',
	'Content-Type' : 'video/webm'
    });
    req.on('close', function(code) {
	numberOfClients--;
	if (numberOfClients == 0) {
	    ffmpeg.kill();
	}
    });
    req.setTimeout(20000, function(code) {
	numberOfClients--;
	if (numberOfClients == 0) {
	    ffmpeg.kill();
	}
    });
    ffmpeg.stdout.pipe(res);
}
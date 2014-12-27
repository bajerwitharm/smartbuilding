var spawn = require('child_process').spawn;
var exec = require('child_process').exec;

var numberOfClients = 0;
var ffmpeg;
var EventEmitter = require('events').EventEmitter
var app = new EventEmitter;

module.exports = app

// defaults
app.settings = {
    host : 'KameraParter.salwatorska6',
    port : 81,
    user : 'viewer',
    pass : 'viewer123',
    name : 'KameraParter'
};

module.exports.recordCamera = function(req, res) {
    exec("ffmpeg -i rtsp://"+app.settings.user+":"+app.settings.pass+"@"
		    + app.settings.host + ":554"
		    + " -t 60 -vcodec copy -preset slow /home/salwatorska/data/camera/today/`date +%#F_%H.%M.%S`_"+app.settings.name+".mp4",
	    function puts(error, stdout, stderr) {
	    });
    res.send("ffmpeg -t 60 -i rtsp://"+app.settings.user+":"+app.settings.pass+"@"
		    + app.settings.host + ":554"
		    + " -preset slow /home/salwatorska/`date +%#F_%H.%M.%S`_"+app.settings.name+".mp4");
}

module.exports.getLiveCamera = function(req, res) {
    if (numberOfClients == 0) {
		ffmpeg = spawn("ffmpeg", [
           '-i', "rtsp://viewer:viewer123@192.168.1.52:554",
           '-f','webm',
           '-vcodec','libvpx',
           '-acodec','libvorbis',
           '-tune', 'zerolatency',
           '-fflags', 'nobuffer',
           'pipe:1']);
    }
    ;
    numberOfClients++;
    res.writeHead(200, {
	'Transfer-Encoding' : 'chunked',
	'Content-Type' : 'video/webm',
	"Connection": "keep-alive",
	"Accept-Ranges": "bytes" 
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



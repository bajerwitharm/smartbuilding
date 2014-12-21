var spawn = require('child_process').spawn;
var exec = require('child_process').exec;

var numberOfClients = 0;
var numberOfClients2 = 0;
var ffmpeg;
var ffmpeg2;
var EventEmitter = require('events').EventEmitter
var app = new EventEmitter;

module.exports = app

// defaults
app.settings = {
    host : 'kamerapietro1.salwatorska6',
    port : 81,
    user : 'viewer',
    pass : 'viewer123',
    name : 'KameraParter'
};

module.exports.initOnvif = function(db) {
    database = db;
    var datagram;
    var server = require('net').createServer(
	    function(stream) {
		stream.setEncoding('utf8');
		stream.addListener("data", function(slice) {
		    datagram = datagram + slice;
		    if (slice.length < 3472) {
			try {
			    var logentries = JSON.parse("["
				    + slice.slice(0, -1) + "]");
			    for ( var logentry in logentries) {
				logentry = logentries[logentry];
				if (logentry.length == 0) {
				    continue;
				}
				console.log(alarmEntry);
				//recordCamera(alarmEntry);
			    }
			} catch (err) {
			}

			datagram = "";
		    }
		});
	    });
    server.listen(15002, '0.0.0.0');
}

recordCamera = function(alarmEntry) {
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
}

module.exports.getLiveCamera = function(req, res) {
    if (numberOfClients == 0) {
	ffmpeg = spawn("ffmpeg", [
		"-rtsp_transport", "tcp",
		'-i', "rtsp://192.168.1.53/user=admin_password=FaWsG5QU_channel=1_stream=1.sdp?real_stream",
	        "-vcodec", "copy", 
		"-f", "mp4", 
		"-movflags", "frag_keyframe+empty_moov", 
                "-reset_timestamps", "1", 
		"-vsync", "1",
		"-flags", "global_header", 
		"-bsf:v", "dump_extra", "-y", "-" ]);

    }
    ;
    numberOfClients++;
    res.writeHead(200, {
	'Transfer-Encoding' : 'chunked',
	'Content-Type' : 'video/mp4',
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

module.exports.getLiveCamera2 = function(req, res) {
    if (numberOfClients2 == 0) {
	ffmpeg2 = spawn("ffmpeg", [
		"-rtsp_transport", "tcp",
		'-i', "rtsp://192.168.1.54/user=admin_password=FaWsG5QU_channel=1_stream=1.sdp?real_stream",
	        "-vcodec", "copy", 
		"-f", "mp4", 
		"-movflags", "frag_keyframe+empty_moov", 
                "-reset_timestamps", "1", 
		"-vsync", "1",
		"-flags", "global_header", 
		"-bsf:v", "dump_extra", "-y", "-" ]);

    }
    ;
    numberOfClients2++;
    res.writeHead(200, {
	'Transfer-Encoding' : 'chunked',
	'Content-Type' : 'video/mp4',
	"Connection": "keep-alive",
	"Accept-Ranges": "bytes" 
    });

    req.on('close', function(code) {
	numberOfClients2--;
	if (numberOfClients == 0) {
	    ffmpeg.kill();
	}
    });
    req.setTimeout(20000, function(code) {
	numberOfClients2--;
	if (numberOfClients == 0) {
	    ffmpeg.kill();
	}
    });
    ffmpeg2.stdout.pipe(res);
}
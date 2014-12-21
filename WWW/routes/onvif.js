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
app.settings = [{
    host : 'kamerapietro1.salwatorska6',
    port : 81,
    user : 'viewer',
    pass : 'viewer123',
    name : 'KameraPietro1'
},
{
    host : 'kamerapietro2.salwatorska6',
    port : 81,
    user : 'viewer',
    pass : 'viewer123',
    name : 'KameraPietro2'
}
];

module.exports.initOnvif = function(db) {
    database = db;
    var datagram;
    var server = require('net').createServer(
	    function(stream) {
		stream.setEncoding('utf8');
		stream.addListener("data", function(slice) {
		    slice=slice.substring(slice.indexOf("{"));
		    datagram = datagram + slice;
		    if (slice.length < 3472) {
			try {
			    console.log("[" + slice.slice(0, -1) + "]");
			    var alarmentries = JSON.parse("[" + slice.slice(0, -1) + "]");
			    console.log(alarmentries);
			    for ( var alarmentry in alarmentries) {
				alarmentry = logentries[alarmentry];
				if (alarmentry.length == 0) {
				    continue;
				}
				console.log(alarmEntry);
				if (alarmEntry.SerialID=="001212399c78") {
					recordCamera(app.settings[0]);
				} else {
					recordCamera(app.settings[1]);
				};
			    }
			} catch (err) {
				console.log(err);
			}

			datagram = "";
		    }
		});
	    });
    server.listen(15002, '0.0.0.0');
}

recordCamera = function(camera) {
    exec("ffmpeg -r 5 -t 60 -i 'http://"
		    + camera.host
		    + ":"
		    + camera.port
		    + "/cgi-bin/encoder?USER="
		    + camera.user
		    + "&PWD="
		    + camera.pass
		    + "&GET_STREAM' -acodec copy -vcodec mpeg4 -preset slow /home/salwatorska/`date +%#F_%H.%M.%S`"+camera.name+".avi",
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
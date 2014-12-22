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
			    var alarmEntries = JSON.parse("[" + slice.slice(0, -1) + "]");
			    console.log(alarmEntries);
			    console.log(alarmEntries[0]);
  			    if (alarmEntries[0].Status=="Start") {	
					if (alarmEntries[0].SerialID=="0012123972ca") {
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
    console.log("ffmpeg -t 60 -i rtsp://"
		    + camera.host + "/user=admin_password=FaWsG5QU_channel=1_stream=0.sdp?real_stream"
		    + " -preset slow /home/salwatorska/`date +%#F_%H.%M.%S`_"+camera.name+".avi");

    exec("ffmpeg -t 60 -i rtsp://"
		    + camera.host + "/user=admin_password=FaWsG5QU_channel=1_stream=0.sdp?real_stream"
		    + " -preset slow /home/salwatorska/`date +%#F_%H.%M.%S`_"+camera.name+".avi",
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
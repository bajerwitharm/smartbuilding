var spawn = require('child_process').spawn;
var exec = require('child_process').exec;

var numberOfClients = 0;
var numberOfClients2 = 0;
var EventEmitter = require('events').EventEmitter
var app = new EventEmitter;
var mqtt;

module.exports = app

// defaults
app.settings = [
    {
        url: 'rtsp://192.168.1.51:554/streaming/video0',
        name: 'KameraWejscie',
        serialId: '0012123972ab',
        exec: 0,
        timeout: 0
    },
    {
        url: 'rtsp://192.168.1.52:554/streaming/video0',
        name: 'KameraParter',
        serialId: '0012123972cd',
        exec: 0,
        timeout: 0
    },
    {
        url: 'rtsp://192.168.1.53:554/user=admin_password=FaWsG5QU_channel=1_stream=0.sdp?real_stream',
        name: 'KameraPietro1',
        serialId: '0012123972ca',
        exec: 0,
        timeout: 0
    },
    {
        url: 'rtsp://192.168.1.54:554/user=admin_password=FaWsG5QU_channel=1_stream=0.sdp?real_stream',
        name: 'KameraPietro2',
        serialId: '001212399c78',
        exec: 0,
        timeout: 0
    }
];

module.exports.init = function (mqtt_server) {
    var datagram = "";
    mqtt = mqtt_server.get_client();
    var server = require('net').createServer(
        function (stream) {
            stream.setEncoding('utf8');
            stream.addListener("data", function (slice) {
                slice = slice.substring(slice.indexOf("{"));
                datagram = datagram + slice;
                if (slice.length < 3472) {
                    try {
                        json = datagram.substring(datagram.indexOf('{'), datagram.lastIndexOf('}') + 1)
                        console.log(json);
                        var alarmEntries = JSON.parse("[" + json + "]");
                        if (alarmEntries[0].Status == "Start") {
                            app.settings.forEach(function (element) {
                                if (alarmEntries[0].SerialID == element.serialId) {
                                    recordCamera(element);
                                }
                            });
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

recordCamera = function (camera) {
    if (camera.exec==0) {
        mqtt_message = '{"SerialID": "' + camera.serialId + '", "Event": "Start", "Type": "Alarm"}';
        mqtt.publish("salwatorska6/"+camera.name+"/status", mqtt_message, {'qos': 1, 'retain': true}, function () {});
        camera.exec = exec("ffmpeg -i " + camera.url + " -t 300 -vcodec copy -preset slow /ftp/`date +%#F_%H.%M.%S`_" + camera.name + ".mp4",
        //camera.exec = exec("D:\\Downloads\\ffmpeg-20151224-git-4707497-win64-static\\ffmpeg-20151224-git-4707497-win64-static\\bin\\ffmpeg -i " + camera.url + " -t 300 -vcodec copy -preset slow C:\\Users\\plmabaj\\a_" + camera.name + ".mp4",
            function puts(error, stdout, stderr) {
                console.log(error);
            }
        );
        camera.exec.on('close', function (code, signal) {
            camera.exec = 0;
            camera.timeout = 0;
            mqtt_message = '{"SerialID": "' + camera.serialId + '", "Event": "Stop", "Type": "Alarm"}';
            mqtt.publish("salwatorska6/"+camera.name+"/status", mqtt_message, {'qos': 1, 'retain': true}, function () {});
        });
    }
    if (camera.timeout!=0) {
        clearTimeout(camera.timeout);
    }
    camera.timeout = setTimeout(function(){
        if (camera.exec!=0) {
            camera.exec.kill('SIGINT');
        }
    },20000)

}

module.exports.getLiveCamera = function (req, res) {
    if (numberOfClients == 0) {
        ffmpeg = spawn("ffmpeg", [
            "-rtsp_transport", "tcp",
            "-re",
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
        'Transfer-Encoding': 'chunked',
        'Content-Type': 'video/mp4',
        "Connection": "keep-alive",
        "Accept-Ranges": "bytes"
    });

    req.on('close', function (code) {
        numberOfClients--;
        if (numberOfClients == 0) {
            ffmpeg.kill();
        }
    });
    req.setTimeout(20000, function (code) {
        numberOfClients--;
        if (numberOfClients == 0) {
            ffmpeg.kill();
        }
    });
    ffmpeg.stdout.pipe(res);
}

module.exports.getLiveCamera2 = function (req, res) {
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
        'Transfer-Encoding': 'chunked',
        'Content-Type': 'video/mp4',
        "Connection": "keep-alive",
        "Accept-Ranges": "bytes"
    });

    req.on('close', function (code) {
        numberOfClients2--;
        if (numberOfClients == 0) {
            ffmpeg.kill();
        }
    });
    req.setTimeout(20000, function (code) {
        numberOfClients2--;
        if (numberOfClients == 0) {
            ffmpeg.kill();
        }
    });
    ffmpeg2.stdout.pipe(res);
}

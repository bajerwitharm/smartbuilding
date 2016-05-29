var spawn = require('child_process').spawn;
var exec = require('child_process').exec;

var numberOfClients = 0;
var numberOfClients2 = 0;
var EventEmitter = require('events').EventEmitter
var app = new EventEmitter;
var mqtt;

module.exports = app

// defaults
app.settings = require('../config/onvif.js')();

function waitForDatagram(slice, callback) {
    console.log(slice);
    waitForDatagram.datagram = waitForDatagram.datagram + slice;
    if (slice.length < 3472) {
        try {
           callback(waitForDatagram.datagram);
        } catch (err) {
            console.log(err);
        }
        waitForDatagram.datagram = '';
    }
}
waitForDatagram.datagram = '';

function findCamera(message, callback) {
    try {
        var json_datagram = JSON.parse("[" + message.substring.message.indexOf('{'), message.lastIndexOf('}') + 1) + "]");
        if (json_datagram[0].Status == "Start") {
            app.settings.forEach(function (element) {
                if (json_datagram[0].SerialID == element.serialId) {
                    callback(element);
                }
            });
        }
    } catch (e) {
        if (message.indexOf("Info:md alarm") > -1) {
            var found = message.match(/IP:(.*) Info/gi)[0];
            var serialId = found.substring(3, found.search("Info"));
            app.settings.forEach(function (element) {
                if (element.serialId == serialId) {
                    callback(element);
                }
            });
        }
    }
}

String.prototype.replaceAll = function(str1, str2, ignore) 
{
    return this.replace(new RegExp(str1.replace(/([\/\,\!\\\^\$\{\}\[\]\(\)\.\*\+\?\|\<\>\-\&])/g,"\\$&"),(ignore?"gi":"g")),(typeof(str2)=="string")?str2.replace(/\$/g,"$$$$"):str2);
} 

module.exports = function (mqtt_server) {
    var datagram = "";
    mqtt = mqtt_server.get_client();
    var server = require('net').createServer(
        function (stream) {
            stream.setEncoding('utf8');
            stream.addListener("data", function (slice) {
                waitForDatagram(slice, function (json_datagram) {
                    findCamera(json_datagram, recordCamera);
                })
            });
        });
    server.listen(15002, '0.0.0.0');
    var express = require('express');
    var router = express.Router();
    router.use(function timeLog(req, res, next) {
        //console.log('Time: ', Date.now());
        next();
    });
    router.all('/', function (req, res) {
        findCamera(JSON.parse(req.query.Message.replaceAll("'","\"")), recordCamera)
    });

    return router
}

function publishMqttStartRecord(camera) {
    var mqtt_message = '{"SerialID": "' + camera.serialId + '", "Event": "Start", "Type": "Alarm"}';
    mqtt.publish("salwatorska6/" + camera.name, mqtt_message, { 'qos': 1, 'retain': true }, function () { });
}

function publishMqttEndRecord(camera) {
    var mqtt_message = '{"SerialID": "' + camera.serialId + '", "Event": "Stop", "Type": "Alarm"}';
    mqtt.publish("salwatorska6/" + camera.name, mqtt_message, { 'qos': 1, 'retain': true }, function () { });
}

recordCamera = function (camera) {
    if (camera.exec == 0) {
        publishMqttStartRecord(camera);
        camera.exec = exec("mkdir -p /ftp/`date +%#F` && ffmpeg -i " + camera.url + " -t 30 -vcodec copy -v 0 -preset slow /ftp/`date +%#F`/`date +%#F_%H.%M.%S`_" + camera.name + ".mp4",
            function puts(error, stdout, stderr) {
                console.log(error);
            }
        );
        camera.exec.on('close', function (code, signal) {
            camera.exec = 0;
            camera.timeout = 0;
            publishMqttEndRecord(camera)
        });
    }
    if (camera.timeout != 0) {
        clearTimeout(camera.timeout);
    }
    camera.timeout = setTimeout(function () {
        if (camera.exec != 0) {
            camera.exec.kill('SIGINT');
        }
    }, 20000)
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
            "-bsf:v", "dump_extra", "-y", "-"]);

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
            "-bsf:v", "dump_extra", "-y", "-"]);

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

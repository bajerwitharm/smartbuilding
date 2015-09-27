var _ = require('struct-fu');
var com = require("serialport");
var underscore = require('underscore');
var mqtt;


Buffer.prototype.split = function (delim) {
    if (!(delim instanceof Buffer)) {
        delim = new Buffer(delim);
    }
    var buf = this;
    var ret = [];
    var delimIdx = 0;
    var stOffset = 0;
    var edOffset = 0;
    var histroy = [];

    for (var i = 0; i < this.length; i++) {
        if (buf[i] === delim[delimIdx]) {
            histroy.push(i);
            delimIdx += 1;
        }
        if (delimIdx === delim.length) {
            edOffset = histroy[0];
            ret.push(buf.slice(stOffset, edOffset));
            stOffset = edOffset + delim.length;
            delimIdx = 0;
            histroy = [];
        }
    }

    ret.push(buf.slice(stOffset, this.length));
    return ret;
};

var parsers = module.exports = {
    relayboard: function () {

        var data = new Buffer([]);
        return function (emitter, buffer) {
            // Collect data
            data = Buffer.concat([ data, buffer ]);
            // Split collected data by delimiter
            var parts = data.split(new Buffer([ 0x7B ]));
            data = parts.pop();
            parts.forEach(function (part, i, array) {
                emitter.emit('data', part);
            });
        };
    }
};

var isWin64 = process.env.hasOwnProperty('ProgramFiles(x86)');
var serialPort;
if (isWin64) {
    serialPort = new com.SerialPort("\\\\.\\COM6", {
        baudRate: 4800,
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false,
        parser: parsers.relayboard()
    });
} else {
    serialPort = new com.SerialPort("/dev/ttyUSB0", {
        baudRate: 4800,
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false,
        parser: parsers.relayboard()
    });
}

var busMaster = 0x0A
var relayBoard = 0x0B
var startChar = 0x7E
var action_triggered_e = 0xDE
var trigger_action_e = 0xDF
var get_info_e = 0xEA

//
// var converter = _.struct([
// _.struct('header', [
// _.uint8('start'),
// _.uint8('source'),
// _.uint8('destination'),
// _.uint8('fc'),
// _.uint8('size') ]),
// _.struct('actuator', [
// _.uint8('output_on'),
// _.uint8('output_off'),
// _.uint8('output_toggle'),
// _.uint8('state_on'),
// _.uint8('state_off'),
// _.uint8('state_toggle')]),
// ]);

var inputs = [ 'key_main_door', "KitchenSw2", "RoomSw1", "RoomSw2", "CorridorSw",
    "ToiletOutSw", "ToiletInSw", "KitchenMotion" ].reverse().concat(
        [ "RoomMotion", "CorridorMotion", "", "", "", "", "", "" ].reverse());
var outputs = [ "bulb_outside", "bulb_enterance_main", "bulb_enterance_small", "bulb_1kitchen_desk", "bulb_1kitchen_center", "bulb_1corridor", "bulb_1toilet_mirror",
    "bulb_1toilet_center" ].reverse();
var states = [ "KitchenSw1", "KitchenSw2", "KitchenMotion", "RoomSw1",
    "RoomSw2", "RoomMotion", "CorridorSw", "CorritorMotion" ].reverse()
    .concat([ "", "HeardBeat", "", "", "", "", "", "" ].reverse());

var genereteBitStruct = function (elements) {
    var result = [];
    elements.forEach(function (element) {
        result.push(_.bool(element));
    });
    return result;
}

var header_t = _.struct('header', [ _.uint8('start'), _.uint8('source'),
    _.uint8('destination'), _.uint8('fc'), _.uint8('size') ]);

var actuator_t = _.struct('actuator', [
    _.struct('output_on', genereteBitStruct(outputs)),
    _.struct('output_off', genereteBitStruct(outputs)),
    _.struct('output_toggle', genereteBitStruct(outputs)),
    _.struct('state_on', genereteBitStruct(states)),
    _.struct('state_off', genereteBitStruct(states)),
    _.struct('state_toggle', genereteBitStruct(states)) ]);

var activator_t = _.struct('activator', [
    _.struct('input_on', genereteBitStruct(inputs)),
    _.struct('input_off', genereteBitStruct(inputs)),
    _.struct('output_on', genereteBitStruct(outputs)),
    _.struct('output_off', genereteBitStruct(outputs)),
    _.struct('state_on', genereteBitStruct(states)),
    _.struct('state_off', genereteBitStruct(states)) ]);

var timer_t = _.struct('timer', [ _.uint16('time_preload'),
    _.uint16('time_current') ]);

var info_t = _.struct('info', [ _.struct('inputs', genereteBitStruct(inputs)),
    _.struct('outputs', genereteBitStruct(outputs)),
    _.struct('states', genereteBitStruct(states)) ]);

var crc_t = _.uint8('crc');

var trigger_action_t = _.struct([ header_t, actuator_t, crc_t ]);

// trigger_action
// { header: { start: 126, source: 250, destination: 251, fc: 223, size: 6 },
// actuator:
// { output_on:
// { Kitchen1: false,
// Kitchen2: false,
// Room1: false,
// Room2: false,
// Corridor: false,
// ToiletIn: false,
// ToiletOut: false,
// NotUsed: false },
// output_off:
// { Kitchen1: false,
// Kitchen2: false,
// Room1: false,
// Room2: false,
// Corridor: false,
// ToiletIn: false,
// ToiletOut: false,
// NotUsed: false },
// output_toggle:
// { Kitchen1: true,
// Kitchen2: true,
// Room1: true,
// Room2: true,
// Corridor: true,
// ToiletIn: true,
// ToiletOut: true,
// NotUsed: true },
// state_on:
// { KitchenSw1: false,
// KitchenSw2: false,
// KitchenMotion: false,
// RoomSw1: false,
// RoomSw2: false,
// RoomMotion: false,
// CorridorSw: false,
// CorritorMotion: false },
// state_off:
// { KitchenSw1: false,
// KitchenSw2: false,
// KitchenMotion: false,
// RoomSw1: false,
// RoomSw2: false,
// RoomMotion: false,
// CorridorSw: false,
// CorritorMotion: false },
// state_toggle:
// { KitchenSw1: false,
// KitchenSw2: false,
// KitchenMotion: false,
// RoomSw1: false,
// RoomSw2: false,
// RoomMotion: false,
// CorridorSw: false,
// CorritorMotion: false } },
// crc: 171 }
//
// [ 0x7E, 0xFA, 0xFB, 0xDF, 0x06, 0x00, 0x00, 0xFF,
// 0x00, 0x00, 0x00, 0xAB ]

var action_triggered_t = _.struct([ header_t, activator_t, actuator_t, timer_t, info_t,
    crc_t ]);

// { header: { start: 126, source: 251, destination: 250, fc: 222, size: 18 },
// activator:
// { input_on:
// { CorridorMotion: false,
// RoomMotion: true,
// KitchenMotion: false,
// ToiletInSw: false,
// ToiletOutSw: false,
// CorridorSw: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenSw2: false,
// KitchenSw1: false },
// input_off:
// { CorridorMotion: false,
// RoomMotion: false,
// KitchenMotion: false,
// ToiletInSw: false,
// ToiletOutSw: false,
// CorridorSw: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenSw2: false,
// KitchenSw1: false },
// output_on:
// { NotUsed: false,
// ToiletOut: false,
// ToiletIn: false,
// Corridor: false,
// Room2: false,
// Room1: false,
// Kitchen2: false,
// Kitchen1: false },
// output_off:
// { NotUsed: false,
// ToiletOut: false,
// ToiletIn: false,
// Corridor: false,
// Room2: false,
// Room1: false,
// Kitchen2: false,
// Kitchen1: false },
// state_on:
// { CorritorMotion: false,
// CorridorSw: false,
// RoomMotion: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenMotion: false,
// KitchenSw2: false,
// KitchenSw1: false },
// state_off:
// { CorritorMotion: false,
// CorridorSw: false,
// RoomMotion: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenMotion: false,
// KitchenSw2: false,
// KitchenSw1: false } },
// actuator:
// { output_on:
// { NotUsed: false,
// ToiletOut: false,
// ToiletIn: false,
// Corridor: false,
// Room2: false,
// Room1: false,
// Kitchen2: false,
// Kitchen1: false },
// output_off:
// { NotUsed: false,
// ToiletOut: false,
// ToiletIn: false,
// Corridor: false,
// Room2: false,
// Room1: false,
// Kitchen2: false,
// Kitchen1: false },
// output_toggle:
// { NotUsed: false,
// ToiletOut: false,
// ToiletIn: false,
// Corridor: false,
// Room2: false,
// Room1: false,
// Kitchen2: false,
// Kitchen1: true },
// state_on:
// { CorritorMotion: false,
// CorridorSw: false,
// RoomMotion: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenMotion: false,
// KitchenSw2: false,
// KitchenSw1: false },
// state_off:
// { CorritorMotion: false,
// CorridorSw: false,
// RoomMotion: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenMotion: false,
// KitchenSw2: false,
// KitchenSw1: false },
// state_toggle:
// { CorritorMotion: false,
// CorridorSw: false,
// RoomMotion: false,
// RoomSw2: false,
// RoomSw1: false,
// KitchenMotion: false,
// KitchenSw2: false,
// KitchenSw1: true } },
// timer: { time_preload: 256, time_current: 256 },
// crc: 171 }
// Buffer([ 0x7E, 0xFB, 0xFA, 0xDE, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0xAB
// ])

var get_info_t_req = _.struct([ header_t,
// _.uint8('data'),
    crc_t ]);

var get_info_t_res = _.struct([ header_t, info_t, crc_t ]);

module.exports.requestInfo = function (req, res) {
    telegram = {
        header: {
            start: startChar,
            source: busMaster,
            destination: relayBoard,
            fc: get_info_e,
            size: 0x00
        },
        crc: 0xAB
    };
    serialPort.write(get_info_t_req.pack(telegram));
    console.log(get_info_t_req.pack(telegram));
    serialPort.once('data', function (data) {
        // console.log(data);
        res.send(get_info_t_res.unpack(data));
    });
}

var cleanJson = function (obj) {
    var isArray = obj instanceof Array;
    for (var k in obj) {
        if ((obj[k] == null) || (obj[k] == false)) {
            if (isArray) {
                if (obj[k].length > 0) {
                    obj.splice(k, 1)
                } else {
                    delete obj[k];
                }
            } else {
                delete obj[k];
            }
        } else if (typeof obj[k] == "object") {
            cleanJson(obj[k]);
            if (Object.getOwnPropertyNames(obj[k]).length == 0) {
                delete obj[k];
            }
        }
    }

}

serialPort.on('data', function (data) {
    console.log(data);
    // try {
    switch (data[3]) {
        case get_info_e:
            data = get_info_t_res.unpack(data);
            console.log(data);
            mqtt.relayEvent(JSON.stringify(data));
            break;
        case action_triggered_e:
            data = action_triggered_t.unpack(data);
            // cleanJson(data);
            console.log(data);
            mqtt.relayEvent(JSON.stringify(data));
            break;
        case trigger_action_e:
            data = trigger_action_t.unpack(data);
            // cleanJson(data);
            console.log(data);
            break;
    }

    // } catch (err) {
    // }
});


module.exports.relayOffTelegram = function (req, res) {
    console.log(req.toString());
    telegram = {
        header: {
            start: startChar,
            source: busMaster,
            destination: relayBoard,
            fc: trigger_action_e,
            size: 9
        },
        actuator: {
            output_off: {
                bulb_4room: true,
                bulb_1room: true,
                bulb_4kitchen: true,
                Room2: true,
                Corridor: true,
                ToiletIn: true,
                ToiletOut: true,
                NotUsed: true
            }
        },
        crc: 171
    }
    serialPort.write(trigger_action_t.pack(telegram));
    serialPort.once('data', function (data) {
        data = trigger_action_t.unpack(data);
        cleanJson(data);
        res.send(data);
    });
}


module.exports.relayOnTelegram = function (req, res) {
    console.log(req.toString());

    serialPort.write(trigger_action_t.pack(telegram));
    serialPort.once('data', function (data) {
        data = trigger_action_t.unpack(data);
        cleanJson(data);
        res.send(data);
    });

}

module.exports.init = function (mqtt_global) {
    mqtt = mqtt_global;
    mqtt.relayControl(function(telegram){
        console.log(telegram);
        var base_telegram = {
            header: {
                start: startChar,
                source: busMaster,
                destination: relayBoard,
                fc: trigger_action_e,
                size: 9
            },
            crc: 171
        }
        base_telegram.actuator = JSON.parse(telegram);
        console.log(base_telegram);
        console.log(trigger_action_t.pack(base_telegram));
        serialPort.write(trigger_action_t.pack(base_telegram));
    });
}
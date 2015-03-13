var _ = require('struct-fu');
var SerialPort = require("serialport").SerialPort;
var serialPort = new SerialPort("/dev/ttyUSB0", {
	baudRate : 9600,
	dataBits : 8,
	parity : 'none',
	stopBits : 1,
	flowControl : false
});

serialPort.on("data", function(data) {
	console.log(data);
});
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



var inputs = ["KitchenSw1","KitchenSw2","RoomSw1","RoomSw2","CorridorSw","ToiletOutSw","ToiletInSw","KitchenMotion","RoomMotion","CorridorMotion","","","","","",""].reverse();
var outputs = ["Kitchen1","Kitchen2","Room1","Room2","Corridor","ToiletIn","ToiletOut","NotUsed"].reverse();
var states = ["KitchenSw1","KitchenSw2","KitchenMotion","RoomSw1","RoomSw2","RoomMotion","CorridorSw","CorritorMotion"].reverse();

var genereteBitStruct = function(elements) {
	var result = [];
	elements.forEach(
			function(element){	
				result.push(_.bool(element));
			}
	);
	return result;
}

var header_t = _.struct('header', [ 
                                   _.uint8('start'), 
                                   _.uint8('source'), 
                                   _.uint8('destination'),
                                   _.uint8('fc'), 
                                   _.uint8('size') ]);

var actuator_t = _.struct('actuator', [ 
                                       _.struct('output_on',genereteBitStruct(outputs)), 
                                       _.struct('output_off',genereteBitStruct(outputs)),
                                       _.struct('output_toggle',genereteBitStruct(outputs)),
                                       _.struct('state_on',genereteBitStruct(states)),
                                       _.struct('state_off',genereteBitStruct(states)),
                                       _.struct('state_toggle',genereteBitStruct(states))]);

var activator_t = _.struct('activator', [
                                       _.struct('input_on',genereteBitStruct(inputs)), 
                                       _.struct('input_off',genereteBitStruct(inputs)),
                                       _.struct('output_on',genereteBitStruct(outputs)),
                                       _.struct('output_off',genereteBitStruct(outputs)),
                                       _.struct('state_on',genereteBitStruct(states)),
                                       _.struct('state_off',genereteBitStruct(states))]); 

var timer_t =  _.struct('timer', [
                                  _.uint16('time_preload'), 
                                  _.uint16('time_current')
                                  ]);

var crc_t =  _.uint8('crc');

var trigger_action_t = _.struct([
                             	header_t,
                             	actuator_t,
                             	crc_t
                             ]);

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

var action_triggered_t = _.struct([
                                	header_t,
                                	activator_t,
                                	actuator_t,
                                	timer_t,
                                	crc_t
                                ]);

//{ header: { start: 126, source: 251, destination: 250, fc: 222, size: 18 },
//	  activator: 
//	   { input_on: 
//	      { CorridorMotion: false,
//	        RoomMotion: true,
//	        KitchenMotion: false,
//	        ToiletInSw: false,
//	        ToiletOutSw: false,
//	        CorridorSw: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenSw2: false,
//	        KitchenSw1: false },
//	     input_off: 
//	      { CorridorMotion: false,
//	        RoomMotion: false,
//	        KitchenMotion: false,
//	        ToiletInSw: false,
//	        ToiletOutSw: false,
//	        CorridorSw: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenSw2: false,
//	        KitchenSw1: false },
//	     output_on: 
//	      { NotUsed: false,
//	        ToiletOut: false,
//	        ToiletIn: false,
//	        Corridor: false,
//	        Room2: false,
//	        Room1: false,
//	        Kitchen2: false,
//	        Kitchen1: false },
//	     output_off: 
//	      { NotUsed: false,
//	        ToiletOut: false,
//	        ToiletIn: false,
//	        Corridor: false,
//	        Room2: false,
//	        Room1: false,
//	        Kitchen2: false,
//	        Kitchen1: false },
//	     state_on: 
//	      { CorritorMotion: false,
//	        CorridorSw: false,
//	        RoomMotion: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenMotion: false,
//	        KitchenSw2: false,
//	        KitchenSw1: false },
//	     state_off: 
//	      { CorritorMotion: false,
//	        CorridorSw: false,
//	        RoomMotion: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenMotion: false,
//	        KitchenSw2: false,
//	        KitchenSw1: false } },
//	  actuator: 
//	   { output_on: 
//	      { NotUsed: false,
//	        ToiletOut: false,
//	        ToiletIn: false,
//	        Corridor: false,
//	        Room2: false,
//	        Room1: false,
//	        Kitchen2: false,
//	        Kitchen1: false },
//	     output_off: 
//	      { NotUsed: false,
//	        ToiletOut: false,
//	        ToiletIn: false,
//	        Corridor: false,
//	        Room2: false,
//	        Room1: false,
//	        Kitchen2: false,
//	        Kitchen1: false },
//	     output_toggle: 
//	      { NotUsed: false,
//	        ToiletOut: false,
//	        ToiletIn: false,
//	        Corridor: false,
//	        Room2: false,
//	        Room1: false,
//	        Kitchen2: false,
//	        Kitchen1: true },
//	     state_on: 
//	      { CorritorMotion: false,
//	        CorridorSw: false,
//	        RoomMotion: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenMotion: false,
//	        KitchenSw2: false,
//	        KitchenSw1: false },
//	     state_off: 
//	      { CorritorMotion: false,
//	        CorridorSw: false,
//	        RoomMotion: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenMotion: false,
//	        KitchenSw2: false,
//	        KitchenSw1: false },
//	     state_toggle: 
//	      { CorritorMotion: false,
//	        CorridorSw: false,
//	        RoomMotion: false,
//	        RoomSw2: false,
//	        RoomSw1: false,
//	        KitchenMotion: false,
//	        KitchenSw2: false,
//	        KitchenSw1: true } },
//	  timer: { time_preload: 256, time_current: 256 },
//	  crc: 171 }
// Buffer([ 0x7E, 0xFB, 0xFA, 0xDE, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0xAB ])

module.exports.writeTelegram = function(req, res) {
	telegram = {header:{start:0x7e,source:0xFA,destination:0xFB,fc:0xDF,size:0x06},
				actuator:{output_on:0x00,output_off:0x00,output_toggle:0xFF,state_on:0x00,state_off:0x00,state_toggle:0x00}}
	toReceive = action_triggered_t.unpack(Buffer([ 0x7E, 0xFB, 0xFA, 0xDE, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0xAB ]));
// toSend = converter.pack(telegram);
	console.log(toReceive);
// serialPort.write(Buffer([ 0x7E, 0xFA, 0xFB, 0xDF, 0x06, 0x00, 0x00, 0xFF,
// 0x00, 0x00, 0x00, 0xAB ]));
	
	

}
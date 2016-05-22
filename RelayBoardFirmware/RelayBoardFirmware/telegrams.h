#include "triggers.h"
/*
 * telegrams.h
 *
 * Created: 2015-01-24 22:57:24
 *  Author: Marcin Bajer (https://github.com/bajerwitharm)
 * 
 * Specify all communication telegram types used in the system
 */ 


#ifndef TELEGRAMS_H_
#define TELEGRAMS_H_

#pragma pack(1)
typedef uint8_t crc_t;

typedef enum __attribute__ ((__packed__)) {
	action_triggered_e = 0xDE,
	trigger_action_e = 0xDF,
	get_info_e = 0xEA,
	get_triggers_e = 0xDA, 	
	set_trigger_e = 0xDB, 
	error_e = 0xAB,	
} function_code_t;

typedef enum __attribute__ ((__packed__)) {
	telegram_too_short_e = 0x01,
	telegram_too_long_e = 0x02,
	telegram_crc_error_e = 0x03,
} error_code_t;

typedef struct __attribute__ ((__packed__)) {
	uint8_t source;
	uint8_t destination;
	function_code_t fc;
    uint8_t size;	
} telegram_header_t;

typedef struct __attribute__ ((__packed__)) {
	telegram_header_t header;
	error_code_t error;
} error_telegram_t;

//Test telegram:7E FB FA DE 12 02 00 00 00 00 00 00 03 03 00 00 04 00 00 01 00 01 00 AB
//				7E FB FA DE 12 01 00 00 00 00 00 00 00 00 00 01 00 00 01 01 00 01 00 AB
typedef struct __attribute__ ((__packed__)) {
	telegram_header_t header;
	trigger_t trigger;
	info_t info;
} action_triggered_t;

typedef struct __attribute__ ((__packed__)) {
	telegram_header_t header;
	trigger_t trigger;
	uint8_t trigger_id;
} set_trigger_t;

//Test telegram: 0x7E 0xFA 0xFB 0xDF 0x06 0x00 0x00 0xFF 0x00 0x00 0x00 0xAB
typedef struct __attribute__ ((__packed__)) {
	telegram_header_t header;
	actuator_t actuator;
} trigger_action_t;

//Test telegram: 0x7E 0xFA 0xFB 0xEA 0x00 0xAB
typedef struct __attribute__ ((__packed__)) {
	telegram_header_t header;
	info_t info;
} get_info_t;
#pragma pack()


#endif /* TELEGRAMS_H_ */
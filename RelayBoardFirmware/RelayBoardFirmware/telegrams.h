#include "triggers.h"
/*
 * telegrams.h
 *
 * Created: 2015-01-24 22:57:24
 *  Author: PLMABAJ
 */ 


#ifndef TELEGRAMS_H_
#define TELEGRAMS_H_

#pragma pack(1)
typedef uint8_t crc_t;

typedef enum __attribute__ ((__packed__)) {
	action_triggered_e = 0xDE,
	trigger_action_e = 0xDF	
} function_code_t;

typedef struct {
	uint8_t source;
	uint8_t destination;
	function_code_t fc;
    uint8_t size;	
} telegram_header_t;

//Test telegram:7E FB FA DE 12 02 00 00 00 00 00 00 03 03 00 00 04 00 00 01 00 01 00 AB
//				7E FB FA DE 12 01 00 00 00 00 00 00 00 00 00 01 00 00 01 01 00 01 00 AB
typedef struct {
	telegram_header_t header;
	trigger_t trigger;
} action_triggered_t;

//Test telegram: 0x7E 0xFA 0xFB 0xDF 0x06 0x00 0x00 0xFF 0x00 0x00 0x00 0xAB
typedef struct {
	telegram_header_t header;
	actuator_t actuator;
} trigger_action_t;
#pragma pack()


#endif /* TELEGRAMS_H_ */
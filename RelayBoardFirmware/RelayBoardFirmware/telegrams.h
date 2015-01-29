#include "triggers.h"
/*
 * telegrams.h
 *
 * Created: 2015-01-24 22:57:24
 *  Author: PLMABAJ
 */ 


#ifndef TELEGRAMS_H_
#define TELEGRAMS_H_

typedef enum {
	action_triggered_e,
	trigger_action_e	
} function_code_t;

typedef uint8_t crc_t;

typedef struct {
	uint8_t source;
	uint8_t destination;
	function_code_t fc;	
} telegram_header_t;

typedef struct {
	telegram_header_t header;
	trigger_t trigger;
} action_triggered_t;

typedef struct {
	telegram_header_t header;
	actuator_t actuator;
} trigger_action_t;



#endif /* TELEGRAMS_H_ */
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
} function_code_t;

typedef uint8_t crc_t;

typedef struct {
	function_code_t fc;
	trigger_t trigger;
} action_triggered_t;



#endif /* TELEGRAMS_H_ */
/*
 * triggers.h
 *
 * Created: 1/29/2015 1:43:31 PM
 *  Author: Marcin Bajer
 */ 


#ifndef TRIGGERS_H_
#define TRIGGERS_H_

#include "global.h"
#include "timer.h"

typedef struct {
	uint16_t input_on;
	uint16_t input_off;
	uint8_t output_on;
	uint8_t output_off;
	uint8_t state_on;
	uint8_t state_off;
} activator_t;

typedef struct {
	uint8_t output_on;
	uint8_t output_off;
	uint8_t output_toggle;
	uint8_t state_on;
	uint8_t state_off;
	uint8_t state_toggle;
} actuator_t;

typedef struct {
	activator_t activator;
	actuator_t actuator;
	struct {
		uint16_t time_preload;
		uint16_t time_current;
	} timer;
} trigger_t;

void activateTrigger(const actuator_t* const actuator_p);
void processTriggers();




#endif /* TRIGGERS_H_ */
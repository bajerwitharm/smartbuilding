/*
 * timer.h
 *
 * Created: 2015-01-18 12:03:09
 *  Author: Marcin Bajer
 */ 


#ifndef TIMER_H_
#define TIMER_H_

typedef struct {
	struct {
		uint16_t input_on;		
		uint16_t input_off;		
		uint8_t output_on;		
		uint8_t output_off;
		uint8_t state_on;
		uint8_t state_off;
	} activator;
	struct {
		uint8_t output_on;
		uint8_t output_off;
		uint8_t output_toggle;
		uint8_t state_on;
		uint8_t state_off;
	} actuator;
	struct {
		uint16_t time_preload;
		uint16_t time_current;
	} timer;
} trigger_t;

void timerInit();

#endif /* TIMER_H_ */
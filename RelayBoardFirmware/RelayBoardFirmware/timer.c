/*
 * timer.c
 *
 * Created: 2015-01-18 12:03:22
 *  Author: Marcin Bajer
 */ 

#define TIMER_PRELOAD_100ms 158	// trigger interrupt every 0.1 s
#define TIMER_MS_TO_TIMER(x) x/100

#include "global.h"
#include "io_pins.h"
#include "timer.h"
#include "usart.h"

trigger_t triggers[] = {
{
	.activator = {
		.input_off = 0x00F0,
		.input_on = 0x0001,
		.output_off = 0x00,
		.output_on = 0x00,
		.time_preload = TIMER_MS_TO_TIMER(200),
		.time_current = 0,
	},	
	.actuator = {
		.output_off = 0x00,
		.output_toggle = 0x01,
		.output_on = 0x00
	}
}
};

bool inputOffVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.input_off & ~ioGetInputs()) == triggers[trigger_index].activator.input_off);
}

bool inputOnVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.input_on & ioGetInputs()) == triggers[trigger_index].activator.input_on);
}

bool outputOffVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.output_off & ~ioGetOutputs()) == triggers[trigger_index].activator.output_off);
}

bool outputOnVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.output_on & ioGetOutputs()) == triggers[trigger_index].activator.output_on);
}

void processTrigger(uint8_t trigger_index) {
	if (
		inputOffVerified(trigger_index) && 
		inputOnVerified(trigger_index) &&
		outputOffVerified(trigger_index) &&
		outputOnVerified(trigger_index)
	) {	
		//all condition satisfied, check if action should be triggered
		if (triggers[trigger_index].activator.time_current == triggers[trigger_index].activator.time_preload) {
			//already count down and new state should be activated
			ioSetOutput(triggers[trigger_index].actuator.output_on);
			ioResetOutput(triggers[trigger_index].actuator.output_off);
			ioToggleOutput(triggers[trigger_index].actuator.output_toggle);
			usartSendAction(&triggers[trigger_index]);
			triggers[trigger_index].activator.time_current++;
		}
		//check if should count down to activate new output state
		if (triggers[trigger_index].activator.time_current < triggers[trigger_index].activator.time_preload)
		{
			triggers[trigger_index].activator.time_current++;
		}	
	}
	else
	{
		//condition not valid, set countdown counter to 0
		triggers[trigger_index].activator.time_current = 0;
	}
}

//Place/Copy this part in declaration section
void timerInit(){
	TCCR0 |= (1 << CS02) | (1 << CS00);	// set prescaler to 1024 and start the timer
	TIMSK |= (1 << TOIE0);
	TCNT0 = TIMER_PRELOAD_100ms;
}

ISR (TIMER0_OVF_vect)  // timer0 overflow interrupt
{
	TCNT0 = TIMER_PRELOAD_100ms;
	processTrigger(0);
}
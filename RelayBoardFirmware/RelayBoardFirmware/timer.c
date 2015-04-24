/*
 * timer.c
 *
 * Created: 2015-01-18 12:03:22
 *  Author: Marcin Bajer (https://github.com/bajerwitharm)
 *
 * Provides functionality of calling processTriggers() periodically every 100ms
 */ 

#define TIMER_PRELOAD_100ms 158	// trigger interrupt every 0.1 s


#include "global.h"
#include "triggers.h"


/**
 * Init timer
 */
void timerInit(){
	TCCR0 |= (1 << CS02) | (1 << CS00);	// set prescaler to 1024 and start the timer
	TIMSK |= (1 << TOIE0);
	TCNT0 = TIMER_PRELOAD_100ms;
}

/**
 * Interrupt from timer
 */
ISR (TIMER0_OVF_vect)  // timer0 overflow interrupt
{
	TCNT0 = TIMER_PRELOAD_100ms;
	processTriggers();
}
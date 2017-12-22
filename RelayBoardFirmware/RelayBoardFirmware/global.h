/*
 * global.h
 *
 * Created: 2015-01-17 19:34:34
 *  Author: Marcin Bajer (https://github.com/bajerwitharm)
 */ 

#include <stdint.h> 
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "conf_board.h"
#include "triggers.h"


#ifndef F_CPU
#define F_CPU 1000000 // 1 Mhz
#endif

#ifndef NO_TRIGGERS

const uint8_t getNumberOfTriggers();
#endif

#endif /* INCFILE1_H_ */
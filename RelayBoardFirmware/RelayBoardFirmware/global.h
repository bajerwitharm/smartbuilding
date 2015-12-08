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
#include "triggers.h"

#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifndef F_CPU
#define F_CPU 1000000 // 1 Mhz
#endif

extern trigger_t triggers[];
const uint8_t getNumberOfTriggers();

#endif /* INCFILE1_H_ */
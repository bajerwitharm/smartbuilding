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

#ifndef F_CPU
#define F_CPU 1000000 // 1 Mhz
#endif

typedef uint16_t inputs_t;
typedef uint8_t outputs_t;
typedef uint16_t states_t;

#endif /* INCFILE1_H_ */
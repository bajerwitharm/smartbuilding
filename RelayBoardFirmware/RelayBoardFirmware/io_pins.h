/*
 * io_pins.h
 *
 * Created: 2015-01-16 23:03:28
 *  Author: Marcin Bajer
 */ 

#include <stdint.h>
#include "global.h"

#ifndef IO_PINS_
#define IO_PINS_

void ioInit(void);
void ioSetOutput(outputs_t outputs);
void ioResetOutput(outputs_t outputs);
void ioToggleOutput(outputs_t outputs);
uint16_t ioGetInputs();
uint8_t ioGetOutputs();

#endif /* IO_PINS_ */
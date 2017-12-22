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
void ioSetOutput(const outputs_t outputs);
void ioResetOutput(const outputs_t outputs);
void ioToggleOutput(const outputs_t outputs);
inputs_t ioGetInputs();
outputs_t ioGetOutputs();

#endif /* IO_PINS_ */
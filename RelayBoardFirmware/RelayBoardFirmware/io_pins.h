/*
 * io_pins.h
 *
 * Created: 2015-01-16 23:03:28
 *  Author: Marcin Bajer
 */ 

#include <stdint.h>

#ifndef IO_PINS_
#define IO_PINS_

void ioInit(void);
void ioSetOutput(uint16_t outputs);
void ioResetOutput(uint16_t outputs);
void ioToggleOutput(uint16_t outputs);
uint16_t ioGetInputs();
uint8_t ioGetOutputs();

#endif /* INCFILE1_H_ */
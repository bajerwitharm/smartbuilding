/*
 * io.h
 *
 * Created: 2015-01-16 23:03:28
 *  Author: PLMABAJ
 */ 

#include <stdint.h>

#ifndef INCFILE1_H_
#define INCFILE1_H_

void ioInit(void);
void ioSetOutput(uint16_t outputs);
void ioResetOutput(uint16_t outputs);
uint16_t ioGetInputs();
uint8_t ioGetOutputs();

#endif /* INCFILE1_H_ */
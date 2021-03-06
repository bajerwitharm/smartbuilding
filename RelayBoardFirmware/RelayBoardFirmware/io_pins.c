/*
* io_pins.c
*
* Created: 2015-01-16 23:02:36
*  Author: Marcin Bajer (https://github.com/bajerwitharm)
*
* Provides abstraction layer for inputs and outputs. Inputs/outputs are described by variable no matter to which pin/port they are physically connected.
*/


#include "global.h"

// outputs connected used by relays
#define OUTPUT1_PIN 0 //port B
#define OUTPUT2_PIN 1 //port B
#define OUTPUT3_PIN 2 //port B
#define OUTPUT4_PIN 3 //port B
#define OUTPUT5_PIN 4 //port B
#define OUTPUT6_PIN 5 //port B
#define OUTPUT7_PIN 6 //port D
#define OUTPUT8_PIN 7 //port D
#define OUTPUT1_6_PORT_DIR DDRB
#define OUTPUT1_6_PORT PORTB
#define OUTPUT7_8_PORT_DIR DDRD
#define OUTPUT7_8_PORT PORTD

#ifdef RELAY_16_BOARD
#define OUTPUT9_PIN 2 //port D
#define OUTPUT10_PIN 3 //port D
#define OUTPUT9_10_PORT_DIR DDRD
#define OUTPUT9_10_PORT PORTD
#define OUTPUT11_PIN 0 //port B
#define OUTPUT12_PIN 1 //port B
#define OUTPUT13_PIN 2 //port B
#define OUTPUT14_PIN 3 //port B
#define OUTPUT15_PIN 4 //port B
#define OUTPUT16_PIN 5 //port B
#define OUTPUT11_16_PORT_DIR DDRC
#define OUTPUT11_16_PORT PORTC
#endif

//PD0 - FTDI RX
//PD1 - FTDI TX
//1wire Portc.3

/**
* Inputs (switches)
*/
#define SWITCH8_PIN 2
#define SWITCH9_PIN 3
#define SWITCH10_PIN 4
#define SWITCH11_PIN 5
#define SWITCH8_11_PORT_DIR DDRD
#define SWITCH8_11_PORT_PULLUP PORTD
#define SWITCH8_11_PORT ((uint16_t)PIND)
#define SWITCH1_PIN 0
#define SWITCH2_PIN 1
#define SWITCH3_PIN 2
#define SWITCH4_PIN 3
#define SWITCH5_PIN 4
#define SWITCH6_PIN 5
#define SWITCH7_PIN 6
#define SWITCH1_7_PORT_DIR DDRC
#define SWITCH1_7_PORT_PULLUP PORTC
#define SWITCH1_7_PORT ((uint16_t)PINC)



void initOutputPins(void)
{
	OUTPUT1_6_PORT_DIR |= (1<<OUTPUT1_PIN|1<<OUTPUT2_PIN|1<<OUTPUT3_PIN|1<<OUTPUT4_PIN|1<<OUTPUT5_PIN|1<<OUTPUT6_PIN);
	OUTPUT7_8_PORT_DIR |= (1<<OUTPUT7_PIN|1<<OUTPUT8_PIN);
	#ifdef RELAY_16_BOARD
	OUTPUT9_10_PORT_DIR |= (1<<OUTPUT9_PIN|1<<OUTPUT10_PIN);
	OUTPUT11_16_PORT_DIR |= (1<<OUTPUT11_PIN|1<<OUTPUT12_PIN|1<<OUTPUT13_PIN|1<<OUTPUT14_PIN|1<<OUTPUT15_PIN|1<<OUTPUT16_PIN);
	#endif
}

/**
* Init Input pins according to
*/
void initInputPins(void)
{
	#ifndef RELAY_16_BOARD
	SWITCH8_11_PORT_DIR &= ~(1<<SWITCH8_PIN|1<<SWITCH9_PIN|1<<SWITCH10_PIN|1<<SWITCH11_PIN);
	SWITCH8_11_PORT_PULLUP |= (1<<SWITCH8_PIN|1<<SWITCH9_PIN|1<<SWITCH10_PIN|1<<SWITCH11_PIN);
	SWITCH1_7_PORT_DIR &= ~(1<<SWITCH1_PIN|1<<SWITCH2_PIN|1<<SWITCH3_PIN|1<<SWITCH4_PIN|1<<SWITCH5_PIN|1<<SWITCH6_PIN|1<<SWITCH7_PIN);
	SWITCH1_7_PORT_PULLUP |= (1<<SWITCH1_PIN|1<<SWITCH2_PIN|1<<SWITCH3_PIN|1<<SWITCH4_PIN|1<<SWITCH5_PIN|1<<SWITCH6_PIN|1<<SWITCH7_PIN);
	#endif
}

/**
* Init port pins directions
*/
void ioInit(void)
{
	initOutputPins();
	initInputPins();
	//	MCUCSR |= 0x80;
	//	MCUCSR |= (1<<7);
}

/**
* Set output of device
* \param outputs - if bit is set appropriate output will be set, if bit is not
*					set no output change occur i.e outputs=0x03 sets outputs 1 and 2
*
* Refer to \ref ioResetOutput for output reset
*/
void ioSetOutput(const outputs_t outputs)
{
	OUTPUT1_6_PORT |= (outputs & 0x003F)<<OUTPUT1_PIN;
	OUTPUT7_8_PORT |= (outputs & 0x00C0)<<(6-OUTPUT7_PIN);
	#ifdef RELAY_16_BOARD
	OUTPUT9_10_PORT |= (outputs & 0x0300)>>(8-OUTPUT9_PIN);
	OUTPUT11_16_PORT |= (outputs & 0xFC00)>>(10-OUTPUT11_PIN);
	#endif
}

/**
* Set output of device
* \param outputs - if bit is set appropriate output will be set, if bit is not
*					set no output change occur i.e outputs=0x03 sets outputs 1 and 2
*
* Refer to \ref ioSetOutput for output setting
*/
void ioResetOutput(const outputs_t outputs)
{
	OUTPUT1_6_PORT &= ~((outputs & 0x3F)<<OUTPUT1_PIN);
	OUTPUT7_8_PORT &= ~((outputs & 0xC0)<<(6-OUTPUT7_PIN));
	#ifdef RELAY_16_BOARD
	OUTPUT9_10_PORT &= ~((outputs & 0x0300)>>(8-OUTPUT9_PIN));
	OUTPUT11_16_PORT &= ~((outputs & 0xFC00)>>(10-OUTPUT11_PIN));
	#endif
}

/**
* Set output of device
* \param outputs - if bit is set appropriate output will be set, if bit is not
*					set no output change occur i.e outputs=0x03 sets outputs 1 and 2
*
* Refer to \ref ioSetOutput for output setting
*/
void ioToggleOutput(const outputs_t outputs)
{
	OUTPUT1_6_PORT ^= (outputs & 0x3F)<<OUTPUT1_PIN;
	OUTPUT7_8_PORT ^= (outputs & 0xC0)<<(6-OUTPUT7_PIN);
	#ifdef RELAY_16_BOARD
	OUTPUT9_10_PORT ^= ((outputs & 0x0300)>>(8-OUTPUT9_PIN));
	OUTPUT11_16_PORT ^= ((outputs & 0xFC00)>>(10-OUTPUT11_PIN));
	#endif
}

/**
* Returns inputs state
* if bit is set means input is high if bit not set input is reset i.e. 0x0101 means that inputs 9 and 1 are high
*/
inputs_t ioGetInputs()
{
	return (((SWITCH8_11_PORT<<(7-SWITCH8_PIN)) & 0x0780)|((SWITCH1_7_PORT>>(SWITCH1_PIN)) & 0x007F));
}

/**
* Returns output state
* if bit is set means input is high if bit not set input is reset i.e. 0x11 means that outputs 5 and 1 are high
*/
outputs_t ioGetOutputs()
{
	#ifndef RELAY_16_BOARD
	return ((OUTPUT1_6_PORT>>OUTPUT1_PIN) & 0x3F)|((OUTPUT7_8_PORT<<(6-OUTPUT7_PIN)) & 0xC0);
	#else
	return (
	(((outputs_t)OUTPUT1_6_PORT>>OUTPUT1_PIN) & 0x003F)|
	(((outputs_t)OUTPUT7_8_PORT<<(6-OUTPUT7_PIN)) & 0x00C0)|
	((((outputs_t)OUTPUT9_10_PORT)<<(8-OUTPUT9_PIN))& 0x0300)|
	((((outputs_t)OUTPUT11_16_PORT)<<(10-OUTPUT11_PIN))& 0xFC00)
	);
	#endif
}
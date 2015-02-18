/*
 * io_pins.c
 *
 * Created: 2015-01-16 23:02:36
 *  Author: Marcin Bajer
 */ 


#include "global.h"

#define OUTPUT1_PIN 0
#define OUTPUT2_PIN 1
#define OUTPUT3_PIN 2
#define OUTPUT4_PIN 3
#define OUTPUT5_PIN 4
#define OUTPUT6_PIN 5
#define OUTPUT7_PIN 6
#define OUTPUT8_PIN 7
#define OUTPUT1_6_PORT_DIR DDRB
#define OUTPUT1_6_PORT PORTB
#define OUTPUT7_8_PORT_DIR DDRD
#define OUTPUT7_8_PORT PORTD
//PD0 - FTDI RX
//PD1 - FTDI TX 
//1wire Portc.3

/**
* Switches are isolated with transoptors
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
}

/**
* Init Input pins according to                                                                      
*/
void initInputPins(void)
{
	SWITCH8_11_PORT_DIR &= ~(1<<SWITCH8_PIN|1<<SWITCH9_PIN|1<<SWITCH10_PIN|1<<SWITCH11_PIN);
	SWITCH8_11_PORT_PULLUP |= (1<<SWITCH8_PIN|1<<SWITCH9_PIN|1<<SWITCH10_PIN|1<<SWITCH11_PIN);
	SWITCH1_7_PORT_DIR &= ~(1<<SWITCH1_PIN|1<<SWITCH2_PIN|1<<SWITCH3_PIN|1<<SWITCH4_PIN|1<<SWITCH5_PIN|1<<SWITCH6_PIN|1<<SWITCH7_PIN);
	SWITCH1_7_PORT_PULLUP |= (1<<SWITCH1_PIN|1<<SWITCH2_PIN|1<<SWITCH3_PIN|1<<SWITCH4_PIN|1<<SWITCH5_PIN|1<<SWITCH6_PIN|1<<SWITCH7_PIN);
}

/**
* Init port pins directions
*/
void ioInit(void)
{
	initOutputPins();
	initInputPins();
}

/**
 * Set output of device
 * \param outputs - if bit is set appropriate output will be set, if bit is not 
 *					set no output change occur i.e outputs=0x03 sets outputs 1 and 2
 *                                                                    
 * Refer to \ref ioResetOutput for output reset
 */
void ioSetOutput(uint16_t outputs)
{
	OUTPUT1_6_PORT |= (outputs & 0x3F)<<OUTPUT1_PIN;
	OUTPUT7_8_PORT |= (outputs & 0xC0)<<(6-OUTPUT7_PIN);
}

/**
 * Set output of device
 * \param outputs - if bit is set appropriate output will be set, if bit is not
 *					set no output change occur i.e outputs=0x03 sets outputs 1 and 2 
 *                                                                     
 * Refer to \ref ioSetOutput for output setting 
 */
void ioResetOutput(uint16_t outputs)
{
	OUTPUT1_6_PORT &= ~((outputs & 0x3F)<<OUTPUT1_PIN);
	OUTPUT7_8_PORT &= ~((outputs & 0xC0)<<(6-OUTPUT7_PIN));
}

/**
 * Set output of device
 * \param outputs - if bit is set appropriate output will be set, if bit is not
 *					set no output change occur i.e outputs=0x03 sets outputs 1 and 2 
 *                                                                     
 * Refer to \ref ioSetOutput for output setting 
 */
void ioToggleOutput(uint16_t outputs)
{
	OUTPUT1_6_PORT ^= (outputs & 0x3F)<<OUTPUT1_PIN;
	OUTPUT7_8_PORT ^= (outputs & 0xC0)<<(6-OUTPUT7_PIN);
}

/**
* Returns inputs state
* if bit is set means input is high if bit not set input is reset i.e. 0x0101 means that inputs 9 and 1 are high
*/
uint16_t ioGetInputs() 
{
	return (((SWITCH8_11_PORT<<(7-SWITCH8_PIN)) & 0x0780)|((SWITCH1_7_PORT>>(SWITCH1_PIN)) & 0x007F));
}

/**
* Returns output state
* if bit is set means input is high if bit not set input is reset i.e. 0x11 means that outputs 5 and 1 are high
*/
uint8_t ioGetOutputs()
{
	return ((OUTPUT1_6_PORT>>OUTPUT1_PIN) & 0x3F)|((OUTPUT7_8_PORT<<(6-OUTPUT7_PIN)) & 0xC0);
}
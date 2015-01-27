/*
 * io_pins.c
 *
 * Created: 2015-01-16 23:02:36
 *  Author: Marcin Bajer
 */ 

#include "global.h"

#define RELAY1_PIN 0
#define RELAY2_PIN 1
#define RELAY3_PIN 2
#define RELAY4_PIN 3
#define RELAY5_PIN 4
#define RELAY6_PIN 5
#define RELAY7_PIN 6
#define RELAY8_PIN 7
#define RELAY1_6_PORT_DIR DDRB
#define RELAY1_6_PORT PORTB
#define RELAY7_8_PORT_DIR DDRD
#define RELAY7_8_PORT PORTD

//1wire Portc.3

/**
* Switches are isolated with transoptors
*/
#define SWITCH1_PIN 2
#define SWITCH2_PIN 3
#define SWITCH3_PIN 4
#define SWITCH4_PIN 5
#define SWITCH1_4_PORT_DIR DDRD
#define SWITCH1_4_PORT PIND
#define SWITCH5_PIN 4
#define SWITCH6_PIN 5
#define SWITCH5_6_PORT_DIR DDRC
#define SWITCH5_6_PORT PINC

/**
* Switches are NOT isolated with transoptors                                                                     
*/
#define INPUT7_PIN 0
#define INPUT8_PIN 1
#define INPUT9_PIN 2
#define INPUT7_9_PORT_DIR DDRC
#define INPUT7_9_PORT PORTC

void initOutputPins(void)
{
	RELAY1_6_PORT_DIR |= (1<<RELAY1_PIN|1<<RELAY2_PIN|1<<RELAY3_PIN|1<<RELAY4_PIN|1<<RELAY5_PIN|1<<RELAY6_PIN);
	RELAY7_8_PORT_DIR |= (1<<RELAY7_PIN|1<<RELAY8_PIN);
}

void initInputPins(void)
{
	SWITCH1_4_PORT_DIR &= ~(1<<SWITCH1_PIN|1<<SWITCH2_PIN|1<<SWITCH3_PIN|1<<SWITCH4_PIN);
	SWITCH5_6_PORT_DIR &= ~(1<<SWITCH5_PIN|1<<SWITCH6_PIN);
	INPUT7_9_PORT_DIR &= ~(1<<INPUT7_PIN|1<<INPUT8_PIN|1<<INPUT9_PIN);
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
	RELAY1_6_PORT |= (outputs & 0x3F)<<RELAY1_PIN;
	RELAY7_8_PORT |= (outputs & 0xC0)<<(6-RELAY7_PIN);
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
	RELAY1_6_PORT &= ~((outputs & 0x3F)<<RELAY1_PIN);
	RELAY7_8_PORT &= ~((outputs & 0xC0)<<(6-RELAY7_PIN));
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
	RELAY1_6_PORT ^= (outputs & 0x3F)<<RELAY1_PIN;
	RELAY7_8_PORT ^= (outputs & 0xC0)<<(6-RELAY7_PIN);
}

/**
* Returns inputs state
* if bit is set means input is high if bit not set input is reset i.e. 0x0101 means that inputs 9 and 1 are high
*/
uint16_t ioGetInputs() 
{
	return ~((SWITCH1_4_PORT>>SWITCH1_PIN) & 0x000F)|((SWITCH5_6_PORT<<(4-SWITCH5_PIN)) & 0x0030)|((INPUT7_9_PORT<<(6-INPUT7_PIN)) & 0x01C0);
}

/**
* Returns output state
* if bit is set means input is high if bit not set input is reset i.e. 0x11 means that outputs 5 and 1 are high
*/
uint8_t ioGetOutputs()
{
	return ((RELAY1_6_PORT>>RELAY1_PIN) & 0x3F)|((RELAY7_8_PORT<<(6-RELAY7_PIN)) & 0xC0);
}
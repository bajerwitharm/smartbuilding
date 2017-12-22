/*
 * triggers.h
 *
 * Created: 1/29/2015 1:43:31 PM
 *  Author: Marcin Bajer (https://github.com/bajerwitharm)
 */ 


#ifndef TRIGGERS_H_
#define TRIGGERS_H_
#include "conf_board.h"
#include "timer.h"


//maps physical relays on the board with logical abstraction layer outputs
#define RELAY_1 0x04
#define RELAY_2 0x80
#define RELAY_3 0x01
#define RELAY_4 0x02
#define RELAY_5 0x10
#define RELAY_6 0x20
#define RELAY_7 0x08
#define RELAY_8 0x40
//const uint8_t RELAYS[]={RELAY_1,RELAY_2,RELAY_3,RELAY_4,RELAY_5,RELAY_6,RELAY_7,RELAY_8};

#define INPUT_1 0x0001
#define INPUT_2 0x0002
#define INPUT_3 0x0004
#define INPUT_4 0x0008
#define INPUT_5 0x0010
#define INPUT_6 0x0040
#define INPUT_7 0x0080
#define INPUT_8 0x0100
#define INPUT_9 0x0200
#define INPUT_10 0x0400
#define INPUT_11 0x0020

#pragma pack(1)

typedef uint16_t inputs_t;
#ifndef RELAY_16_BOARD
typedef uint8_t outputs_t;
#else
typedef uint16_t outputs_t;
#endif
typedef uint16_t states_t;

typedef struct {
#ifndef RELAY_16_BOARD
	inputs_t input_on;
	inputs_t input_off;
#endif
	outputs_t output_on;
	outputs_t output_off;
#ifndef NO_TRIGGERS
	states_t state_on;
	states_t state_off;
#endif
} activator_t;

typedef struct {
	outputs_t output_on;
	outputs_t output_off;
	outputs_t output_toggle;
#ifndef RELAY_16_BOARD
	uint8_t padding;
#endif
#ifndef NO_TRIGGERS
	states_t state_on;
	states_t state_off;
	states_t state_toggle;
#endif
} actuator_t;

typedef struct {
	activator_t activator;
	actuator_t actuator;
	struct {
		uint16_t time_preload;
		uint16_t time_current;
	} timer;
} trigger_t;

typedef struct {
#ifndef RELAY_16_BOARD
	inputs_t inputs;
#endif
	outputs_t outputs;
#ifndef NO_TRIGGERS
	states_t states;
#endif
} info_t;

#pragma pack()
const uint8_t getNumberOfTriggers();
void activateTrigger(const actuator_t* const actuator_p);
void processTriggers();
states_t getState();
extern trigger_t triggers[];

#endif /* TRIGGERS_H_ */
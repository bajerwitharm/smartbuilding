/*
* triggers.c
*
* Created: 1/29/2015 1:43:56 PM
*  Author: Marcin Bajer (https://github.com/bajerwitharm)
*
* Control logic for application (specifies how outputs react on inputs). 
* 
* Every trigger contains its activator (condition which triggers action defined by trigger), actuator (action which is done after trigger) and timer (count down timer between action trigger and actuator action).
* Activator - trigger count down when all conditions are satisfied input_off set to 1 are off input_on set to 1 are on...etc
* Actuator - action to be done after activator is satisfied and count down timer is passed. Output_off set to 1 will be switched off, output_on set to 1 will be switched on..etc
* Timer - if activator condition is satisfied count down starts, if during count down activator will be not satisfied count down resets to time_preload
*/

#include "global.h"
#include "triggers.h"
#include "io_pins.h"
#include "usart.h"

static states_t device_state = 0;

void setState(const states_t bits_to_activate)
{
	device_state |= bits_to_activate;
}

void resetState(const states_t bits_to_deactivate)
{
	device_state &= ~bits_to_deactivate;
}

void toggleState(const states_t bits_to_toggle)
{
	device_state ^= bits_to_toggle;
}

states_t getState()
{
	return device_state;
}     

/*
 *			   USB
 *			|		|	+ Kuchnia1 -   + Pokoj1 -     + Przedpokoj -      + £azienkaLustro -      + £azienkaSufit -       + RESET -			+5V
 *						(Input1 PC0)   (Input2 PC1)      (Input3 PC2)           (Input4 PC3)         (Input5 PC4)     (Input6 PC6) RESET 
 *
 *
 *
 *				        + Kuchnia R-   +  Pokój R-    + Przedpokoj R -     + Przedpokoj R -             + I2C -            GND              +5V
 *                      (Input7 PD2)   (Input8 PD3)     (Input9 PD4)        (Input10 PD5)             (Input11 PC5) 
 */


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
#define INPUT_11 0x0010

#define KITCHEN_LAMP_SWITCH_1 INPUT_1
#define ROOM_LAMP_SWITCH_1    INPUT_2
#define CORRIDOR_LAMP_SWITCH  INPUT_3
#define TOILET_LAMP_SWITCH_MIRROR  INPUT_4
#define TOILET_LAMP_SWITCH_MAIN  INPUT_5
//#define TOILET_LAMP_SWITCH_1 INPUT_6
//#define TOILET_LAMP_SWITCH_2 INPUT_7
#define KITCHEN_LAMP_MOTION INPUT_7
#define ROOM_LAMP_MOTION	INPUT_8
#define CORRIDOR_LAMP_MOTION INPUT_9





#define KITCHEN_LAMP_OUTPUT_1 RELAY_1
#define KITCHEN_LAMP_OUTPUT_2 RELAY_2
#define ROOM_LAMP_OUTPUT_1 RELAY_3
#define ROOM_LAMP_OUTPUT_2 RELAY_4
#define CORRIDOR_LAMP_OUTPUT RELAY_5
#define TOILET_LAMP_OUTPUT_1 RELAY_6
#define TOILET_LAMP_OUTPUT_2 RELAY_7


#define KITCHEN_LAMP_1_MANUAL_SWTICH_ON 0x01
#define KITCHEN_LAMP_2_MANUAL_SWTICH_ON 0x02
#define KITCHEN_LAMP_MONTION_SWTICH_ON 0x04
#define ROOM_LAMP_1_MANUAL_SWTICH_ON 0x08
#define ROOM_LAMP_2_MANUAL_SWTICH_ON 0x10
#define ROOM_LAMP_MONTION_SWTICH_ON 0x20
#define CORRIDOR_LAMP_MANUAL_SWTICH_ON 0x40
#define CORRIDOR_LAMP_MONTION_SWTICH_ON 0x80
#define HEARDBEAT_STATE 0x200

#define ON_AFTER_MOTION_TIME 10000
#define HEARDBEAT_TIME 25000


//#define MOTION_DETECTION_ENABLED

#define SHORT_CLICK_TIME 200
#define LONG_CLICK_TIME 1500

trigger_t triggers[] = {
	// Heardbeat1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = 0x0000,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x0000,
			.state_off = HEARDBEAT_STATE,
		},
		.actuator = {
			.output_off = 0x0000,
			.output_toggle = 0x0000,
			.output_on = 0x00,
			.state_on =  0x00,
			.state_off =  0x00,
			.state_toggle = HEARDBEAT_STATE,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(HEARDBEAT_TIME),
			.time_current = 0,
		}
	},
	// Heardbeat2
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = 0x0000,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = HEARDBEAT_STATE,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x0000,
			.output_toggle = 0x0000,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = HEARDBEAT_STATE,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(HEARDBEAT_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch kitchen lamp 1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = KITCHEN_LAMP_SWITCH_1,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = KITCHEN_LAMP_OUTPUT_1,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch kitchen lamp 2
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = KITCHEN_LAMP_SWITCH_1,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = KITCHEN_LAMP_OUTPUT_2,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
			.time_current = 0,
		}
	},
	//// ON/OFF switch room lamp 1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = ROOM_LAMP_SWITCH_1,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = ROOM_LAMP_OUTPUT_1,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch room lamp 2
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = ROOM_LAMP_SWITCH_1,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = ROOM_LAMP_OUTPUT_2,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch corridor lamp
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_LAMP_SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = CORRIDOR_LAMP_OUTPUT,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch outside Toilet lamp 1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = TOILET_LAMP_SWITCH_MAIN,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = TOILET_LAMP_OUTPUT_1,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch outside Toilet lamp 2
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = TOILET_LAMP_SWITCH_MAIN,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = TOILET_LAMP_OUTPUT_2,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch inside Toilet lamp 1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = TOILET_LAMP_SWITCH_MIRROR,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = TOILET_LAMP_OUTPUT_2,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch inside Toilet lamp 2
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = TOILET_LAMP_SWITCH_MIRROR,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = TOILET_LAMP_OUTPUT_1,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
			.time_current = 0,
		}
	},
	#ifdef MOTION_DETECTION_ENABLED
	// ON motion detector kitchen lamp
	{
		.activator = {
			.input_on = KITCHEN_LAMP_MOTION,
			.input_off = 0x0000,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = KITCHEN_LAMP_1_MANUAL_SWTICH_ON|KITCHEN_LAMP_2_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = 0x00,
			.output_on = KITCHEN_LAMP_OUTPUT_1|KITCHEN_LAMP_OUTPUT_2,
			.state_on = KITCHEN_LAMP_MONTION_SWTICH_ON,
			.state_off = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// OFF motion detector kitchen lamp
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = KITCHEN_LAMP_MOTION,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = KITCHEN_LAMP_MONTION_SWTICH_ON,
			.state_off = KITCHEN_LAMP_1_MANUAL_SWTICH_ON|KITCHEN_LAMP_2_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = KITCHEN_LAMP_OUTPUT_1|KITCHEN_LAMP_OUTPUT_2,
			.output_toggle = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = KITCHEN_LAMP_MONTION_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME),
			.time_current = 0,
		}
	},
	// ON motion detector room lamp
	{
		.activator = {
			.input_on = ROOM_LAMP_MOTION,
			.input_off = 0x0000,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = ROOM_LAMP_1_MANUAL_SWTICH_ON|ROOM_LAMP_2_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = 0x00,
			.output_on = ROOM_LAMP_OUTPUT_1|ROOM_LAMP_OUTPUT_2,
			.state_on = ROOM_LAMP_MONTION_SWTICH_ON,
			.state_off = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// OFF motion detector kitchen lamp
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = ROOM_LAMP_MOTION,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = ROOM_LAMP_MONTION_SWTICH_ON,
			.state_off = ROOM_LAMP_1_MANUAL_SWTICH_ON|ROOM_LAMP_2_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = ROOM_LAMP_OUTPUT_1|ROOM_LAMP_OUTPUT_2,
			.output_toggle = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = ROOM_LAMP_MONTION_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME),
			.time_current = 0,
		}
	},
	// ON motion detector corridor lamp
	{
		.activator = {
			.input_on = CORRIDOR_LAMP_MOTION,
			.input_off = 0x0000,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = CORRIDOR_LAMP_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = 0x00,
			.output_on = CORRIDOR_LAMP_OUTPUT,
			.state_on = CORRIDOR_LAMP_MONTION_SWTICH_ON,
			.state_off = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// OFF motion detector corridor lamp
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_LAMP_MOTION,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = CORRIDOR_LAMP_MONTION_SWTICH_ON,
			.state_off = CORRIDOR_LAMP_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = CORRIDOR_LAMP_OUTPUT,
			.output_toggle = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = CORRIDOR_LAMP_MONTION_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME),
			.time_current = 0,
		}
	},
	
	#endif
};

const uint8_t getNumberOfTriggers()
{
	static const uint8_t number_of_triggers =  sizeof(triggers) / sizeof(triggers[0]);
	return number_of_triggers;
}

bool stateOffVerified(const uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.state_off & ~device_state) == triggers[trigger_index].activator.state_off);
}

bool stateOnVerified(const uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.state_on & device_state) == triggers[trigger_index].activator.state_on);
}

bool inputOffVerified(const uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.input_off & ~ioGetInputs()) == triggers[trigger_index].activator.input_off);
}

bool inputOnVerified(const uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.input_on & ioGetInputs()) == triggers[trigger_index].activator.input_on);
}

bool outputOffVerified(const uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.output_off & ~ioGetOutputs()) == triggers[trigger_index].activator.output_off);
}

bool outputOnVerified(const uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.output_on & ioGetOutputs()) == triggers[trigger_index].activator.output_on);
}

void activateTrigger(const actuator_t* const actuator_p)
{
	ioSetOutput(actuator_p->output_on);
	ioResetOutput(actuator_p->output_off);
	ioToggleOutput(actuator_p->output_toggle);
	setState(actuator_p->state_on);
	resetState(actuator_p->state_off);
	toggleState(actuator_p->state_toggle);
}

void processTrigger(const uint8_t trigger_index) {
	static uint8_t oldState=0;
	if (
	inputOffVerified(trigger_index) &&
	inputOnVerified(trigger_index) &&
	outputOffVerified(trigger_index) &&
	outputOnVerified(trigger_index) &&
	stateOffVerified(trigger_index) &&
	stateOnVerified(trigger_index)
	) {
		//all condition satisfied, check if action should be triggered
		if (triggers[trigger_index].timer.time_current == triggers[trigger_index].timer.time_preload) {
			//already count down and new state should be activated
			activateTrigger(&triggers[trigger_index].actuator);
			usartSendAction(&triggers[trigger_index], BUS_MASTER_ADDRESS);
			triggers[trigger_index].timer.time_current++;
		}
		//check if should count down to activate new output state
		if (triggers[trigger_index].timer.time_current < triggers[trigger_index].timer.time_preload)
		{
			triggers[trigger_index].timer.time_current++;
		}
		oldState |=((uint16_t)1)<<trigger_index;
	}
	else
	{
		//condition not valid, set countdown counter to 0
		if (!(oldState&(((uint16_t)1)<<trigger_index))) {
			triggers[trigger_index].timer.time_current = 0;
		}
		oldState &=~(((uint16_t)1)<<trigger_index);
	}

}

void processTriggers() {
	for (uint8_t i=0;i<getNumberOfTriggers();i++) {
		processTrigger(i);
	}	
}
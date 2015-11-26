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
*			|		|	+ RuchLewy -   + RuchPrawy -     + Przycisk -        + Brak -      + Brak -            + RESET -			+5V
*						(Input1 PC0)   (Input2 PC1)      (Input3 PC2)      (Input4 PC3)  (Input5 PC4)     (Input6 PC6) RESET
*
*
*
*				        + Brak -        +  Brak R-        + Brak -            + Brak -         + I2C -            GND               +5V
*                      (Input7 PD2)   (Input8 PD3)     (Input9 PD4)        (Input10 PD5)   (Input11 PC5)
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

#define MOTION_2LEFT INPUT_7
#define MOTION_2RIGHT INPUT_8
#define CORRIDOR_2SWITCH INPUT_6
#define MOTION_3 INPUT_4
#define CORRIDOR_3SWITCH INPUT_5

#define LAMP_2LEFT_OUTPUT_1 RELAY_1
#define LAMP_2LEFT_OUTPUT_2 RELAY_2
#define LAMP_2RIGHT_OUTPUT_1 RELAY_3
#define LAMP_2RIGHT_OUTPUT_2 RELAY_4
#define POWER_24V_OUTPUT RELAY_5
#define LAMP_3_OUTPUT_1 RELAY_6
#define LAMP_3_OUTPUT_2 RELAY_7

#define HEARDBEAT_STATE 0x01
#define DISABLE_2MONTION 0x02
#define BRIGHT_2LIGHT 0x04
#define DISABLE_3MONTION 0x08
#define BRIGHT_3LIGHT 0x10

#define ON_AFTER_MOTION_TIME_1 50000
#define ON_AFTER_MOTION_TIME_2 50000
#define MOTION_REACTION_TIME 100
#define HEARDBEAT_TIME 60000
#define OFF_TIME_24V 5000


#define MOTION_DETECTION_ENABLED
#define MANUAL_ON_ENABLED

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
	// 24V_OFF relay
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = 0x0000,
			.output_off = 0x00,
			.output_on = POWER_24V_OUTPUT,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = POWER_24V_OUTPUT,
			.output_toggle = 0x0000,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(HEARDBEAT_TIME),
			.time_current = 0,
		}
	},
	#ifdef MANUAL_ON_ENABLED
	// Manual ON/OFF lamps 1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_2SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = LAMP_2LEFT_OUTPUT_1|LAMP_2RIGHT_OUTPUT_1,
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
	// Manual ON/OFF lamps 2
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_2SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = LAMP_2LEFT_OUTPUT_2|LAMP_2RIGHT_OUTPUT_2,
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
	// Manual ON/OFF lamps 3
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_3SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = LAMP_3_OUTPUT_2,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = 0x00,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
			.time_current = 0,
		}},
		// Manual ON/OFF lamps 3
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = CORRIDOR_3SWITCH,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = LAMP_3_OUTPUT_1,
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
		#endif
		#ifdef MOTION_DETECTION_ENABLED
		// ON motion detector left low light
		{
			.activator = {
				.input_on = MOTION_2LEFT,
				.input_off = 0x0000,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = 0x00,
				.output_on = LAMP_2LEFT_OUTPUT_1,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
				.time_current = 0,
			}
		},
		// ON motion detector left bright light
		{
			.activator = {
				.input_on = MOTION_2LEFT,
				.input_off = 0x0000,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = BRIGHT_2LIGHT,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = 0x00,
				.output_on = LAMP_2LEFT_OUTPUT_2,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
				.time_current = 0,
			}
		},
		// OFF motion detector left
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = MOTION_2LEFT,
				.output_off = 0x00,
				.output_on = LAMP_2LEFT_OUTPUT_1,
				.state_on = 0x00,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = LAMP_2LEFT_OUTPUT_1,
				.output_toggle = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_1),
				.time_current = 0,
			}
		},
		// OFF motion detector left
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = MOTION_2LEFT,
				.output_off = 0x00,
				.output_on = LAMP_2LEFT_OUTPUT_2,
				.state_on = 0x00,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = LAMP_2LEFT_OUTPUT_2,
				.output_toggle = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_2),
				.time_current = 0,
			}
		},
		
		// ON motion detector right low light
		{
			.activator = {
				.input_on = MOTION_2RIGHT,
				.input_off = 0x0000,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = 0x00,
				.output_on = LAMP_2RIGHT_OUTPUT_1,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
				.time_current = 0,
			}
		},
		// ON motion detector left bright light
		{
			.activator = {
				.input_on = MOTION_2RIGHT,
				.input_off = 0x0000,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = BRIGHT_2LIGHT,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = 0x00,
				.output_on = LAMP_2RIGHT_OUTPUT_2,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
				.time_current = 0,
			}
		},
		// OFF motion detector left
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = MOTION_2RIGHT,
				.output_off = 0x00,
				.output_on = LAMP_2RIGHT_OUTPUT_1,
				.state_on = 0x00,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = LAMP_2RIGHT_OUTPUT_1,
				.output_toggle = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_1),
				.time_current = 0,
			}
		},
		// OFF motion detector left
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = MOTION_2RIGHT,
				.output_off = 0x00,
				.output_on = LAMP_2RIGHT_OUTPUT_2,
				.state_on = 0x00,
				.state_off = DISABLE_2MONTION,
			},
			.actuator = {
				.output_off = LAMP_2RIGHT_OUTPUT_2,
				.output_toggle = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_2),
				.time_current = 0,
			}
		},
		// ON motion detector right low light
		{
			.activator = {
				.input_on = MOTION_3,
				.input_off = 0x0000,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = DISABLE_3MONTION,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = 0x00,
				.output_on = LAMP_3_OUTPUT_1,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
				.time_current = 0,
			}
		},
		// ON motion detector left bright light
		{
			.activator = {
				.input_on = MOTION_3,
				.input_off = 0x0000,
				.output_off = 0x00,
				.output_on = 0x00,
				.state_on = BRIGHT_3LIGHT,
				.state_off = DISABLE_3MONTION,
			},
			.actuator = {
				.output_off = 0x00,
				.output_toggle = 0x00,
				.output_on = LAMP_3_OUTPUT_2,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
				.time_current = 0,
			}
		},
		// OFF motion detector left
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = MOTION_3,
				.output_off = 0x00,
				.output_on = LAMP_3_OUTPUT_1,
				.state_on = 0x00,
				.state_off = DISABLE_3MONTION,
			},
			.actuator = {
				.output_off = LAMP_3_OUTPUT_1,
				.output_toggle = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_1),
				.time_current = 0,
			}
		},
		
		// OFF motion detector left
		{
			.activator = {
				.input_on = 0x0000,
				.input_off = MOTION_3,
				.output_off = 0x00,
				.output_on = LAMP_3_OUTPUT_2,
				.state_on = 0x00,
				.state_off = DISABLE_3MONTION,
			},
			.actuator = {
				.output_off = LAMP_3_OUTPUT_2,
				.output_toggle = 0x00,
				.output_on = 0x00,
				.state_on = 0x00,
				.state_off = 0x00,
			},
			.timer = {
				.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_2),
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
		static uint32_t oldState=0;
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
			oldState |=((uint32_t)1)<<trigger_index;
		}
		else
		{
			//condition not valid, set countdown counter to 0
			if (!(oldState&(((uint32_t)1)<<trigger_index))) {
				triggers[trigger_index].timer.time_current = 0;
			}
			oldState &=~(((uint32_t)1)<<trigger_index);
		}

	}

	void processTriggers() {
		for (uint8_t i=0;i<getNumberOfTriggers();i++) {
			processTrigger(i);
		}
	}
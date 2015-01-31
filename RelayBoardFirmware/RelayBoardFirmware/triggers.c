/*
* triggers.c
*
* Created: 1/29/2015 1:43:56 PM
*  Author: Marcin Bajer
*/

#include "global.h"
#include "triggers.h"
#include "io_pins.h"
#include "usart.h"

static uint8_t device_state = 0;

void setState(uint8_t bits_to_activate)
{
	device_state |= bits_to_activate;
}

void resetState(uint8_t bits_to_deactivate)
{
	device_state &= ~bits_to_deactivate;
}

void toggleState(uint8_t bits_to_toggle)
{
	device_state ^= bits_to_toggle;
}

#define KITCHEN_LAMP_SWITCH 0x0001
#define KITCHEN_LAMP_MOTION 0x0002
#define KITCHEN_LAMP_RELAY_1 0x01
#define KITCHEN_LAMP_RELAY_2 0x02
#define KITCHEN_LAMP_1_MANUAL_SWTICH_ON 0x01
#define KITCHEN_LAMP_2_MANUAL_SWTICH_ON 0x02
#define KITCHEN_LAMP_3_MONTION_SWTICH_ON 0x04
#define ON_AFTER_MOTION_TIME 10000

#define SHORT_CLICK_TIME 150
#define LONG_CLICK_TIME 1500

trigger_t triggers[] = {
	// ON/OFF switch kitchen lamp 1
	{
		.activator = {
			.input_off = 0x0000,
			.input_on = KITCHEN_LAMP_SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = KITCHEN_LAMP_RELAY_1,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = KITCHEN_LAMP_1_MANUAL_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
			.time_current = 0,
		}
	},
	// ON/OFF switch kitchen lamp 2
	{
		.activator = {
			.input_off = 0x0000,
			.input_on = KITCHEN_LAMP_SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = KITCHEN_LAMP_RELAY_2,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
			.state_toggle = KITCHEN_LAMP_2_MANUAL_SWTICH_ON
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
			.time_current = 0,
		}
	},
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
			.output_on = KITCHEN_LAMP_RELAY_1|KITCHEN_LAMP_RELAY_2,
			.state_on = KITCHEN_LAMP_3_MONTION_SWTICH_ON,
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
			.state_on = KITCHEN_LAMP_3_MONTION_SWTICH_ON,
			.state_off = KITCHEN_LAMP_1_MANUAL_SWTICH_ON|KITCHEN_LAMP_2_MANUAL_SWTICH_ON,
		},
		.actuator = {
			.output_off = KITCHEN_LAMP_RELAY_1|KITCHEN_LAMP_RELAY_2,
			.output_toggle = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = KITCHEN_LAMP_3_MONTION_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME),
			.time_current = 0,
		}
	},
};

static const uint8_t number_of_triggers =  sizeof(triggers) / sizeof(triggers[0]);

uint8_t getNumberOfTriggers()
{
	return number_of_triggers;
}

bool stateOffVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.state_off & ~device_state) == triggers[trigger_index].activator.state_off);
}

bool stateOnVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.state_on & device_state) == triggers[trigger_index].activator.state_on);
}

bool inputOffVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.input_off & ~ioGetInputs()) == triggers[trigger_index].activator.input_off);
}

bool inputOnVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.input_on & ioGetInputs()) == triggers[trigger_index].activator.input_on);
}

bool outputOffVerified(uint8_t trigger_index)
{
	return ((triggers[trigger_index].activator.output_off & ~ioGetOutputs()) == triggers[trigger_index].activator.output_off);
}

bool outputOnVerified(uint8_t trigger_index)
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

void processTrigger(uint8_t trigger_index) {
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
	}
	else
	{
		//condition not valid, set countdown counter to 0
		triggers[trigger_index].timer.time_current = 0;
	}
}

void processTriggers() {
	for (uint8_t i=0;i<number_of_triggers;i++) {
		processTrigger(i);
	}
}
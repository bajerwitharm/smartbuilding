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
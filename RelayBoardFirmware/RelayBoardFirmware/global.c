/*
* global.c
*
* Created: 2015-11-30 09:32:23
*  Author: PLMABAJ
*/

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

#include "global.h"

#define HEARDBEAT_TIME 60000
#define HEARDBEAT_STATE 0x01
#define STAIRCASE
//#define FIRST_FLOOR

#ifdef STAIRCASE

#define MOTION_LEFT INPUT_7
#define MOTION_RIGHT INPUT_8
#define CORRIDOR_SWITCH INPUT_9
#define MOTION_UP INPUT_10
#define CORRIDOR_2SWITCH INPUT_5

#define LAMP_LEFT_OUTPUT_1 RELAY_1
#define LAMP_LEFT_OUTPUT_2 RELAY_2
#define LAMP_RIGHT_OUTPUT_1 RELAY_3
#define LAMP_RIGHT_OUTPUT_2 RELAY_4
#define POWER_24V_OUTPUT RELAY_5
#define LAMP_UP_OUTPUT_1 RELAY_6
#define LAMP_UP_OUTPUT_2 RELAY_7

#define DISABLE_MONTION 0x02
#define BRIGHT_LIGHT 0x04
#define DISABLE_UP_MONTION 0x08
#define BRIGHT_UP_LIGHT 0x10

#define ON_AFTER_MOTION_TIME_1 50000
#define ON_AFTER_MOTION_TIME_2 50000
#define MOTION_REACTION_TIME 100
#define OFF_TIME_24V 5000

#else
#define ON_AFTER_MOTION_TIME 10000
#define KITCHEN_LAMP_SWITCH_1 INPUT_1
#define KITCHEN_LAMP_SWITCH_2 INPUT_2
#define ROOM_LAMP_SWITCH_1	INPUT_3
#define ROOM_LAMP_SWITCH_2	INPUT_4
#define CORRIDOR_LAMP_SWITCH INPUT_5
#define TOILET_LAMP_SWITCH_1 INPUT_6
#define TOILET_LAMP_SWITCH_2 INPUT_7
#define KITCHEN_LAMP_MOTION INPUT_8
#define ROOM_LAMP_MOTION	INPUT_9
#define CORRIDOR_LAMP_MOTION INPUT_10

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

#endif

#define MOTION_DETECTION_ENABLED
#define MANUAL_ON_ENABLED

#define SHORT_CLICK_TIME 200
#define LONG_CLICK_TIME 1500
#define VERY_LONG_CLICK_TIME 15000

trigger_t triggers[] = {
	// Heardbeat1
	{
		.activator = {
			.state_off = HEARDBEAT_STATE,
		},
		.actuator = {
			.state_toggle = HEARDBEAT_STATE,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(HEARDBEAT_TIME),
		}
	},
	// Heardbeat2
	{
		.activator = {
			.state_on = HEARDBEAT_STATE,
		},
		.actuator = {
			.state_toggle = HEARDBEAT_STATE,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(HEARDBEAT_TIME),
		}
	},
	#ifdef STAIRCASE
	// 24V_OFF relay (if output is set to ON it will be automatically back to OFF after OFF_TIME_24V
	{
		.activator = {
			.output_on = POWER_24V_OUTPUT,
		},
		.actuator = {
			.output_off = POWER_24V_OUTPUT,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(OFF_TIME_24V),
		}
	},
	#ifdef MANUAL_ON_ENABLED
	#ifdef MOTION_DETECTION_ENABLED
	// Manual ON/OFF lamps low light (when motion is disabled corridor switch only allows to ON light)
	{
		.activator = {
			.input_off = CORRIDOR_SWITCH,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_LEFT_OUTPUT_1|LAMP_RIGHT_OUTPUT_1,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),			
		}
	},
	// Manual ON/OFF lamps bright light (when motion is disabled corridor switch only allows to ON light) 
	{
		.activator = {		
			.input_off = CORRIDOR_SWITCH,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_LEFT_OUTPUT_2|LAMP_RIGHT_OUTPUT_2,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),			
		}
	},
	// Manual ON/OFF lamps 3
	{
		.activator = {
			.input_off = CORRIDOR_2SWITCH,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_UP_OUTPUT_2,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
		}
	},
	// Manual ON/OFF lamps 3
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_2SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = DISABLE_MONTION,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = 0x00,
			.output_on = LAMP_UP_OUTPUT_1,
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
	// Manual ON/OFF lamps 1
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = DISABLE_MONTION,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = LAMP_LEFT_OUTPUT_1|LAMP_RIGHT_OUTPUT_1,
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
			.input_off = CORRIDOR_SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = DISABLE_MONTION,
			.state_off = 0x00,
		},
		.actuator = {
			.output_off = 0x00,
			.output_toggle = LAMP_LEFT_OUTPUT_2|LAMP_RIGHT_OUTPUT_2,
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
	#ifndef FIRST_FLOOR
	// Manual ON/OFF lamps 3
	{
		.activator = {			
			.input_off = CORRIDOR_2SWITCH,
			.state_on = DISABLE_MONTION,			
		},
		.actuator = {
			.output_toggle = LAMP_UP_OUTPUT_2,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),			
		}
	},
	// Manual ON/OFF lamps 3
	{
		.activator = {
			.input_off = CORRIDOR_2SWITCH,
			.state_on = DISABLE_MONTION,
		},
		.actuator = {
			.output_toggle = LAMP_UP_OUTPUT_1,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),
		}
	},
	#endif
	#endif
	#ifdef MOTION_DETECTION_ENABLED
	// ON motion detector left low light
	{
		.activator = {
			.input_on = MOTION_LEFT,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_LEFT_OUTPUT_1,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
		}
	},
	// ON motion detector left bright light
	{
		.activator = {
			.input_on = MOTION_LEFT,
			.state_on = BRIGHT_LIGHT,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_LEFT_OUTPUT_2,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
		}
	},
	// OFF motion detector left
	{
		.activator = {
			.input_off = MOTION_LEFT,
			.output_on = LAMP_LEFT_OUTPUT_1,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_off = LAMP_LEFT_OUTPUT_1,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_1),
		}
	},
	// OFF motion detector left
	{
		.activator = {
			.input_off = MOTION_LEFT,
			.output_on = LAMP_LEFT_OUTPUT_2,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_off = LAMP_LEFT_OUTPUT_2,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_2),
		}
	},
	
	// ON motion detector right low light
	{
		.activator = {
			.input_on = MOTION_RIGHT,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_RIGHT_OUTPUT_1,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
		}
	},
	// ON motion detector left bright light
	{
		.activator = {
			.input_on = MOTION_RIGHT,
			.state_on = BRIGHT_LIGHT,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_on = LAMP_RIGHT_OUTPUT_2,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),
		}
	},
	// OFF motion detector left
	{
		.activator = {
			.input_off = MOTION_RIGHT,
			.output_on = LAMP_RIGHT_OUTPUT_1,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_off = LAMP_RIGHT_OUTPUT_1,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_1),
			.time_current = 0,
		}
	},
	// OFF motion detector left
	{
		.activator = {
			.input_off = MOTION_RIGHT,
			.output_on = LAMP_RIGHT_OUTPUT_2,
			.state_off = DISABLE_MONTION,
		},
		.actuator = {
			.output_off = LAMP_RIGHT_OUTPUT_2,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_2),
		}
	},
	// Toggle DISABLE_MOTION
	{
		.activator = {
			.input_on = 0x0000,
			.input_off = CORRIDOR_SWITCH,
			.output_off = 0x00,
			.output_on = 0x00,
			.state_on = 0x00,
			.state_off = 0x00,
		},
		.actuator = {
			.state_toggle = DISABLE_MONTION,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(VERY_LONG_CLICK_TIME),
			.time_current = 0,
		}
	},	
	#ifndef FIRST_FLOOR
	// ON motion detector up low light
	{
		.activator = {
			.input_on = MOTION_UP,
			.state_off = DISABLE_UP_MONTION,
		},
		.actuator = {
			.output_on = LAMP_UP_OUTPUT_1,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),			
		}
	},
	// ON motion detector left bright light
	{
		.activator = {
			.input_on = MOTION_UP,
			.state_on = BRIGHT_UP_LIGHT,
			.state_off = DISABLE_UP_MONTION,
		},
		.actuator = {
			.output_on = LAMP_UP_OUTPUT_2,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(MOTION_REACTION_TIME),			
		}
	},
	// OFF motion detector left
	{
		.activator = {
			.input_off = MOTION_UP,
			.output_on = LAMP_UP_OUTPUT_1,
			.state_off = DISABLE_UP_MONTION,
		},
		.actuator = {
			.output_off = LAMP_UP_OUTPUT_1,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_1),			
		}
	},	
	// OFF motion detector left
	{
		.activator = {
			.input_off = MOTION_UP,
			.output_on = LAMP_UP_OUTPUT_2,
			.state_off = DISABLE_UP_MONTION,
		},
		.actuator = {
			.output_off = LAMP_UP_OUTPUT_2,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(ON_AFTER_MOTION_TIME_2),			
		}
	},
	#endif
	
	#endif
	#else
	// ON/OFF switch kitchen lamp 1
	{
		.activator = {
			.input_on = KITCHEN_LAMP_SWITCH_1,			
		},		
		.actuator = {
			.output_toggle = KITCHEN_LAMP_OUTPUT_1,
			.state_toggle = KITCHEN_LAMP_1_MANUAL_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),			
		}
	},
	// ON/OFF switch kitchen lamp 2
	{
		.activator = {
			.input_on = KITCHEN_LAMP_SWITCH_2,			
		},
		.actuator = {
			.output_toggle = KITCHEN_LAMP_OUTPUT_2,
			.state_toggle = KITCHEN_LAMP_2_MANUAL_SWTICH_ON
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),			
		}
	},
	//// ON/OFF switch room lamp 1
	{
		.activator = {
			.input_on = ROOM_LAMP_SWITCH_1,			
		},
		.actuator = {
			.output_toggle = ROOM_LAMP_OUTPUT_1,
			.state_toggle = ROOM_LAMP_1_MANUAL_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),			
		}
	},
	// ON/OFF switch room lamp 2
	{
		.activator = {
			.input_on = ROOM_LAMP_SWITCH_2,			
		},
		.actuator = {
			.output_toggle = ROOM_LAMP_OUTPUT_2,
			.state_toggle = ROOM_LAMP_2_MANUAL_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),			
		}
	},
	// ON/OFF switch corridor lamp
	{
		.activator = {
			.input_on = CORRIDOR_LAMP_SWITCH,			
		},
		.actuator = {
			.output_toggle = CORRIDOR_LAMP_OUTPUT,			
			.state_toggle = CORRIDOR_LAMP_MANUAL_SWTICH_ON,
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),			
		}
	},
	// ON/OFF switch outside Toilet lamp 1
	{
		.activator = {
			.input_on = TOILET_LAMP_SWITCH_1,			
		},
		.actuator = {
			.output_toggle = TOILET_LAMP_OUTPUT_1,			
		},
		.timer = {
			.time_preload = TIMER_MS_TO_TIMER(SHORT_CLICK_TIME),		
		}
	},
	// ON/OFF switch outside Toilet lamp 2
	//{
	//.activator = {
	//.input_off = 0x0000,
	//.input_on = TOILET_LAMP_SWITCH_1,
	//.output_off = 0x00,
	//.output_on = 0x00,
	//.state_on = 0x00,
	//.state_off = 0x00,
	//},
	//.actuator = {
	//.output_off = 0x00,
	//.output_toggle = TOILET_LAMP_OUTPUT_2,
	//.output_on = 0x00,
	//.state_on = 0x00,
	//.state_off = 0x00,
	//.state_toggle = 0x00,
	//},
	//.timer = {
	//.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
	//.time_current = 0,
	//}
	//},
	// ON/OFF switch inside Toilet lamp 1
	{
		.activator = {
			.input_off = 0x0000,
			.input_on = TOILET_LAMP_SWITCH_2,
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
	//// ON/OFF switch inside Toilet lamp 2
	//{
	//.activator = {
	//.input_off = 0x0000,
	//.input_on = TOILET_LAMP_SWITCH_2,
	//.output_off = 0x00,
	//.output_on = 0x00,
	//.state_on = 0x00,
	//.state_off = 0x00,
	//},
	//.actuator = {
	//.output_off = 0x00,
	//.output_toggle = TOILET_LAMP_OUTPUT_1,
	//.output_on = 0x00,
	//.state_on = 0x00,
	//.state_off = 0x00,
	//.state_toggle = 0x00,
	//},
	//.timer = {
	//.time_preload = TIMER_MS_TO_TIMER(LONG_CLICK_TIME),
	//.time_current = 0,
	//}
	//},
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
	#endif
};

const uint8_t getNumberOfTriggers()
{
	static const uint8_t number_of_triggers =  sizeof(triggers) / sizeof(triggers[0]);
	return number_of_triggers;
}
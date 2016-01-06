#ifndef GLOBAL_H_
#define GLOBAL_H_

#define MAX_ITEM_LEN 25
#define OUTPUTS_NR 8
#define STATES_NR 16
#define INPUTS_NR 11

#define WIFI_SSID "Salwatorska6admin"
#define WIFI_PASSWORD "Administrator@wifi6"
#define MQTT_SERVER_HOST "mqtt.salwatorska.pl"
#define MQTT_SERVER_PORT 1883

/*
*         USB
*        |   |  + RuchLewy -   + RuchPrawy -     + Przycisk -        + Brak -      + Brak -            + RESET -      +5V
*               (Input1 PC0)   (Input2 PC1)      (Input3 PC2)      (Input4 PC3)  (Input5 PC4)     (Input6 PC6) RESET
*
*
*
*               + Brak -        +  Brak R-        + Brak -            + Brak -         + I2C -            GND        +5V
*               (Input7 PD2)   (Input8 PD3)     (Input9 PD4)        (Input10 PD5)   (Input11 PC5)
*/

#define FLAT_10

#ifdef GROUNDFLOOR

#define MQTT_USER "groundfloor"
#define MQTT_PASSWORD "groundfloor"
#define HOST_NAME "groundfloor"
#define MQTT_CONTROL_TOPIC "salwatorska6/groundfloor/control"
#define MQTT_STATUS_TOPIC "salwatorska6/groundfloor/status"
#define MQTT_DEBUG_TOPIC "salwatorska6/groundfloor/debug"

#define RELAY_1 "bulb_0stairs_right_small"
#define RELAY_2 "bulb_0stairs_right_main"
#define RELAY_3 "bulb_0stairs_left_small"
#define RELAY_4 "bulb_0stairs_left_main"
#define RELAY_5 "key_main_door"
#define RELAY_6 "bulb_enerance_small"
#define RELAY_7 "bulb_enerance_main"
#define RELAY_8 "bulb_outside"

#define INPUT_1 ""
#define INPUT_2 ""
#define INPUT_3 ""
#define INPUT_4 ""
#define INPUT_5 ""
#define INPUT_6 ""
#define INPUT_7 "motion_0stairs_right"
#define INPUT_8 "motion_0stairs_left"
#define INPUT_9 "switch_0stairs"
#define INPUT_10 ""
#define INPUT_11 ""

#define STATE_1 "heard_beat_0stairs"
#define STATE_2 "motion_0stairs_off"
#define STATE_3 "bright_light_0stairs"
#define STATE_4 ""
#define STATE_5 ""
#define STATE_6 ""
#define STATE_7 ""
#define STATE_8 ""
#define STATE_9 ""
#define STATE_10 ""
#define STATE_11 ""
#define STATE_12 ""
#define STATE_13 ""
#define STATE_14 ""
#define STATE_15 ""
#define STATE_16 ""

#endif

#ifdef FIRSTFLOOR

#define MQTT_USER "firstfloor"
#define MQTT_PASSWORD "firstfloor"
#define HOST_NAME "firstfloor"
#define MQTT_CONTROL_TOPIC "salwatorska6/firstfloor/control"
#define MQTT_STATUS_TOPIC "salwatorska6/firstfloor/status"
#define MQTT_DEBUG_TOPIC "salwatorska6/firstfloor/debug"

#define RELAY_1 "bulb_1stairs_right_small"
#define RELAY_2 "bulb_1stairs_right_main"
#define RELAY_3 "bulb_1stairs_left_small"
#define RELAY_4 "bulb_1stairs_left_main"
#define RELAY_5 "OFF_24_1stairs"
#define RELAY_6 ""
#define RELAY_7 ""
#define RELAY_8 ""

#define INPUT_1 ""
#define INPUT_2 ""
#define INPUT_3 ""
#define INPUT_4 ""
#define INPUT_5 ""
#define INPUT_6 ""
#define INPUT_7 "motion_1stairs_right"
#define INPUT_8 "motion_1stairs_left"
#define INPUT_9 "switch_1stairs"
#define INPUT_10 ""
#define INPUT_11 ""

#define STATE_1 "heard_beat_1stairs"
#define STATE_2 "motion_1stairs_off"
#define STATE_3 "bright_light_1stairs"
#define STATE_4 ""
#define STATE_5 ""
#define STATE_6 ""
#define STATE_7 ""
#define STATE_8 ""
#define STATE_9 ""
#define STATE_10 ""
#define STATE_11 ""
#define STATE_12 ""
#define STATE_13 ""
#define STATE_14 ""
#define STATE_15 ""
#define STATE_16 ""

#endif

#ifdef SECONDFLOOR

#define MQTT_USER "secondfloor"
#define MQTT_PASSWORD "secondfloor"
#define HOST_NAME "secondfloor"
#define MQTT_CONTROL_TOPIC "salwatorska6/secondfloor/control"
#define MQTT_STATUS_TOPIC "salwatorska6/secondfloor/status"
#define MQTT_DEBUG_TOPIC "salwatorska6/secondfloor/debug"

#define RELAY_1 "bulb_2stairs_right_small"
#define RELAY_2 "bulb_2stairs_right_main"
#define RELAY_3 "bulb_2stairs_left_small"
#define RELAY_4 "bulb_2stairs_left_main"
#define RELAY_5 "OFF_24_2stairs"
#define RELAY_6 "bulb_3stairs_small"
#define RELAY_7 "bulb_3stairs_main"
#define RELAY_8 ""

#define INPUT_1 ""
#define INPUT_2 ""
#define INPUT_3 ""
#define INPUT_4 ""
#define INPUT_5 "switch_3stairs"
#define INPUT_6 ""
#define INPUT_7 "motion_2stairs_right"
#define INPUT_8 "motion_2stairs_left"
#define INPUT_9 "switch_2stairs"
#define INPUT_10 "motion_3stairs"
#define INPUT_11 ""

#define STATE_1 "heard_beat_2stairs"
#define STATE_2 "motion_2stairs_off"
#define STATE_3 "bright_light_2stairs"
#define STATE_4 "motion_3stairs_off"
#define STATE_5 "bright_light_3stairs"
#define STATE_6 ""
#define STATE_7 ""
#define STATE_8 ""
#define STATE_9 ""
#define STATE_10 ""
#define STATE_11 ""
#define STATE_12 ""
#define STATE_13 ""
#define STATE_14 ""
#define STATE_15 ""
#define STATE_16 ""

#endif

#ifdef FLAT_10

#define MQTT_USER "Mieszkanie10"
#define MQTT_PASSWORD "Jackson21"
#define HOST_NAME "Mieszkanie10"
#define MQTT_CONTROL_TOPIC "salwatorska6/Mieszkanie10/control"
#define MQTT_STATUS_TOPIC "salwatorska6/Mieszkanie10/status"
#define MQTT_DEBUG_TOPIC "salwatorska6/Mieszkanie10/debug"

#define RELAY_1 "bulb_10toilet_mirror"
#define RELAY_2 "bulb_10room_right"
#define RELAY_3 "bulb_10room_left"
#define RELAY_4 ""
#define RELAY_5 "bulb_10toilet_center"
#define RELAY_6 "bulb_10kitchen_center"
#define RELAY_7 "bulb_10kitchen_desk"
#define RELAY_8 "bulb_10corridor"

#define INPUT_1 "switch_10kitchen"
#define INPUT_2 "switch_10room"
#define INPUT_3 ""
#define INPUT_4 "switch_10toilet_outside"
#define INPUT_5 "switch_10toilet_mirror"
#define INPUT_6 ""
#define INPUT_7 "switch_10corridor"
#define INPUT_8 "motion_10corridor"
#define INPUT_9 "motion_10room"
#define INPUT_10 "motion_10kitchen"
#define INPUT_11 ""

#define STATE_1 "auto_on_10kitchen_lamp1"
#define STATE_2 "auto_on_10kitchen_lamp2"
#define STATE_3 "auto_on_10room_lamp1"
#define STATE_4 "auto_on_10room_lamp2"
#define STATE_5 "auto_on_10corridor"
#define STATE_6 "auto_off_10kitchen_lamp1"
#define STATE_7 "auto_off_10kitchen_lamp2"
#define STATE_8 "auto_off_10room_lamp1"
#define STATE_9 "auto_off_10room_lamp2"
#define STATE_10 "auto_off_10corridor"
#define STATE_11 ""
#define STATE_12 ""
#define STATE_13 ""
#define STATE_14 ""
#define STATE_15 ""
#define STATE_16 ""

#endif

#endif

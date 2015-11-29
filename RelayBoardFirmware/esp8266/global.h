#ifndef GLOBAL_H_
#define GLOBAL_H_

#define WIFI_SSID "Salwatorska6admin"
#define WIFI_PASSWORD "Administrator@wifi6"
#define MQTT_SERVER_HOST "mqtt.salwatorska.pl"
#define MQTT_SERVER_PORT 1883
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

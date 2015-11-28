/*
 * esp8266
 *
 * Created: 2015-01-24 22:57:24
 *  Author: Marcin Bajer (https://github.com/bajerwitharm)
 *
 * Main file of esp8266 serial to JSON translator
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "telegrams.h"
#include "serial.h"
#include "mqtt.h"

Mqtt mqtt;

void setup() {
  setup_serial();
  mqtt.setup_mqtt();
}

void loop() {
  serial_loop();
  mqtt.mqtt_loop();
}



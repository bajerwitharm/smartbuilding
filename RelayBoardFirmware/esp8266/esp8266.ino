/*
 *  
 */
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "telegrams.h"
#include "serial.h"
#include "mqtt.h"
#define DEBUG

Mqtt mqtt;

void setup() {
  setup_serial();
  mqtt.setup_mqtt();
}
 
void loop() {
  serial_loop();
  mqtt.mqtt_loop(); 
}



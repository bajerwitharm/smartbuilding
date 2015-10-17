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



void setup() {
  setup_serial();
  setup_mqtt();
}
 
void loop() {
  serial_loop();
  mqtt_loop(); 
}



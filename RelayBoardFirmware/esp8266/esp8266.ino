/*
 *  
 */
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "telegrams.h"
#include "serial.h"

const char* ssid     = "Salwatorska6admin";
const char* password = "Admin@wifi6";



void setup() {
  setup_serial();
  setup_mqtt();
}
 
void loop() {
  serial_loop();
  mqtt_loop(); 
}



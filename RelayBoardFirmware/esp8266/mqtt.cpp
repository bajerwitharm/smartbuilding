#include "Arduino.h"
#include "telegrams.h"
#include "mqtt.h"
#include "global.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
  }
}

void mqtt_callback(char* topic, unsigned char* payload, unsigned int length) {
  decode_telegram(payload); 
}

void Mqtt::setup_mqtt() {
  setup_wifi(); 
  client.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  client.setCallback(mqtt_callback);
}


void reconnect() {
  while (!client.connected()) {
    if (client.connect(HOST_NAME, MQTT_USER, MQTT_PASSWORD)) {
      client.subscribe(MQTT_CONTROL_TOPIC);
    } else {
       delay(5000);
    }
  }
}

void Mqtt::mqtt_loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void Mqtt::publish_status(uint8_t* message, size_t length) {
  client.publish(MQTT_STATUS_TOPIC, message, length, true);
}

void Mqtt::publish_debug(uint8_t* message, size_t length) {
  client.publish(MQTT_DEBUG_TOPIC, message, length);
}


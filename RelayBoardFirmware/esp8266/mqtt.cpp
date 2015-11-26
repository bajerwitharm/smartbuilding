#include "Arduino.h"
#include "telegrams.h"
#include "mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//const char* ssid     = "TP-LINK_ANIA";
const char* ssid     = "Salwatorska6admin";
const char* password = "Administrator@wifi6";
//const char* password = "KochamAnie1";
const char* mqtt_server = "mqtt.salwatorska.pl";
const char* mqtt_user = "secondfloor";
const char* mqtt_password = "secondfloor";


const char* host_name = "secondfloor";
const char* mqtt_control_topic = "salwatorska6/secondfloor/control";
const char* mqtt_status_topic = "salwatorska6/secondfloor/status";
const char* mqtt_debug_topic = "salwatorska6/secondfloor/debug";
//#define DEBUG

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    // We start by connecting to a WiFi network
#if defined(DEBUG)
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif
 // WiFi.hostname(host_name);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if defined(DEBUG)
    Serial.print(".");
#endif
  }
  
#if defined(DEBUG) 
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif

}

void mqtt_callback(char* topic, unsigned char* payload, unsigned int length) {
#if defined(DEBUG) 
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
#endif
  decode_telegram(payload); 
}

void Mqtt::setup_mqtt() {
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
#if defined(DEBUG) 
    Serial.print("Attempting MQTT connection...");
#endif
    // Attempt to connect
    if (client.connect(host_name,mqtt_user,mqtt_password)) {
#if defined(DEBUG) 
      Serial.println("Connected");
#endif
      // ... and resubscribe
      client.subscribe(mqtt_control_topic);
    } else {
#if defined(DEBUG) 
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
#endif
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
  client.publish(mqtt_status_topic, message, length, true);
}

void Mqtt::publish_debug(uint8_t* message, size_t length) {
  client.publish(mqtt_debug_topic, message, length);
}

//size_t Mqtt::write(uint8_t character) { 
// // buffer.concat(character);return 1;
//}
//
//size_t Mqtt::write(const char *str) { 
// Serial.print("test2");return 1;
//}
//
//size_t Mqtt::write(const uint8_t *buffer, size_t size) { /*blahblah is the name of your class*/
//Serial.print("asd");return size;
//}
//
//void Mqtt::print(const char[]) {
//Serial.print("test4");  
//}
//
//void Mqtt::println(const char[]) {
//Serial.print("test5");  
//}
//
//void Mqtt::print(const String &) {
//Serial.print("test6");  
//}
//size_t Mqtt::print(const char* message) {
//  Serial.print("test1");
// client.publish("broker.mqtt-dashboard", message);
//}

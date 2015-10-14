#include "Arduino.h"


const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_control_topic = "broker.mqtt-dashboard.com";
const char* mqtt_status_topic = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    // We start by connecting to a WiFi network
#if defined(DEBUG)
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
#if defined(DEBUG) 
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif

}

void setup_mqtt() {
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
    if (client.connect("ESP8266Client")) {
#if defined(DEBUG) 
      Serial.println("Connected");
#endif
      // Once connected, publish an announcement...
      client.publish(mqtt_status_topic, "hello world");
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

void mqtt_loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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

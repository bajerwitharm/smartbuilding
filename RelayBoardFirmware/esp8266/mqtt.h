#ifndef MQTT_H_
#define MQTT_H_

void setup_mqtt();
void mqtt_loop();
void publish_debug(uint8_t* message, uint8_t length);
#endif

#ifndef MQTT_H_
#define MQTT_H_

class Mqtt
{
  public:
    void setup_mqtt();
    void mqtt_loop();
    void publish_debug(const char* message, size_t length);
    void publish_status(uint8_t* message, size_t length);
};
#endif


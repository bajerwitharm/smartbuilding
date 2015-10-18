#ifndef MQTT_H_
#define MQTT_H_

class Mqtt
{
  public:
  void setup_mqtt();
  void mqtt_loop();
  void publish_debug(uint8_t* message, size_t length);
  void publish_status(uint8_t* message, size_t length);
//  virtual size_t write(uint8_t);
//  virtual size_t write(const char *str);
//  virtual size_t write(const uint8_t *buffer, size_t size);
//  void print(const char[]);
//  void println(const char[]);
//  void print(const String &);
};
#endif

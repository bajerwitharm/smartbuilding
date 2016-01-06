#include <EEPROM.h>
#include "Arduino.h"
#include "config_eeprom.h"

config_t config = {
  .outputs = {RELAY_4, RELAY_3, RELAY_2, RELAY_7, RELAY_5, RELAY_6, RELAY_8, RELAY_1},
  .states = { STATE_1, STATE_2, STATE_3, STATE_4, STATE_5, STATE_6, STATE_7, STATE_8, STATE_9, STATE_10, STATE_11, STATE_12, STATE_13, STATE_14, STATE_15, STATE_16},
  .inputs = {INPUT_1, INPUT_2, INPUT_3, INPUT_4, INPUT_5, INPUT_11, INPUT_6, INPUT_7, INPUT_8, INPUT_9, INPUT_10}  
};

void write_to_eeprom(){
  unsigned char* buffer = (unsigned char*)&config;
  EEPROM.begin(4095);
  for (uint16_t i = 0 ; i < sizeof(config_t) ; i++) {
    EEPROM.write(i, (uint8_t)buffer[i]);
  }
  delay(200);
  EEPROM.commit();
  EEPROM.end();
}

void read_from_eeprom(){
  EEPROM.begin(4095);
  EEPROM.get( 0, config );
  EEPROM.end();
}

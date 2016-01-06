#ifndef CONFIG_EEPROM_H_
#define CONFIG_EEPROM_H_
#include "global.h"

typedef struct {
  char outputs[OUTPUTS_NR][MAX_ITEM_LEN];
  char states[STATES_NR][MAX_ITEM_LEN];
  char inputs[INPUTS_NR][MAX_ITEM_LEN];
} config_t;

extern config_t config;

void read_from_eeprom();
void write_to_eeprom();

#endif

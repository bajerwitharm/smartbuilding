#include "Arduino.h"
#include <ArduinoJson.h>
#include "serial.h"
#include "mqtt.h"
#include "Stream.h"
#include "global.h"
#include "config_eeprom.h"

extern Mqtt mqtt;

typedef uint16_t inputs_t;
typedef uint8_t outputs_t;
typedef uint16_t states_t;

#pragma pack(1)

//{"header":{"start":126,"source":11,"destination":10,"fc":222,"size":28},"activator":{"input_on":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"input_off":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false}},"actuator":{"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_toggle":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false},"state_toggle":{"HeardBeat":false}},"timer":{"time_preload":0,"time_current":0},"info":{"inputs":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"outputs":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"states":{"HeardBeat":false}},"crc":0}
//{"header":{"start":126,"source":11,"destination":10,"fc":223,"size":28},"actuator":{"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_toggle":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false},"state_toggle":{"HeardBeat":false}},"crc":0}
//{"header":{"start":126,"source":11,"destination":10,"fc":234,"size":28},"info":{"inputs":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"outputs":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":true,"bulb_1corridor":true,"bulb_1toilet_mirror":true,"reset":true,"bulb_1kitchen_desk":true},"states":{"HeardBeat":true}},"crc":10}
//{"header":{"start":126,"source":11,"destination":10,"fc":234,"size":28},"info":{"inputs":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"outputs":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":true,"bulb_1corridor":true,"bulb_1toilet_mirror":true,"reset":true,"bulb_1kitchen_desk":true},"states":{"HeardBeat":true}},"crc":10}{"header":{"start":126,"source":11,"destination":10,"fc":218,"size":28},"trigger_id":0,"activator":{"input_on":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"input_off":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false}},"actuator":{"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_toggle":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false},"state_toggle":{"HeardBeat":false}},"timer":{"time_preload":0,"time_current":0},"crc":0}
//{"header":{"start":126,"source":11,"destination":10,"fc":219,"size":28},"trigger_id":0,"activator":{"input_on":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"input_off":{"switch_lamp_kitchen":false,"switch_room_lamp":false,"switch_corridor":false,"switch_toilet_mirror":false,"switch_toilet_main":false,"motion_kitchen":false,"motion_room":false,"motion_corridor":false},"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false}},"actuator":{"output_on":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_off":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"output_toggle":{"bulb_1room_right":false,"bulb_1room_left":false,"bulb_1kitchen_center":false,"bulb_1toilet_center":false,"bulb_1corridor":false,"bulb_1toilet_mirror":false,"reset":false,"bulb_1kitchen_desk":false},"state_on":{"HeardBeat":false},"state_off":{"HeardBeat":false},"state_toggle":{"HeardBeat":false}},"timer":{"time_preload":0,"time_current":10},"crc":10}
//echo -en '\x7e\x0b\x0a\xdf\x09\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\xab\x7b' > /dev/ttyS3
//echo -en '\x7e\x0b\x0a\xde\x1c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x01\x00\x00\x1d\x00\x00\xbb\x7b' > /dev/ttyS3
typedef enum __attribute__ ((__packed__)) {
  action_triggered_e = 0xDE,
  trigger_action_e = 0xDF,
  get_info_e = 0xEA,
  get_config_e = 0x12,
  set_config_e = 0x13,
  get_trigger_e = 0xDA,
  set_trigger_e = 0xDB,
} function_code_t;

typedef struct {
  uint8_t source;
  uint8_t destination;
  function_code_t fc;
  uint8_t telegram_size;
} telegram_header_t;

typedef struct {
  inputs_t input_on;
  inputs_t input_off;
  outputs_t output_on;
  outputs_t output_off;
  states_t state_on;
  states_t state_off;
} activator_t;

typedef struct {
  outputs_t output_on;
  outputs_t output_off;
  outputs_t output_toggle;
  uint8_t padding;
  states_t state_on;
  states_t state_off;
  states_t state_toggle;
} actuator_t;

typedef struct {
  uint16_t time_preload;
  uint16_t time_current;
} timers_t;

typedef struct {
  activator_t activator;
  actuator_t actuator;
  timers_t timer;
} trigger_t;

typedef struct {
  inputs_t inputs;
  outputs_t outputs;
  states_t states;
} info_t;

typedef uint8_t crc_t;

typedef struct {
  telegram_header_t header;
  trigger_t trigger;
  info_t info;
  uint8_t crc;
} action_triggered_t;

typedef struct {
  telegram_header_t header;
  trigger_t trigger;
  uint8_t trigger_id;
  uint8_t crc;
} set_trigger_t;

typedef struct {
  telegram_header_t header;
  actuator_t actuator;
  uint8_t crc;
} trigger_action_t;

typedef struct {
  telegram_header_t header;
  info_t info;
  uint8_t crc;
} get_info_t;

#pragma pack()

void encode_uint8s(JsonObject& json, uint8_t* out_telegram, const char* elements[], const uint8_t count ) {

  for (size_t i = 0; i<count; i++)
  {
    const char* element = elements[i];
    json[element] = out_telegram[i];
  }
}

void encode_uint16s(JsonObject& json, uint16_t* out_telegram, const char* elements[], const uint8_t count ) {

  for (size_t i = 0; i<count; i++)
  {
    const char* element = elements[i];
    json[element] = out_telegram[i];
  }
}

void encode_bools_only_true(JsonObject& json, uint8_t* out_telegram, const char elements[][MAX_ITEM_LEN], const uint8_t count ) {

  for (size_t i = 0; i<count; i++)
  {
    const char* element = elements[i];
    if (strcmp(element, "") != 0) {
      if (i < 8) {
        if ((out_telegram[0] & (1 << (i))) != 0) {
          json[element] = true;
        }
      }
      else {
        if ((out_telegram[1] & (1 << ((i - 8)))) != 0) {
          json[element] = true;
        }
      }
    }
  }
}

void encode_bools(JsonObject& json, uint8_t* out_telegram, const char elements[][MAX_ITEM_LEN], const uint8_t count ) {

  for (size_t i = 0; i<count; i++)
  {
    const char* element = elements[i];
    if (strcmp(element, "") != 0) {
      if (i < 8) {
        if ((out_telegram[0] & (1 << (i))) != 0) {
          json[element] = true;
        }
        else
        {
          json[element] = false;
        }
      }
      else {
        if ((out_telegram[1] & (1 << ((i - 8)))) != 0) {
          json[element] = true;
        }
        else
        {
          json[element] = false;
        }
      }
    }
  }
}


void decode_uint8s(JsonObject& json, uint8_t* out_telegram, const char* elements[], const uint8_t count ) {
  for (JsonObject::iterator it = json.begin(); it != json.end(); ++it)
  {
    for (size_t i = 0; i<count; i++)
    {
      const char* element = elements[i];
      if (strcmp(element, it->key) == 0) {
        out_telegram[i] = it->value.as<uint8_t>();
        break;
      }
    }
  }
}

void decode_uint16s(JsonObject& json, uint16_t* out_telegram, const char* elements[],const uint8_t count ) {
  for (JsonObject::iterator it = json.begin(); it != json.end(); ++it)
  {
    for (size_t i = 0; i<count; i++)
    {
      const char* element = elements[i];
      if (strcmp(it->key, element) == 0) {
        out_telegram[i] = it->value.as<uint16_t>();
      }
    }
  }
}


void decode_bools(JsonObject& json, uint8_t* out_telegram, const char elements[][MAX_ITEM_LEN],const uint8_t count  ) {
  out_telegram[0] = 0;
  for (size_t i = 0; i<count ; i++)
  {
    if (i == 8) {
      out_telegram[1] = 0;
    }
    for (JsonObject::iterator it = json.begin(); it != json.end(); ++it)
    {
      const char* element = elements[i];
      if (strcmp(it->key, element) == 0) {
        if (it->value.as<bool>()) {
          if (i < 8) {
            out_telegram[0] |= (1 << (i));
          }
          else {
            out_telegram[1] |= (1 << (i - 8));
          }
        }
      }
    }
  }
}

void decode_header(JsonObject& json, telegram_header_t* out_telegram) {
  const char* elements[] = {"source", "destination", "fc", "size"};
  decode_uint8s(json, (uint8_t*)&out_telegram[0], elements,4);
}

void encode_header(JsonObject& json, telegram_header_t* out_telegram) {
  const char* elements[] = {"source", "destination", "fc", "size"};
  encode_uint8s(json, (uint8_t*)&out_telegram[0], elements,4);
}

void decode_actuator(JsonObject& json, actuator_t* out_telegram) {
  decode_bools(json["output_on"], (uint8_t*)&out_telegram->output_on, config.outputs,OUTPUTS_NR);
  decode_bools(json["output_off"], (uint8_t*)&out_telegram->output_off, config.outputs,OUTPUTS_NR);
  decode_bools(json["output_toggle"], (uint8_t*)&out_telegram->output_toggle, config.outputs,OUTPUTS_NR);
  decode_bools(json["state_on"], (uint8_t*)&out_telegram->state_on, config.states,STATES_NR);
  decode_bools(json["state_off"], (uint8_t*)&out_telegram->state_off, config.states,STATES_NR);
  decode_bools(json["state_toggle"], (uint8_t*)&out_telegram->state_toggle, config.states,STATES_NR);
}

void encode_actuator(JsonObject& json, actuator_t* out_telegram) {
  encode_bools_only_true(json.createNestedObject("output_on"), (uint8_t*)&out_telegram->output_on, config.outputs,OUTPUTS_NR);
  encode_bools_only_true(json.createNestedObject("output_off"), (uint8_t*)&out_telegram->output_off, config.outputs,OUTPUTS_NR);
  encode_bools_only_true(json.createNestedObject("output_toggle"), (uint8_t*)&out_telegram->output_toggle, config.outputs,OUTPUTS_NR);
  encode_bools_only_true(json.createNestedObject("state_on"), (uint8_t*)&out_telegram->state_on, config.states,STATES_NR);
  encode_bools_only_true(json.createNestedObject("state_off"), (uint8_t*)&out_telegram->state_off, config.states,STATES_NR);
  encode_bools_only_true(json.createNestedObject("state_toggle"), (uint8_t*)&out_telegram->state_toggle, config.states,STATES_NR);
}

void decode_activator(JsonObject& json, activator_t* out_telegram) {
  decode_bools(json["input_on"], (uint8_t*)&out_telegram->input_on, config.inputs,INPUTS_NR);
  decode_bools(json["input_off"], (uint8_t*)&out_telegram->input_off, config.inputs,INPUTS_NR);
  decode_bools(json["output_on"], (uint8_t*)&out_telegram->output_on, config.outputs,OUTPUTS_NR);
  decode_bools(json["output_off"], (uint8_t*)&out_telegram->output_off, config.outputs,OUTPUTS_NR);
  decode_bools(json["state_on"], (uint8_t*)&out_telegram->state_on, config.states,STATES_NR);
  decode_bools(json["state_off"], (uint8_t*)&out_telegram->state_off, config.states,STATES_NR);
}

void encode_activator(JsonObject& json, activator_t* out_telegram) {
  encode_bools_only_true(json.createNestedObject("input_on"), (uint8_t*)&out_telegram->input_on, config.inputs,INPUTS_NR);
  encode_bools_only_true(json.createNestedObject("input_off"), (uint8_t*)&out_telegram->input_off, config.inputs,INPUTS_NR);
  encode_bools_only_true(json.createNestedObject("output_on"), (uint8_t*)&out_telegram->output_on, config.outputs,OUTPUTS_NR);
  encode_bools_only_true(json.createNestedObject("output_off"), (uint8_t*)&out_telegram->output_off, config.outputs,OUTPUTS_NR);
  encode_bools_only_true(json.createNestedObject("state_on"), (uint8_t*)&out_telegram->state_on, config.states,STATES_NR);
  encode_bools_only_true(json.createNestedObject("state_off"), (uint8_t*)&out_telegram->state_off, config.states,STATES_NR);
}

void decode_timer(JsonObject& json, timers_t* out_telegram) {
  const char* elements[] = {"time_preload", "time_current"};
  decode_uint16s(json, (uint16_t*)&out_telegram[0], elements,2);
}

void encode_timer(JsonObject& json, timers_t* out_telegram) {
  const char* elements[] = {"time_preload", "time_current"};
  encode_uint16s(json, (uint16_t*)&out_telegram[0], elements, 2);
}

void decode_info(JsonObject& json, info_t* out_telegram) {
  decode_bools(json["inputs"], (uint8_t*)&out_telegram->inputs, config.inputs,INPUTS_NR);
  decode_bools(json["outputs"], (uint8_t*)&out_telegram->outputs, config.outputs,OUTPUTS_NR);
  decode_bools(json["states"], (uint8_t*)&out_telegram->states, config.states,STATES_NR);
}

void encode_info(JsonObject& json, info_t* out_telegram) {
  encode_bools(json.createNestedObject("inputs"), (uint8_t*)&out_telegram->inputs, config.inputs,INPUTS_NR);
  encode_bools(json.createNestedObject("outputs"), (uint8_t*)&out_telegram->outputs, config.outputs,OUTPUTS_NR);
  encode_bools(json.createNestedObject("states"), (uint8_t*)&out_telegram->states, config.states,STATES_NR);
}

void decode_trigger(JsonObject& json, trigger_t* out_telegram) {
  decode_activator(json["activator"], &out_telegram->activator);
  decode_actuator(json["actuator"], &out_telegram->actuator);
  decode_timer(json["timer"], &out_telegram->timer);
}

void encode_trigger(JsonObject& json, trigger_t* out_telegram) {
  encode_activator(json.createNestedObject("activator"), &out_telegram->activator);
  encode_actuator(json.createNestedObject("actuator"), &out_telegram->actuator);
  encode_timer(json.createNestedObject("timer"), &out_telegram->timer);
}

void decode_info_response(JsonObject& json, get_info_t* out_telegram) {
  decode_header(json["header"], &out_telegram->header);
  decode_info(json["info"], &out_telegram->info);
  out_telegram->crc = json["crc"];
}

void encode_info_response(JsonObject& json, get_info_t* out_telegram) {
  encode_header(json.createNestedObject("header"), &out_telegram->header);
  encode_info(json.createNestedObject("info"), &out_telegram->info);
  json["crc"] = out_telegram->crc;
}

void encode_action_triggered(JsonObject& json, action_triggered_t* out_telegram) {
  encode_header(json.createNestedObject("header"), &out_telegram->header);
  encode_trigger(json, &out_telegram->trigger);
  encode_info(json.createNestedObject("info"), &out_telegram->info);
  json["crc"] = out_telegram->crc;
}

void decode_action_triggered(JsonObject& json, action_triggered_t* out_telegram) {
  decode_header(json["header"], &out_telegram->header);
  decode_trigger(json, &out_telegram->trigger);
  decode_info(json["info"], &out_telegram->info);
  uint8_t crc = json["crc"];
  out_telegram->crc = crc;
}

void decode_trigger_action(JsonObject& json, trigger_action_t* out_telegram) {
  decode_header(json["header"], &out_telegram->header);
  decode_actuator(json["actuator"], &out_telegram->actuator);
  uint8_t crc = json["crc"];
  out_telegram->crc = crc;
}


void encode_trigger_action(JsonObject& json, trigger_action_t* out_telegram) {
  encode_header(json.createNestedObject("header"), &out_telegram->header);
  encode_actuator(json.createNestedObject("actuator"), &out_telegram->actuator);
  json["crc"] = out_telegram->crc;
}

void decode_get_trigger(JsonObject& json, set_trigger_t* out_telegram) {
  decode_header(json["header"], &out_telegram->header);
  decode_trigger(json, &out_telegram->trigger);
  uint8_t trigger_id = json["trigger_id"];
  out_telegram->trigger_id = trigger_id;
  uint8_t crc = json["crc"];
  out_telegram->crc = crc;
}

void encode_get_trigger(JsonObject& json, set_trigger_t* out_telegram) {
  encode_header(json.createNestedObject("header"), &out_telegram->header);
  json["trigger_id"] = out_telegram->trigger_id;
  encode_trigger(json, &out_telegram->trigger);
  json["crc"] = out_telegram->crc;
}

void add_config_element(JsonObject& root,const char* element_name, char elements[][MAX_ITEM_LEN],const uint8_t count) {
  JsonArray&  array = root.createNestedArray(element_name);
  for (size_t i = 0; i<count; i++) {
     array.add(elements[i]);
  }
}


void decode_get_config() {  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.createNestedObject("header")["fc"] = get_config_e;
  add_config_element(root,"outputs",config.outputs,OUTPUTS_NR);
  add_config_element(root,"inputs",config.inputs,INPUTS_NR);
  add_config_element(root,"states",config.states,STATES_NR);
  size_t length = root.measureLength();
  char* p = (char*)malloc(length + 2);
  root.printTo(p, length + 2);
  mqtt.publish_status(((uint8_t*)p), length);
  free(p);  
}

void get_config_element(JsonObject& root,char* element_name, char* elements[],const uint8_t count) {
  JsonArray&  array = root.createNestedArray(element_name);
  for (size_t i = 0; i<count; i++) {
     array.add(elements[i]);
  }
}

void set_config_element(JsonArray& array, char elements[][MAX_ITEM_LEN],uint8_t len) {
  for (uint8_t i=0;i<len;i++) {
    sprintf(elements[i], array[i]);
  }  
}


void decode_set_config(JsonObject& json) {  
  set_config_element(json["outputs"], config.outputs,OUTPUTS_NR);
  set_config_element(json["inputs"], config.inputs,INPUTS_NR);
  set_config_element(json["states"], config.states,STATES_NR);
  write_to_eeprom();
}



void encode_telegram(unsigned char* buffer) {
  DynamicJsonBuffer jsonBuffer;
  //mqtt.publish_debug(rx_buffer.data, rx_buffer.size);
  JsonObject& root = jsonBuffer.createObject();
  telegram_header_t* header = (telegram_header_t*)buffer;
  switch (header->fc) {
    case get_info_e:
      encode_info_response(root, (get_info_t*)buffer);
      break;    
    case action_triggered_e:
      encode_action_triggered(root, (action_triggered_t*)buffer);
      break;
    case trigger_action_e:
      encode_trigger_action(root, (trigger_action_t*)buffer);
      break;
    case get_trigger_e:
    case set_trigger_e:
      encode_get_trigger(root, (set_trigger_t*)buffer);
      break;
    case get_config_e:
    case set_config_e:
      break;
  }
  size_t length = root.measureLength();
  char* p = (char*)malloc(length + 2);
  root.printTo(p, length + 2);
  mqtt.publish_status(((uint8_t*)p), length);
  free(p);
}


void decode_telegram(unsigned char* payload) {
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject((char*) payload);

  switch (root["header"]["fc"].as<uint8_t>()) {
    case get_config_e:
      decode_get_config();
      return;
      break;
    case set_config_e:
      decode_set_config(root);
      return;
      break;
    case get_info_e:
      decode_info_response(root, (get_info_t*)tx_buffer.data);
      tx_buffer.size = sizeof(get_info_t);
      break;
    case action_triggered_e:
      decode_action_triggered(root, (action_triggered_t*)tx_buffer.data);
      tx_buffer.size = sizeof(action_triggered_t);
      break;
    case trigger_action_e:
      decode_trigger_action(root, (trigger_action_t*)tx_buffer.data);
      tx_buffer.size = sizeof(trigger_action_t);
      break;
    case get_trigger_e:
    case set_trigger_e:
      decode_get_trigger(root, (set_trigger_t*)tx_buffer.data);
      tx_buffer.size = sizeof(set_trigger_t);
      break;
  }
  send_telegram();
}




#include "Arduino.h"
#include <ArduinoJson.h>
#include "serial.h"
#include "mqtt.h"
#include "Stream.h"
//#define DEBUG

extern Mqtt mqtt;

typedef uint16_t inputs_t;
typedef uint8_t outputs_t;
typedef uint16_t states_t;


//const char* outputs[20] = {"bulb_1room_right", "bulb_1room_left" , "bulb_1kitchen_center", "bulb_1toilet_center", "bulb_1corridor", "bulb_1toilet_mirror", "reset","bulb_1kitchen_desk", NULL};
const char* outputs[20] = {"bulb_2stairs_left_small", "bulb_2stairs_left_main", "bulb_2stairs_right_small", "bulb_2stairs_right_main", "OFF_24", "bulb_3stairs_small", "bulb_3stairs_main", NULL};
//const char* states[20] = { "HeardBeat", NULL };
const char* states[20] = { "heard_beat_2stairs", "disable_motion_2stairs", "bright_light_2stairs", "disable_motion_3stairs", "bright_light_3stairs", NULL };
//const char* inputs[20] = { "switch_lamp_kitchen", "switch_room_lamp", "switch_corridor", "switch_toilet_mirror", "switch_toilet_main", "motion_kitchen", "motion_room", "motion_corridor", NULL};
const char* inputs[20] = {"motion_2stairs_left","motion_2stairs_right","switch_2stairs", "motion_3stairs","switch_3stairs",NULL};
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

//Test telegram:7E FB FA DE 12 02 00 00 00 00 00 00 03 03 00 00 04 00 00 01 00 01 00 AB
//        7E FB FA DE 12 01 00 00 00 00 00 00 00 00 00 01 00 00 01 01 00 01 00 AB
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

//Test telegram: 0x7E 0xFA 0xFB 0xDF 0x06 0x00 0x00 0xFF 0x00 0x00 0x00 0xAB
typedef struct {
  telegram_header_t header;
  actuator_t actuator;
  uint8_t crc;
} trigger_action_t;

//Test telegram: 0x7E 0xFA 0xFB 0xEA 0x00 0xAB
typedef struct {
  telegram_header_t header;
  info_t info;
  uint8_t crc;
} get_info_t;

#pragma pack()

void encode_uint8s(JsonObject& json, uint8_t* out_telegram, const char* elements[] ) {

    for (size_t i = 0; elements[i]!=NULL; i++)
    {
        const char* element = elements[i];
        json[element] = out_telegram[i];
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.println(out_telegram[i]);
#endif          
    }
}

void encode_uint16s(JsonObject& json, uint16_t* out_telegram, const char* elements[] ) {

    for (size_t i = 0; elements[i]!=NULL; i++)
    {
        const char* element = elements[i];
        json[element] = out_telegram[i];
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.println(out_telegram[i]);
#endif          
    }
}

void encode_bools(JsonObject& json, uint8_t* out_telegram, const char* elements[] ) {

    for (size_t i = 0; elements[i]!=NULL; i++)
    {
        const char* element = elements[i];
        
          if (i < 8) {
            if (out_telegram[0]&(1 << i)) {
              json[element] = true;  
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.println("=true");
#endif  
            }
            else
            {
              json[element] = false; 
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.println("=false");
#endif   
            }            
          }
          else {
            if (out_telegram[1]&(1 << (i-8))) {
              json[element] = true;  
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.println("=true");
#endif  
            }
            else
            {
              json[element] = false;
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.println("=false");
#endif   
            } 
          } 
    }
}


void decode_uint8s(JsonObject& json, uint8_t* out_telegram, const char* elements[] ) {
  for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
  {
    for (size_t i = 0; elements[i]!=NULL; i++)
    {
        const char* element = elements[i];
        if (strcmp(element,it->key)==0) {
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.println(it->value.as<uint8_t>());
#endif          
          out_telegram[i] = it->value.as<uint8_t>();
          break;
        }
    }
  }
}

void decode_uint16s(JsonObject& json, uint16_t* out_telegram, const char* elements[] ) {
  for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
  {
    for (size_t i = 0; elements[i]!=NULL; i++)
    {
        const char* element = elements[i];
        if (strcmp(it->key,element)==0) {
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.print(it->value.as<uint16_t>());
#endif          
          out_telegram[i] = it->value.as<uint16_t>();
        }
    }
  }
}


void decode_bools(JsonObject& json, uint8_t* out_telegram, const char* elements[] ) {
  out_telegram[0] = 0;
  for (size_t i = 0; elements[i]!=NULL; i++)
  {
    if (i==8) {
      out_telegram[1] = 0;
    }
    for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
    {
        const char* element = elements[i];
        if (strcmp(it->key,element)==0) {
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.println(it->value.as<bool>());
#endif          
          if (it->value.as<bool>()) {
            if (i < 8) {
              out_telegram[0] |= (1 << i);
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
  const char* elements[] = {"source", "destination", "fc", "size", NULL};
  decode_uint8s(json, (uint8_t*)&out_telegram[0], elements);
}

void encode_header(JsonObject& json, telegram_header_t* out_telegram) {
  const char* elements[] = {"source", "destination", "fc", "size", NULL};
  encode_uint8s(json, (uint8_t*)&out_telegram[0], elements);
}

void decode_actuator(JsonObject& json, actuator_t* out_telegram) {
  decode_bools(json["output_on"],(uint8_t*)&out_telegram->output_on,outputs);
  decode_bools(json["output_off"],(uint8_t*)&out_telegram->output_off,outputs);
  decode_bools(json["output_toggle"],(uint8_t*)&out_telegram->output_toggle,outputs);
  decode_bools(json["state_on"],(uint8_t*)&out_telegram->state_on,states);
  decode_bools(json["state_off"],(uint8_t*)&out_telegram->state_off,states);
  decode_bools(json["state_toggle"],(uint8_t*)&out_telegram->state_toggle,states);  
}

void encode_actuator(JsonObject& json, actuator_t* out_telegram) {
  encode_bools(json.createNestedObject("output_on"),(uint8_t*)&out_telegram->output_on,outputs);
  encode_bools(json.createNestedObject("output_off"),(uint8_t*)&out_telegram->output_off,outputs);
  encode_bools(json.createNestedObject("output_toggle"),(uint8_t*)&out_telegram->output_toggle,outputs);
  encode_bools(json.createNestedObject("state_on"),(uint8_t*)&out_telegram->state_on,states);
  encode_bools(json.createNestedObject("state_off"),(uint8_t*)&out_telegram->state_off,states);
  encode_bools(json.createNestedObject("state_toggle"),(uint8_t*)&out_telegram->state_toggle,states);  
}

void decode_activator(JsonObject& json, activator_t* out_telegram) {
  decode_bools(json["input_on"],(uint8_t*)&out_telegram->input_on,inputs);
  decode_bools(json["input_off"],(uint8_t*)&out_telegram->input_off,inputs);
  decode_bools(json["output_on"],(uint8_t*)&out_telegram->output_on,outputs);
  decode_bools(json["output_off"],(uint8_t*)&out_telegram->output_off,outputs);
  decode_bools(json["state_on"],(uint8_t*)&out_telegram->state_on,states);
  decode_bools(json["state_off"],(uint8_t*)&out_telegram->state_off,states);  
}

void encode_activator(JsonObject& json, activator_t* out_telegram) {
  encode_bools(json.createNestedObject("input_on"),(uint8_t*)&out_telegram->input_on,inputs);
  encode_bools(json.createNestedObject("input_off"),(uint8_t*)&out_telegram->input_off,inputs);
  encode_bools(json.createNestedObject("output_on"),(uint8_t*)&out_telegram->output_on,outputs);
  encode_bools(json.createNestedObject("output_off"),(uint8_t*)&out_telegram->output_off,outputs);
  encode_bools(json.createNestedObject("state_on"),(uint8_t*)&out_telegram->state_on,states);
  encode_bools(json.createNestedObject("state_off"),(uint8_t*)&out_telegram->state_off,states);  
}

void decode_timer(JsonObject& json, timers_t* out_telegram) {
  const char* elements[] = {"time_preload","time_current",NULL};
  decode_uint16s(json,(uint16_t*)&out_telegram[0], elements);
}

void encode_timer(JsonObject& json, timers_t* out_telegram) {
  const char* elements[] = {"time_preload","time_current",NULL};
  encode_uint16s(json,(uint16_t*)&out_telegram[0], elements);
}

void decode_info(JsonObject& json, info_t* out_telegram) {
  decode_bools(json["inputs"],(uint8_t*)&out_telegram->inputs,inputs);
  decode_bools(json["outputs"],(uint8_t*)&out_telegram->outputs,outputs);
  decode_bools(json["states"],(uint8_t*)&out_telegram->states,states);
}

void encode_info(JsonObject& json, info_t* out_telegram) {
  encode_bools(json.createNestedObject("inputs"),(uint8_t*)&out_telegram->inputs,inputs);
  encode_bools(json.createNestedObject("outputs"),(uint8_t*)&out_telegram->outputs,outputs);
  encode_bools(json.createNestedObject("states"),(uint8_t*)&out_telegram->states,states);
}

void decode_trigger(JsonObject& json, trigger_t* out_telegram) {
  decode_activator(json["activator"],&out_telegram->activator);
  decode_actuator(json["actuator"],&out_telegram->actuator);
  decode_timer(json["timer"],&out_telegram->timer);
}

void encode_trigger(JsonObject& json, trigger_t* out_telegram) {
  encode_activator(json.createNestedObject("activator"),&out_telegram->activator);
  encode_actuator(json.createNestedObject("actuator"),&out_telegram->actuator);
  encode_timer(json.createNestedObject("timer"),&out_telegram->timer);
}

void decode_info_response(JsonObject& json, get_info_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_info(json["info"],&out_telegram->info);
  out_telegram->crc = json["crc"];
}

void encode_info_response(JsonObject& json, get_info_t* out_telegram) {
  encode_header(json.createNestedObject("header"),&out_telegram->header);
  encode_info(json.createNestedObject("info"),&out_telegram->info);
  json["crc"] = out_telegram->crc;
}

void encode_action_triggered(JsonObject& json, action_triggered_t* out_telegram) {
  encode_header(json.createNestedObject("header"),&out_telegram->header);
  encode_trigger(json,&out_telegram->trigger);
  encode_info(json.createNestedObject("info"),&out_telegram->info);
  json["crc"] = out_telegram->crc;
}

void decode_action_triggered(JsonObject& json, action_triggered_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_trigger(json,&out_telegram->trigger);
  decode_info(json["info"],&out_telegram->info);
  uint8_t crc = json["crc"];
  out_telegram->crc = crc;
}

void decode_trigger_action(JsonObject& json, trigger_action_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_actuator(json["actuator"],&out_telegram->actuator);
  uint8_t crc = json["crc"];
  out_telegram->crc = crc;
}


void encode_trigger_action(JsonObject& json, trigger_action_t* out_telegram) {
  encode_header(json.createNestedObject("header"),&out_telegram->header);
  encode_actuator(json.createNestedObject("actuator"),&out_telegram->actuator);
  json["crc"] = out_telegram->crc;
}

void decode_get_trigger(JsonObject& json, set_trigger_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_trigger(json,&out_telegram->trigger);
  uint8_t trigger_id = json["trigger_id"];
  out_telegram->trigger_id = trigger_id;
  uint8_t crc = json["crc"];
  out_telegram->crc = crc;
}

void encode_get_trigger(JsonObject& json, set_trigger_t* out_telegram) {
  encode_header(json.createNestedObject("header"),&out_telegram->header);
  json["trigger_id"] = out_telegram->trigger_id;
  encode_trigger(json,&out_telegram->trigger);
  json["crc"] = out_telegram->crc;
}


void encode_telegram(unsigned char* buffer) {
  DynamicJsonBuffer jsonBuffer;
  mqtt.publish_debug(rx_buffer.data,rx_buffer.size);
  JsonObject& root = jsonBuffer.createObject(); 
  telegram_header_t* header = (telegram_header_t*)buffer;
  switch (header->fc) {
        case get_info_e:
            encode_info_response(root,(get_info_t*)buffer);
            break;
        case action_triggered_e:
            encode_action_triggered(root,(action_triggered_t*)buffer);
            break;
        case trigger_action_e:
            encode_trigger_action(root,(trigger_action_t*)buffer);          
            break;
        case get_trigger_e:
        case set_trigger_e:
            encode_get_trigger(root,(set_trigger_t*)buffer);          
            break;        
  }
  size_t length = root.measureLength();
  char* p = (char*)malloc(length+2);
  root.printTo(p,length+2);
  mqtt.publish_status(((uint8_t*)p),length);
  free(p);
}


void decode_telegram(unsigned char* payload) {
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject((char*) payload);

#if defined(DEBUG) 
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  else
  {
    Serial.println("parseObject() success");    
  }
#endif  
  switch (root["header"]["fc"].as<uint8_t>()) {
        case get_info_e:
            decode_info_response(root,(get_info_t*)tx_buffer.data);
            tx_buffer.size = sizeof(get_info_t);
            break;
        case action_triggered_e:
            decode_action_triggered(root,(action_triggered_t*)tx_buffer.data);
            tx_buffer.size = sizeof(action_triggered_t);
            break;
        case trigger_action_e:
            decode_trigger_action(root,(trigger_action_t*)tx_buffer.data);
            tx_buffer.size = sizeof(trigger_action_t);
            break;
        case get_trigger_e:
        case set_trigger_e:
            decode_get_trigger(root,(set_trigger_t*)tx_buffer.data);
            tx_buffer.size = sizeof(set_trigger_t);
            break;
  }
  send_telegram();
 // mqtt.publish_debug(tx_buffer.data,tx_buffer.size);
   //     delay(5000);
 // encode_telegram(tx_buffer.data);
}




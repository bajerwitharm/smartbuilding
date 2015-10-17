#include "Arduino.h"
#include <ArduinoJson.h>
#include "serial.h"
#include "mqtt.h"
#define DEBUG


typedef uint16_t inputs_t;
typedef uint8_t outputs_t;
typedef uint16_t states_t;


const char* outputs[20] = {"bulb_1room_right", "bulb_1room_left" , "bulb_1kitchen_center", "bulb_1toilet_center", "bulb_1corridor", "bulb_1toilet_mirror", "","bulb_1kitchen_desk", NULL};
const char* states[20] = { "HeardBeat", NULL };
const char* inputs[20] = { "switch_lamp_kitchen", "switch_room_lamp", "switch_corridor", "switch_toilet_mirror", "switch_toilet_main", "motion_kitchen", "motion_room", "motion_corridor", NULL};

#pragma pack(1)


typedef enum __attribute__ ((__packed__)) {
  action_triggered_e = 0xDE,
  trigger_action_e = 0xDF,
  get_info_e = 0xEA,
  get_triggers_e = 0xDA,  
  set_trigger_e = 0xDB,   
} function_code_t;

typedef struct {
  uint8_t start;
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
} action_triggered_t;

typedef struct {
  telegram_header_t header;
  uint8_t trigger_id;
  trigger_t trigger;
} set_trigger_t;

//Test telegram: 0x7E 0xFA 0xFB 0xDF 0x06 0x00 0x00 0xFF 0x00 0x00 0x00 0xAB
typedef struct {
  telegram_header_t header;
  actuator_t actuator;
} trigger_action_t;

//Test telegram: 0x7E 0xFA 0xFB 0xEA 0x00 0xAB
typedef struct {
  telegram_header_t header;
  info_t info;
} get_info_t;


void encode_uint8s(JsonObject& json, uint8_t* out_telegram, const char* elements[20] ) {

    for (size_t i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
    {
        const char* element = elements[i];
        json[element] = out_telegram[i];
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.print(out_telegram[i]);
#endif          
    }
}

void encode_uint16s(JsonObject& json, uint16_t* out_telegram, const char* elements[20] ) {

    for (size_t i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
    {
        const char* element = elements[i];
        json[element] = out_telegram[i];
#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.print(out_telegram[i]);
#endif          
    }
}

void encode_bools(JsonObject& json, uint8_t* out_telegram, const char* elements[20] ) {

    for (size_t i = 0; i < sizeof(elements) / sizeof(elements[0]); i++)
    {
        const char* element = elements[i];

#if defined(DEBUG) 
          Serial.print("Setting:");
          Serial.print(element);
          Serial.print("=");
          Serial.println(out_telegram[i]);
#endif          
          if (i < 8) {
            if (out_telegram[0]&(1 << i)) {
              json[element] = true;  
            }
            else
            {
              json[element] = false;  
            }            
          }
          else {
            if (out_telegram[1]&(1 << (i-8))) {
              json[element] = true;  
            }
            else
            {
              json[element] = false;  
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
              Serial.println(i);
            }
            else {
              out_telegram[1] |= (1 << (i - 8));
              Serial.println(i);
            } 
          } 
        }
    }
  }
  Serial.println(out_telegram[0]);
}

void decode_header(JsonObject& json, telegram_header_t* out_telegram) {
  const char* elements[] = {"start", "source", "destination", "fc", "size", NULL};
  decode_uint8s(json, (uint8_t*)&out_telegram[0], elements);
}

void encode_header(JsonObject& json, telegram_header_t* out_telegram) {
  const char* elements[20] = {"start", "source", "destination", "fc", "size", NULL};
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

void decode_activator(JsonObject& json, activator_t* out_telegram) {
  decode_bools(json["input_on"],(uint8_t*)&out_telegram->input_on,inputs);
  decode_bools(json["input_off"],(uint8_t*)&out_telegram->input_off,inputs);
  decode_bools(json["output_on"],(uint8_t*)&out_telegram->output_on,outputs);
  decode_bools(json["output_off"],(uint8_t*)&out_telegram->output_off,outputs);
  decode_bools(json["state_on"],(uint8_t*)&out_telegram->state_on,states);
  decode_bools(json["state_off"],(uint8_t*)&out_telegram->state_off,states);  
}

void decode_timer(JsonObject& json, timers_t* out_telegram) {
  const char* elements[20] = {"time_preload","time_current"};
  decode_uint16s(json,(uint16_t*)&out_telegram[0], elements);
}

void decode_info(JsonObject& json, info_t* out_telegram) {
  decode_bools(json["inputs"],(uint8_t*)&out_telegram->inputs,inputs);
  decode_bools(json["outputs"],(uint8_t*)&out_telegram->outputs,outputs);
  decode_bools(json["states"],(uint8_t*)&out_telegram->states,states);
}

void encode_info(JsonObject& json, info_t* out_telegram) {
  decode_bools(json.createNestedObject("inputs"),(uint8_t*)&out_telegram->inputs,inputs);
  decode_bools(json.createNestedObject("outputs"),(uint8_t*)&out_telegram->outputs,outputs);
  decode_bools(json.createNestedObject("states"),(uint8_t*)&out_telegram->states,states);
}

void decode_trigger(JsonObject& json, trigger_t* out_telegram) {
  decode_activator(json["activator"],&out_telegram->activator);
  decode_actuator(json["actuator"],&out_telegram->actuator);
  decode_timer(json["timer"],&out_telegram->timer);
}

void decode_info_response(JsonObject& json, get_info_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_info(json["info"],&out_telegram->info);
}

void encode_info_response(JsonObject& json, get_info_t* out_telegram) {
  encode_header(json.createNestedObject("header"),&out_telegram->header);
  encode_info(json.createNestedObject("info"),&out_telegram->info);
}

void decode_action_triggered(JsonObject& json, action_triggered_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_trigger(json,&out_telegram->trigger);
  decode_info(json["info"],&out_telegram->info);
}

void decode_trigger_action(JsonObject& json, trigger_action_t* out_telegram) {
  decode_header(json["header"],&out_telegram->header);
  decode_actuator(json["actuator"],&out_telegram->actuator);
 // uint8_t crc = json["crc"]; 
 // out_telegram->crc = crc;
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
        #if defined(DEBUG) 
            Serial.println("start decoding get_info_e");
        #endif 
            decode_info_response(root,(get_info_t*)tx_buffer.data);
            tx_buffer.size = sizeof(get_info_t);
            Serial.write(tx_buffer.data,tx_buffer.size);
            break;
        case action_triggered_e:
        #if defined(DEBUG) 
            Serial.println("start decoding action_triggered_e");
        #endif
            decode_action_triggered(root,(action_triggered_t*)tx_buffer.data);
            tx_buffer.size = sizeof(action_triggered_t);
            Serial.write(tx_buffer.data,tx_buffer.size);
            publish_debug(tx_buffer.data,tx_buffer.size);
            break;
        case trigger_action_e:
        #if defined(DEBUG) 
            Serial.write("start decoding trigger_action_e");
        #endif
            decode_trigger_action(root,(trigger_action_t*)tx_buffer.data);
            tx_buffer.size = sizeof(trigger_action_t);
            Serial.write(tx_buffer.data,tx_buffer.size);
            break;
  }
}

void encode_telegram(unsigned char* buffer) {
  StaticJsonBuffer<3000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); 
  telegram_header_t* header = (telegram_header_t*)buffer;
  switch (header->fc) {
        case get_info_e:
            encode_info_response(root,(get_info_t*)tx_buffer.data);
            break;
        case action_triggered_e:

            break;
        case trigger_action_e:

            break;
  }

}

#pragma pack()


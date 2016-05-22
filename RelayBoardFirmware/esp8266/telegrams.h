#ifndef TELEGRAMS_H_
#define TELEGRAMS_H_

extern char* outputs[20];
extern char* states[20];
extern char* inputs[20];

#pragma pack(1)

typedef uint8_t crc_t;

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
  error_e = 0xAB,
} function_code_t;

typedef struct __attribute__ ((__packed__)) {
  uint8_t source;
  uint8_t destination;
  function_code_t fc;
  uint8_t telegram_size;
} telegram_header_t;

typedef enum __attribute__ ((__packed__)) {
  telegram_too_short_e = 0x01,
  telegram_too_long_e = 0x02,
  telegram_crc_error_e = 0x03,
} error_code_t;

typedef struct __attribute__ ((__packed__)) {
  telegram_header_t header;
  error_code_t error;
  crc_t crc;
} error_telegram_t;

#pragma pack()

void decode_telegram(unsigned char* payload);
void encode_telegram(unsigned char* buffer);
void encode_error_telegram(error_code_t error);

#endif


#include "Arduino.h"
#include "telegrams.h"
#include "serial.h"
#include "mqtt.h"
extern Mqtt mqtt;
#define BAUDRATE 4808

#define FRAME_START_CHAR 0x7E
#define FRAME_END_CHAR 0x7B
#define FRAME_OVERHEAD 5 // src_addr + dst_addr + size + fc + crc = 5
#define FRAME_LENGTH_FIELD_INDEX 3 //src_addr=1;dst_addr=2;len=3

static bool telegramInBuffer=false;

const static struct
{
  uint8_t toEscape;
  uint8_t howEscape[2];
  } escape_characters[] = {
  {
    .toEscape = 0x7E, //0x7E telegram start
    .howEscape = {0x7D, 0x5E}
  },
  {
    .toEscape = 0x7D,
    .howEscape = {0x7D, 0x5D}
  },  
  {
    .toEscape = 0x7B, //0x7B telegram end
    .howEscape = {0x7A, 0x5B}
  },
  {
    .toEscape = 0x7A,
    .howEscape = {0x7A, 0x5A}
  }
};


static const uint8_t number_char_to_excape =  sizeof(escape_characters) / sizeof(escape_characters[0] );

tx_buffer_t tx_buffer;
rx_buffer_t rx_buffer;

void setup_serial() {
  delay(10);
  Serial.begin(BAUDRATE); 
}

void send_telegram() {
  tx_buffer.index = 0;
  Serial.write(FRAME_START_CHAR);
  // escape characters
  while (tx_buffer.index<tx_buffer.size) {
    for (uint8_t j=0;j<number_char_to_excape;j++) {
      if (tx_buffer.data[tx_buffer.index] == escape_characters[j].toEscape)
      {
        Serial.write(escape_characters[j].howEscape[0]);
        tx_buffer.data[tx_buffer.index] = escape_characters[j].howEscape[1];
        return;
      }
    }
    mqtt.publish_debug(&tx_buffer.data[tx_buffer.index],1);
    Serial.write(tx_buffer.data[tx_buffer.index++]);
  }
  Serial.write(FRAME_END_CHAR);
}

void serialEvent() {
  while (Serial.available()) {
    uint8_t received = Serial.read(); // Fetch the received byte value into the variable "received"
    mqtt.publish_debug(&received,1);
    if (received == FRAME_START_CHAR) {
      rx_buffer.index = 0;
      rx_buffer.size = 0xFF;
    }
    else
    {
      // escape characters
      for (uint8_t i=0;i<number_char_to_excape;i++) {
        if ((received == escape_characters[i].howEscape[1]) && (rx_buffer.data[rx_buffer.index-1]==escape_characters[i].howEscape[0]))
        {
          received = escape_characters[i].toEscape;
          rx_buffer.index--;
        }
      }
      // save length of telegram
      if (rx_buffer.index == FRAME_LENGTH_FIELD_INDEX) {
       rx_buffer.size = received + FRAME_OVERHEAD;
      }
      rx_buffer.data[rx_buffer.index++] = received;
      if (rx_buffer.index == rx_buffer.size)
      {
        telegramInBuffer=true;
      }
    }
  }
}


void serial_loop() {
  serialEvent();
  if (telegramInBuffer) {
    encode_telegram(rx_buffer.data);
    telegramInBuffer=false;
  }
}


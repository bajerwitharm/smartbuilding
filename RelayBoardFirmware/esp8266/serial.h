#ifndef SERIAL_H_
#define SERIAL_H_

#define TX_BUFFER_SIZE 50
#define RX_BUFFER_SIZE 50

typedef struct {
  uint8_t index;
  uint8_t size;
  uint8_t data[TX_BUFFER_SIZE];
} tx_buffer_t;

extern tx_buffer_t tx_buffer;

typedef struct {
  uint8_t index;
  uint8_t size;
  uint8_t data[RX_BUFFER_SIZE];
} rx_buffer_t;

extern rx_buffer_t rx_buffer;

void setup_serial();
void serial_loop();
void send_telegram();



#endif

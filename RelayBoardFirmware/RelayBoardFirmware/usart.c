/*
 * usart.c
 *
 * Created: 2015-01-17 00:24:04
 *  Author: Marcin Bajer
 */ 
#include "global.h"
#include <string.h>
#include "timer.h"
#include "telegrams.h"

#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*8l)-1)
#define BAUDRATE 9600
#define TX_BUFFER_SIZE 20
#define RX_BUFFER_SIZE 20
#define FRAME_START_CHAR 0x7E

#define FRAME_OVERHEAD 3 // src_addr + dst_addr + crc = 3
#define FRAME_LENGTH_FIELD_INDEX 3 //src_addr=1;dst_addr=2;len=3 
const static struct 
{
	uint8_t toEscape;
	uint8_t howEscape[2];	
} escape_characters[] = {
	{
		.toEscape = 0x7E,
		.howEscape = {0x7D, 0x5E}
	},
	{
		.toEscape = 0x7D,
		.howEscape = {0x7D, 0x5D}
	}
};

static const uint8_t number_char_to_excape =  sizeof(escape_characters) / sizeof(escape_characters[0] );

static struct {
	uint8_t index;
	uint8_t size;
	uint8_t data[TX_BUFFER_SIZE];
} tx_buffer;

static struct {
	uint8_t index;
	uint8_t size;
	uint8_t data[RX_BUFFER_SIZE];
} rx_buffer;

/**
 * \brief Usart initialization 
 */
void usartInit(void)
{
	uint16_t baudrate = UART_BAUD_SELECT(BAUDRATE,QUARTZ);
	UBRRH = (uint8_t) (baudrate>>8);
	UBRRL = (uint8_t) baudrate;
	
	// double the transmission speed
	UCSRA = (1<<U2X);
	// enable interrupt on the RXC Flag, enable Receiver and Transmitter
	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE) | (1<<TXCIE);
	// set frame format: 8 data, 1 stop bit
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}


void handleTelegram(void)
{
}

ISR(USART_RXC_vect)
{
	uint8_t received = UDR; // Fetch the received byte value into the variable "received"
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
			handleTelegram();
		}
	}
}

void addCrc()
{
	tx_buffer.data[tx_buffer.size++] = 0xAB;//TODO: add crc implementation here
}

	
void initSending()
{
	while(!(UCSRA & (1<<UDRE)));
	addCrc();
	tx_buffer.index = 0;
		
	UDR = FRAME_START_CHAR;  
	 
	/* Enable UDR Empty interrupt */
	UCSRB |= (1<<UDRIE);
}

ISR(USART_UDRE_vect)
{
	// escape characters
	for (uint8_t j=0;j<number_char_to_excape;j++) {
		if (tx_buffer.data[tx_buffer.index] == escape_characters[j].toEscape)
		{
			UDR = escape_characters[j].howEscape[0];
			tx_buffer.data[tx_buffer.index] = escape_characters[j].howEscape[1]; 
			return;
		}
	}
	UDR = tx_buffer.data[tx_buffer.index++]; 
	if (tx_buffer.size == tx_buffer.index) {
		UCSRB &= ~(1<<UDRIE);                           // Stop UDR empty interrupt : TX End
	}
}

void usartSendAction(trigger_t* trigger)
{
	action_triggered_t* telegram = (action_triggered_t*)tx_buffer.data;
	tx_buffer.size = sizeof(action_triggered_t);
	telegram->fc = action_triggered_e;
	memcpy(&telegram->trigger,trigger,sizeof(trigger_t));
	initSending();
}

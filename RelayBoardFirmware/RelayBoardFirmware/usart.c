/*
* usart.c
*
* Created: 2015-01-17 00:24:04
*  Author: Marcin Bajer (https://github.com/bajerwitharm)
*
* Provides functions for handling communication via usart. Every telegram starts with \ref FRAME_START_CHAR and ends \ref FRAME_END_CHAR. Every frame contains source address, destination address, length of data field and 1 byte CRC
*/
#include "global.h"
#include <string.h>
#include <util/delay.h>
#include "timer.h"
#include "telegrams.h"
#include "usart.h"
#include "io_pins.h"

#define UART_BAUD_SELECT(baudRate,xtalCpu) ((( (xtalCpu) / ( (baudRate) * 8UL )))-1)
#define BAUDRATE (9600)
#define TX_BUFFER_SIZE 40
#define RX_BUFFER_SIZE 40
#define FRAME_START_CHAR 0x7E
#define FRAME_END_CHAR 0x7B

#define FRAME_OVERHEAD 5 // src_addr + dst_addr + size + fc + crc = 5
#define FRAME_LENGTH_FIELD_INDEX 3 //src_addr=1;dst_addr=2;len=3
static bool telegramInBuffer=false;


bool usartIsTelegramInBuffer(){
	return telegramInBuffer;
};

void usartClearBuffer(){
	telegramInBuffer=false;
};

const static struct
{
	uint8_t toEscape;
	uint8_t howEscape[2];
	} escape_characters[] = {
	{
		.toEscape = FRAME_START_CHAR,	//0x7E telegram start
		.howEscape = {0x7D, 0x5E}
	},
	{
		.toEscape = 0x7D,
		.howEscape = {0x7D, 0x5D}
	},
	{
		.toEscape = FRAME_END_CHAR,	//0x7B telegram end
		.howEscape = {0x7A, 0x5B}
	},
	{
		.toEscape = 0x7A,
		.howEscape = {0x7A, 0x5A}
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
	const uint16_t ubrr = UART_BAUD_SELECT(BAUDRATE,F_CPU);
	UBRRH = (uint8_t) (ubrr>>8);
	UBRRL = (uint8_t) ubrr;
	
	// double the transmission speed
	UCSRA = (1<<U2X);
	// enable interrupt on the RXC Flag, enable Receiver and Transmitter
	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);
	// set frame format: 8 data, 1 stop bit, parity none
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void waitUntilSendingOver()
{
	uint8_t i = 100;
	while(i--){
		if (((UCSRB & (1<<UDRIE)) == 0)&&((UCSRA & (1<<UDRE)))) {
			break;
		}
		_delay_ms(2);
	}
}

/*
*	Base on Optimized Dallas (now Maxim) iButton 8-bit CRC calculation.
*/
uint8_t calcCRC(const uint8_t *data, uint8_t len) {
	uint8_t crc = 0x00;
	while (len--) {
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--) {
			uint8_t sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum) {
				crc ^= 0x8C;
			}
			extract >>= 1;
		}
	}
	return crc;
}

void addCrc()
{
	const uint8_t crc = calcCRC(tx_buffer.data, tx_buffer.size);
	tx_buffer.data[tx_buffer.size++] = crc;
}

bool crcIsCorrect()
{
	const uint8_t crc = calcCRC(rx_buffer.data,rx_buffer.size-1);
	//return (crc == rx_buffer.data[rx_buffer.size-1]);
	return true;
}

void initSending()
{
	addCrc();
	tx_buffer.index = 0;
	UDR = FRAME_START_CHAR;
	/* Enable UDR Empty interrupt */
	UCSRB |= (1<<UDRIE);
}

void usartSendInfo()
{
	waitUntilSendingOver();
	get_info_t* const info = (get_info_t*) tx_buffer.data;
	info->header.destination = BUS_MASTER_ADDRESS;
	info->header.source = THIS_DEVICE_ADDRESS;
	info->header.fc = get_info_e;
	info->header.size = sizeof(info_t);
	#ifndef RELAY_16_BOARD
	info->info.inputs = ioGetInputs();
	#endif
	info->info.outputs = ioGetOutputs();
	#ifndef NO_TRIGGERS
	info->info.states = getState();
	#endif
	tx_buffer.size = sizeof(get_info_t);
	initSending();
}

#ifndef NO_TRIGGERS
void usartGetTriggers()
{
	waitUntilSendingOver();
	set_trigger_t* const triggerRequest = (set_trigger_t*) tx_buffer.data;
	triggerRequest->header.destination = BUS_MASTER_ADDRESS;
	triggerRequest->header.source = THIS_DEVICE_ADDRESS;
	triggerRequest->header.fc = get_triggers_e;
	triggerRequest->header.size = sizeof(trigger_t)+1;
	for (uint8_t i=0;i<getNumberOfTriggers();i++){
		waitUntilSendingOver();
		_delay_ms(400);
		triggerRequest->trigger_id=i;
		memcpy(&triggerRequest->trigger,&triggers[i],sizeof(trigger_t));
		tx_buffer.size = sizeof(set_trigger_t);
		_delay_ms(20);
		initSending();
	}
}

void usartSetTrigger()
{
	set_trigger_t* const setTriggerRequest = (set_trigger_t*) rx_buffer.data;
	memcpy(&triggers[setTriggerRequest->trigger_id],&setTriggerRequest->trigger,sizeof(trigger_t));
	set_trigger_t* const setTriggerResponse = (set_trigger_t*) tx_buffer.data;
	waitUntilSendingOver();
	setTriggerResponse->header.destination = BUS_MASTER_ADDRESS;
	setTriggerResponse->header.source = THIS_DEVICE_ADDRESS;
	setTriggerResponse->header.fc = set_trigger_e;
	setTriggerResponse->trigger_id = setTriggerRequest->trigger_id;
	setTriggerResponse->header.size = sizeof(trigger_t)+1;
	tx_buffer.size = sizeof(set_trigger_t);
	memcpy(&setTriggerResponse->trigger,&setTriggerRequest->trigger,sizeof(trigger_t));
	initSending();
}
#endif

void usartSendError(error_code_t error) {
	error_telegram_t* const telegram = (error_telegram_t*) tx_buffer.data;
	waitUntilSendingOver();
	telegram->header.destination = BUS_MASTER_ADDRESS;
	telegram->header.source = THIS_DEVICE_ADDRESS;
	telegram->header.fc = error_e;
	telegram->error = error;
	telegram->header.size=sizeof(error_code_t);
	tx_buffer.size = sizeof(error_telegram_t);
	initSending();
}

void usartActivateTriggerResponse()
{
	waitUntilSendingOver();
	memcpy(tx_buffer.data,rx_buffer.data,tx_buffer.index);
	trigger_action_t* triggerRequest = (trigger_action_t*) tx_buffer.data;
	//prepere response on telegram
	triggerRequest->header.destination = BUS_MASTER_ADDRESS;
	triggerRequest->header.source = THIS_DEVICE_ADDRESS;
	tx_buffer.size = sizeof(trigger_action_t);
	initSending();
}

void usartHandleTelegram(void)
{
	telegram_header_t* header = (telegram_header_t*) rx_buffer.data;
	if (header->destination == THIS_DEVICE_ADDRESS)
	{
		if (crcIsCorrect()) {
			switch(header->fc) {
				case trigger_action_e:
				if (header->size == sizeof(actuator_t))
				{
					activateTrigger(&((trigger_action_t*)rx_buffer.data)->actuator);
					usartSendInfo();
					} else {
					usartSendError(sizeof(actuator_t));
				}
				break;
				case get_info_e:
				if (header->size == 0) {
					usartSendInfo();
					} else {
					usartSendError(telegram_wrong_size_field);
				}
				break;
				#ifndef NO_TRIGGERS
				case get_triggers_e:
				if (header->size == 0) {
					usartGetTriggers();
					} else {
					usartSendError(telegram_wrong_size_field);
				}
				break;
				case set_trigger_e:
				usartSetTrigger();
				break;
				#endif
				default:
				usartSendError(telegram_wrong_fc_e);
			}
		}
		else
		{
			//usartSendError(telegram_crc_error_e);
			usartSendError(calcCRC(rx_buffer.data,rx_buffer.size-1));
		}
	}
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
			if (rx_buffer.size > RX_BUFFER_SIZE) {
				usartSendError(telegram_too_long_e);
				rx_buffer.size = RX_BUFFER_SIZE;
			}
		}
		if ((received == FRAME_END_CHAR)&&(rx_buffer.index < rx_buffer.size)) {
			usartSendError(telegram_too_short_e);
			telegramInBuffer=false;
			rx_buffer.index = 0;
		}
		rx_buffer.data[rx_buffer.index++] = received;
		if (rx_buffer.index == rx_buffer.size)
		{
			if (received == FRAME_END_CHAR) {
				usartSendError(telegram_too_long_e);
			} else {
				telegramInBuffer=true;
			}
		}

	}
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
	if (tx_buffer.size == tx_buffer.index) {
		UDR = FRAME_END_CHAR;
		UCSRB &= ~(1<<UDRIE);                           // Stop UDR empty interrupt : TX End
		} else {
		UDR = tx_buffer.data[tx_buffer.index++];
	}
}

void usartSendAction(trigger_t* const trigger, const uint8_t destination)
{
	waitUntilSendingOver();
	action_triggered_t* telegram = (action_triggered_t*)tx_buffer.data;
	tx_buffer.size = sizeof(action_triggered_t);
	memcpy(&telegram->trigger,trigger,sizeof(*trigger));
	telegram->header.fc = action_triggered_e;
	telegram->header.source = THIS_DEVICE_ADDRESS;
	telegram->header.destination = destination;
	telegram->header.size = sizeof(*trigger)+sizeof(info_t);
	#ifndef RELAY_16_BOARD
	telegram->info.inputs = ioGetInputs();
	#endif
	telegram->info.outputs = ioGetOutputs();
	#ifndef NO_TRIGGERS
	telegram->info.states = getState();
	#endif
	initSending();
}


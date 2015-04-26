#include "timer.h"
#include "global.h"
/*
 * usart.h
 *
 * Created: 2015-01-17 19:36:44
 *  Author: Marcin Bajer (https://github.com/bajerwitharm)
 */ 


#ifndef USART_H_
#define USART_H_

#include "triggers.h"

#define BUS_MASTER_ADDRESS 0x0A //address of router on the bus
#define THIS_DEVICE_ADDRESS 0x0B//adress of this device on the bus

void usartInit(void);
void usartSendAction(trigger_t* trigger, uint8_t destination);
bool usartIsTelegramInBuffer();
void usartClearBuffer();
void usartHandleTelegram(void);


#endif /* USART_H_ */
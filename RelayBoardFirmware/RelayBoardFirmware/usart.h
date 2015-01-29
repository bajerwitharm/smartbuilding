#include "timer.h"
/*
 * usart.h
 *
 * Created: 2015-01-17 19:36:44
 *  Author: Marcin Bajer
 */ 


#ifndef USART_H_
#define USART_H_

#include "triggers.h"

void usartInit(void);
void usartSendAction(trigger_t* trigger);


#endif /* USART_H_ */
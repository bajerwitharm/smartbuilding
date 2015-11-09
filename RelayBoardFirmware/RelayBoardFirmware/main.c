/*
* main.c
*
* Created: 2015-01-16 19:53:52
*  Author: Marcin Bajer
*/

#include "io_pins.h"
#include "usart.h"
#include "timer.h"
#include "triggers.h"
#include <avr/interrupt.h>
#include "usart.h"



int main(void)
{
	ioInit();
	usartInit();
	timerInit();
	sei();
	while(1)
	{		
		if (usartIsTelegramInBuffer()) {
			usartClearBuffer();
			usartHandleTelegram();
		}
	}
}
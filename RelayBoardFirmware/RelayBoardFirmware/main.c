/*
* main.c
*
* Created: 2015-01-16 19:53:52
*  Author: Marcin Bajer
*/

#include "io_pins.h"
#include "usart.h"

int main(void)
{
	ioInit();
	usartInit();
	while(1)
	{
		ioSetOutput(ioGetInputs());
		ioResetOutput(~ioGetInputs());
	}
}
/*
 * main.c
 *
 * Created: 2015-01-16 19:53:52
 *  Author: PLMABAJ
 */ 

#include "io.h"

int main(void)
{	
	ioInit();
    while(1)
    {
		ioSetOutput(ioGetInputs());
		ioResetOutput(~ioGetInputs());
	}
}
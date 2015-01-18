/*
 * usart.h
 *
 * Created: 2015-01-17 19:36:44
 *  Author: PLMABAJ
 */ 


#ifndef USART_H_
#define USART_H_

struct tx_buffer 
{
	uint8_t dstAddr;
	uint8_t srcAddr;	
	uint8_t len;
	uint8_t payload[20];	
};



#endif /* USART_H_ */
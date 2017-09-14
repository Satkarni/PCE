/*
 * RS485.c
 *
 * Created: 13-07-2015 10:26:37
 *  Author: ADMIN
 */ 

#ifndef F_CPU
#define F_CPU 14745600UL
#endif

#include "RS485_Remote.h"
#include <avr/io.h>
#include <util/delay.h>

void USART_enable(uint8_t rxenable,uint8_t txenable){
	
	if(txenable){
		
		UCSRB |= (1<<TXEN) | (1<<UDRE );
		RTS_PORT |= (1<<RTS_PIN);   
		_delay_us(1);
	}
	else{
		
		UCSRB &= ~(1<<UDRE );
	}
	
	if(rxenable){
		
		UCSRB |= (1<<RXEN) | (1<<RXCIE );
		RTS_PORT &= ~(1<<RTS_PIN);
		_delay_us(1);
	}
	else{
		
		UCSRB &= ~((1<<RXEN) | (1<<RXCIE ));
	}
}
void USART_init(uint16_t ubrr_value){

	RTS_DIR |= (1<<RTS_PIN);	// rts pin is output

   UBRRL = ubrr_value;
   UBRRH = (ubrr_value>>8);
   
   /*Set Frame Format
   >> Asynchronous mode
   >> NO Parity
   >> 1 StopBit
   >> char size 8     */
   UCSRC= (3<<UCSZ0);
   USART_enable(0,0);
}
uint8_t USART_readchar(){
	
	
	while(!(UCSRA & (1<<RXC))){
	}
	return UDR;
}

void USART_writechar(uint8_t data)
{
	
	
	while(!(UCSRA & (1<<UDRE))) {
	}
	UDR = data;
	
}
uint16_t get_CRC(volatile uint8_t *crc_p, uint8_t length){

	uint16_t crc;
	uint8_t thisbyte,shift,lastbit,l;
	
	crc=0xffff;
	for (l=0; l<length; l++)
	{
		thisbyte= *(crc_p+l);
		crc = crc^thisbyte;
		for (shift=1; shift<=8; shift++)
		{
			lastbit = crc & 1;
			crc = (crc >> 1) & 0x7fff;
			if (lastbit==1)
			{
				crc = crc^0xA001 ;
			}
		}
	}
	return crc;
}

/*
 * RS232.c
 *
 * Created: 9/6/2016 11:11:31 AM
 *  Author: Piyush
 */ 

#include "RS232.h"

void RS232_enable(uint8_t rxenable,uint8_t txenable)
{

	if(rxenable)
	{
		UCSR1B |= (1<<RXEN1) | (1<<RXCIE1 );
		RTS_PORT &= ~(1<<RTS_PIN);
	}
	else
	{
		UCSR1B &= ~((1<<RXEN1) | (1<<RXCIE1 ));
	}

	if(txenable)
	{
		UCSR1B |= (1<<TXEN1);
		RTS_PORT |= (1<<RTS_PIN);
	}
	else
	{
		UCSR1B &= ~(1<<TXEN1 );
	}
}
void RS232_init(uint16_t ubrr_value)
{
	RTS_DIR |= (1<<RTS_PIN);	// rts pin is output
	RTS_PORT |= (1<<RTS_PIN);   // rts pin is high
	
	UBRR1L = ubrr_value;
	UBRR1H = (ubrr_value>>8);

	/*Set Frame Format
   >> Asynchronous mode
   >> NO Parity
   >> 1 StopBit
   >> char size 8     */

	UCSR1C= (1<<UCSZ11)|(1<<UCSZ10);
	RS232_enable(0,0);
}
uint8_t RS232_readchar()
{
	while(!(UCSR1A & (1<<RXC1))){
	}
	return UDR1;
}
void RS232_writechar(uint8_t data)
{
	while(!(UCSR1A & (1<<UDRE1))) {
	}
	UDR1=data;
}
void RS232_writeString(char *data)
{
	while(*data != 0)
	{
		RS232_writechar(*data);
		data++;
	}
}
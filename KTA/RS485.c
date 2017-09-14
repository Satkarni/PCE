/* RS485.c
 *
 *  Created on: 24-Aug-2016
 *      Author: piyush
 */

#include "RS485.h"

void RS485_enable(uint8_t rxenable,uint8_t txenable)
{

	if(rxenable)
	{
		UCSR0B |= (1<<RXEN0) | (1<<RXCIE0 );
		RTS_PORT &= ~(1<<RTS_PIN);
	}
	else
	{
		UCSR0B &= ~((1<<RXEN0) | (1<<RXCIE0 ));
	}

	if(txenable)
	{
		UCSR0B |= (1<<TXEN0);
		RTS_PORT |= (1<<RTS_PIN);
	}
	else
	{
		UCSR0B &= ~(1<<TXEN0 );
	}
}
void RS485_init(uint16_t ubrr_value)
{
	RTS_DIR |= (1<<RTS_PIN);	// rts pin is output
	RTS_PORT |= (1<<RTS_PIN);   // rts pin is high
	
	UBRR0L = ubrr_value;
	UBRR0H = (ubrr_value>>8);

	/*Set Frame Format
   >> Asynchronous mode
   >> NO Parity
   >> 1 StopBit
   >> char size 8     */

	UCSR0C= (1<<UCSZ01)|(1<<UCSZ00);
	RS485_enable(0,0);
}
uint8_t RS485_readchar()
{
	while(!(UCSR0A & (1<<RXC0))){
	}
	return UDR0;
}
void RS485_writechar(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0))) {
	}
	UDR0=data;
}
void RS485_writeString(char *data)
{
	while(*data != 0)
	{
		RS485_writechar(*data);
		data++;
	}
}

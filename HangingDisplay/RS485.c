/* RS485.c
 *
 *  Created on: 24-Aug-2016
 *      Author: piyush
 */

#include "RS485.h"
#include "Hardware.h"
void RS485_enable(uint8_t rxenable,uint8_t txenable)
{
	if(rxenable)
	{
		UCSR0B |= (1<<RXEN0) | (1<<RXCIE0 );
	}
	else
	{
		UCSR0B &= ~((1<<RXEN0) | (1<<RXCIE0 ));
	}

	if(txenable)
	{
		UCSR0B |= ((1<<TXEN0)|(1<<TXCIE0));
	}
	else
	{
		UCSR0B &= ~((1<<TXEN0)|(1<<TXCIE0));
	}
}
void RS485_init(uint16_t ubrr_value)
{
	RTS_DIR |= (1<<RTS_PIN);	// rts pin is output
	RTS_PORT &= ~(1<<RTS_PIN);   // rts pin is low

	UBRR0L = ubrr_value;
	UBRR0H = (ubrr_value>>8);

	/*Set Frame Format
   >> Asynchronous mode
   >> Even Parity.....Needed for this project specifically as opposed to usual no parity
   >> 1 StopBit
   >> char size 8     */

#ifdef EVEN_PARITY
	UCSR0C |= (1<<UPM01);
#else
	UCSR0C &= ~(1<<UPM01);
#endif

	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	RS485_enable(1,1);
}
uint8_t RS485_readchar()
{
	while(!(UCSR0A & (1<<RXC0))){ }
	return UDR0;
}
void RS485_writechar(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0))) {}
	UDR0=data;

	RTS_PORT |= (1<<RTS_PIN);
}
void RS485_writeString(char *data)
{
	while(*data != 0)
	{
		RS485_writechar(*data);
		data++;
	}
}

uint16_t getCRC(uint8_t *crc_p, uint8_t length){

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

uint8_t checkCRC(uint8_t *buffer,uint8_t length)
{
	uint8_t  crcError = 0;
	uint16_t crcRx = 0;
	crcRx |= buffer[14]<<8;
	crcRx |= buffer[15];
	uint16_t crcCalc = getCRC(buffer,length);

	if(crcCalc != crcRx) crcError = 1;
	return crcError;
}

/*
 * RS485.c
 *
 * Created: 13-07-2015 10:26:37
 *  Author: ADMIN
 */

#ifndef F_CPU
#define F_CPU 14745600UL
#endif

#include "RS485_Local.h"
#include <avr/io.h>
#include <util/delay.h>

/*! \fn void USART_enable(uint8_t rxenable,uint8_t txenable)
		\brief Enables or disables USART reception or transmission
		@param rxenable Input 1 to enable reception, input 0 to disable reception
		@param txenable Input 1 to enable transmission, input 0 to disable transmission

		For this project we only use RS485 in simplex mode. Hence LCP always transmits and RCP always receives.

		\warning Enabling reception on LCP may cause communication to fail !
*/
void USART_enable(uint8_t rxenable,uint8_t txenable){

	if(rxenable){

		UCSR1B |= (1<<RXEN1) | (1<<RXCIE1 );
		RTS_PORT &= ~(1<<RTS_PIN);
	}
	else{

		UCSR1B &= ~((1<<RXEN1) | (1<<RXCIE1 ));
	}

	if(txenable){

		UCSR1B |= (1<<TXEN1);
		RTS_PORT |= (1<<RTS_PIN);
	}
	else{

		UCSR1B &= ~(1<<TXEN1 );
	}
}

/*! \fn void USART_init(uint16_t ubrr_value)
		\brief Initialize USART for RS485/RS232 communication
		@param ubrr_value Used to set baud rate

		This needs to be called in initialization.\n
		ubrr_value is calculated automatically from macro, hence to change baud rate only the macro #BAUD needs to be changed.
*/
void USART_init(uint16_t ubrr_value){

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
   USART_enable(0,0);
}

/*!
	\fn uint8_t USART_readchar()
	\brief Returns a received byte
*/
uint8_t USART_readchar(){


	while(!(UCSR1A & (1<<RXC1))){
	}
	return UDR1;
}

/*!
	\fn uint8_t USART_writechar(uint8_t data)
	\brief Transmits a byte
	@param data Data byte to be sent
*/
void USART_writechar(uint8_t data)
{


	while(!(UCSR1A & (1<<UDRE1))) {
	}
	UDR1=data;


}


/*!
	\fn uint16_t get_CRC(uint8_t *crc_p, uint8_t length)
	\brief Returns CRC checksum
	@param crc_p Pointer to the input data array for which CRC needs to be calculated
	@param length Length of the array
	\return A 16 bit unsigned int which is calculated CRC for the input data

	Consider a sample array \n
	data[5] = {0,1,2,3,4} \n
	To get the CRC of this array call get_CRC as follows:\n
	uint16_t crc = get_crc(data,5);\n
	crc now contains CRC of the array.
*/
uint16_t get_CRC(uint8_t *crc_p, uint8_t length){

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

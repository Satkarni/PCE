
/*
 * HangingDisplay.c
 *
 *  Created on: 01-Feb-2017
 *      Author: piyush
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Hardware.h"
#include "LCDLib/LCD.h"
#include "RS485.h"
#include "Converter.h"

#define FRAME_END	( 0xAA )
#define FRAME_SIZE	( 8 )

volatile uint8_t receive_buffer[24];
volatile uint8_t i=0,update_flag;
uint32_t printValue=0;
uint8_t precision = 0;

void blink(uint8_t count)
{
	while(count-->0)
	{
		DEBUG_LED_ON;
		_delay_ms(200);
		DEBUG_LED_OFF;
		_delay_ms(200);
	}
}

void init()
{
	DDRC |= (1<<DEBUG_LED);

	// DIP switch pins input and pull ups enabled
	DDRD &= ~((1<<DIP1)|(1<<DIP2)|(1<<DIP3));
	DDRB &= ~((1<<DIP4));
	PORTD |= (1<<DIP1)|(1<<DIP2)|(1<<DIP3);
	PORTB |= (1<<DIP4);

	spi_init();
	RS485_init(BAUDRATE_485);

	blink(2);
	LCD_clear();
	_delay_ms(500);
	for(uint8_t i=0; i<4; i++)
	{
		LCD_print(8888,3-i);
		_delay_ms(200);
	}
}


void test()
{
	uint16_t count = 0;
	while(count++<9999)
	{
		LCD_print(count,count%4);
		_delay_ms(200);
		DEBUG_LED_TOGGLE;
	}
}

uint8_t getDIPswitch()
{
	/* switchState stores the state of the switches in lower nibble
	 *
	 * switchState		x	x	x	x	DIP1	DIP2	DIP3	DIP4
	 * Bit Number		7	6	5	4	3		2		1		0
	 */

	uint8_t switchState = 0;

	if(DIP1_ON)	switchState |= (1<<3);
	else		switchState &= ~(1<<3);

	if(DIP2_ON)	switchState |= (1<<2);
	else		switchState &= ~(1<<2);

	if(DIP3_ON)	switchState |= (1<<1);
	else		switchState &= ~(1<<1);

	if(DIP4_ON)	switchState |= (1<<0);
	else		switchState &= ~(1<<0);

	return switchState;
}


ISR(USART_TX_vect)
{
	RTS_PORT &= ~(1<<RTS_PIN);		// switch to receive mode
}
ISR(USART_RX_vect)
{
#if defined(SLAVE) && !defined(MASTER)
	receive_buffer[i++] = RS485_readchar();
	if(i >= FRAME_SIZE) // check if all data received on end of message
	{
		i=0;
		update_flag = 1;
	}
	else
		update_flag = 0;

#endif

#if defined(MASTER) && !defined(SLAVE)
	uint8_t dummy = RS485_readchar();	// read UDR to clear RXC, else continuous receive interrupt
#endif

#ifdef TEST_ECHO
	uint8_t data = RS485_readchar();
	RS485_writechar(data);
#endif
}

void processBuffer()
{
#if defined(SLAVE) && !defined(MASTER)
	uint8_t parameterAddress = getDIPswitch();
	blink(parameterAddress);
	uint8_t firstRun = 0;
	uint32_t tmp1,tmp2,tmp3;
	double tmpValue;

	if(parameterAddress == receive_buffer[0])
	{

		if(firstRun == 0)
		{
			switch(parameterAddress)
			{
			case 0:
				precision = 1;
				break;

			case 1:
				precision = 2;
				break;

			case 2:
				precision = 1;
				break;

			default:
				precision = 0;
				break;
			}
			firstRun = 1;
		}

		uint32_t value = 0;
		tmp1 = receive_buffer[1];
		tmp2 =receive_buffer[2];
		tmp3 =receive_buffer[3];
		value = (tmp1 << 24) | (tmp2 << 16) | (tmp3 << 8) | (receive_buffer[4]);

		for(uint8_t i=0; i<7; i++) receive_buffer[i] = 0;

		tmpValue = bin32ToDouble(value);

		for(uint8_t i = 0; i < precision; i++)
			tmpValue *= 10;

		//		printValue = (uint32_t)(ceil(tmpValue));
		printValue = (uint32_t)(ceil(value));
	}
#endif
}

void pushData()
{
#if defined(MASTER) && !defined(SLAVE)
	RS485_writechar(0x00);
	RS485_writechar(0x43);
	RS485_writechar(0xcf);
	RS485_writechar(0x8c);
	RS485_writechar(0xcd);
	RS485_writechar(0x00);
	RS485_writechar(0x00);
	RS485_writechar(0xaa);

	DEBUG_LED_TOGGLE;
#endif
}

int main()
{

	init();
	sei();
	while(1)
	{
		//test();

#if defined(SLAVE)
		if(update_flag)
		{
			// process data and print to LED
			processBuffer();
			LCD_print(printValue,precision);
			update_flag = 0;
			_delay_ms(200);
		}
#elif defined(MASTER)
		pushData();
		_delay_ms(500);
#endif

	}
	return 0;
}





/*
 * Modbus.c
 *
 *  Created on: 03-Mar-2017
 *      Author: piyush
 */

#include "Modbus.h"
void enable_timer0_35char(uint8_t frame_ch_init)
{
	switch(frame_ch_init)
	{
	case 0:
		TCNT0 = 244;									 // 11 ticks ~ 750 us  inter character
		break;
	case 1:
		TCNT0 = 230;									// 25 ticks ~ 1750 us inter frame
		break;
	case 2:											// 255 ticks ~ 17 ms initial wait for server response
		TCNT0 = 0;
		break;
	}

	TCCR0A |= (1<<CS02)|(1<<CS00);   // Timer 0 normal mode 1/1024 prescaler (1 tick ~ 70 us)
	TIMSK0 |= (1<<TOIE0);			 // Timer 0 overflow interrupt enable

}

void disable_timer0_35char()
{
	TCCR0A &= ~((1<<CS02)|(1<<CS00));
	TCNT0 = 0x00;
	TIMSK0 &= ~(1<<TOIE0);
}

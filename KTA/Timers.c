/*
 * Timers.c
 *
 *  Created on: 26-Sep-2016
 *      Author: piyush
 */

#include "Timers.h"

void initTimers()
{
	Timers_timer2Init();
	Timers_RPMTimerInit();
	Timers_timer2Enable(TIMER2_RATE);
}

void Timers_timer2Init(void)
{
	TCCR2A |= ((1<<WGM21)|(0<<WGM20));		// CTC Mode
}

void Timers_timer2Enable(uint16_t rate)
{
	Enter_Critical_Section
		OCR2A = rate;
	Leave_Critical_Section
	TIMSK2 |= (1<<OCIE2A);
	TCCR2A |= ((1<<CS22)|(1<<CS21)|(0<<CS20));	// clk/256  prescaler
}

void Timers_RPMTimerInit(void)
{
	DDRE &= ~(1<<RPM_INPUT);
	PORTE |= (1<<RPM_INPUT);
}
void Timers_RPMTimerEnable(void)
{
	TCNT3 = 0x0000;
	TCCR3B |= (1<<CS32)|(1<<CS31) ;   //T1 external clock falling edge
}
void Timers_RPMTimerDisable(void)
{
	TCCR3B &= ~((1<<CS32)|(1<<CS31)) ;
}

void Timers_PWMInit()
{
	TCCR1B |= ((0<<WGM13) | (1<<WGM12));
	TCCR1A |= ((0<<WGM11) | (1<<WGM10) | (1<<COM1A1) | (0<<COM1A0));
	DDRB |= (1<<PWM_OUT);
	PORTB &= ~(1<<PWM_OUT);
	Enter_Critical_Section
	OCR1A = 0x0000;
	Leave_Critical_Section
}

void Timers_setDutyCycle(uint16_t duty_cycle_percentage)
{
	duty_cycle = 2.55*(double)duty_cycle_percentage;
	Enter_Critical_Section
		OCR1A = (uint16_t)duty_cycle;
	Leave_Critical_Section
}

void Timers_PWMEnable()
{
	TCCR1B |= ((1<<CS12) | (0<<CS11) | (1<<CS10));
}

void Timers_PWMDisable()
{
//	TCNT1 = 0x0000;
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}

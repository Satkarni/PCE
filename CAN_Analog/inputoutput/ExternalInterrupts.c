/*
 * ExternalInterrupts.c
 *
 * Created: 8/2/2016 11:22:02 AM
 *  Author: Piyush
 */ 

#include "ExternalInterrupts.h"

static volatile uint16_t timer_counter;

void Ext_Int_init()
{
	EICRA |= ((1<<ISC01)|(1<<ISC00)) ;  //Rising edge INT0 (brightness--)
	EICRB |= ((1<<ISC71)|(1<<ISC70)|(1<<ISC61)|(1<<ISC60)|(1<<ISC41)|(1<<ISC40)); // Rising edge INT7 (mute), INT4 ( brightness++), INT6 (Back)
	EIMSK |= ((1<<INT0)|(1<<INT4)|(1<<INT6)|(1<<INT7)); // INT0, INT4, INT6, INT7 ext interrupt enabled
}

void initHardwareTimer1(void)
{
	TCCR1B |= ((0<<WGM13)|(1<<WGM12));

	//Code for Overflow timer
//	TCCR1B |= (1<<CS11);
//	TCNT1 = 0;
//	TIMSK1 |= (1<<TOIE1);
//	timer_counter = 0;
}

void enableHardwareTimer1(uint16_t rate)
{
	Enter_Critical_Section
	OCR1A = rate;
	Leave_Critical_Section

	TIMSK1 |= (1<<OCIE1A);
	TCCR1B |= ((1<<CS12)|(0<<CS11)|(0<<CS10));	//CLK/256 Prescaler
	timer_counter = 0;
}


ISR(TIMER1_COMPA_vect)		// Main update interrupt
{
	if(timer_counter >= 3)		//At 250ms x 4 = 1 second
	{
		timer1sFlag = 1;
		timer_counter = 0;
	}
	else
	{
		timer_counter += 1;
	}
	timerIntFlag = 1;
	wdt_reset();
	DEBUG_LED2_TOGGLE;
}

ISR(TIMER1_OVF_vect)
{
	if(timer_counter % 8 == 0)
	{
		DEBUG_LED2_TOGGLE;
	}

	if(timer_counter >= 31)		//At 1 second
	{
		timer1sFlag = 1;
		timer_counter = 0;
	}
	else
	{
		timer_counter += 1;
	}
}

ISR(INT6_vect)				// Back key or Reset key
{
	setKeypadEvent(BACK_RESET);
}


ISR(INT0_vect)				// Brightness --
{
	display_decrementBrightness();
}

ISR(INT4_vect)				// Brightness ++
{
	display_incrementBrightness();
}

ISR(INT7_vect)			//Mute
{
	muteSensors();
}

ISR(BADISR_vect)
{
	DEBUG_LED1_ON;
}

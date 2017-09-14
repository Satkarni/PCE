/*
 * Overspeed.c
 *
 * Created: 01/08/2016 15:21:35
 *  Author: ROHANP
 */ 

#define F_CPU ( 14745600UL )

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "Config.h"
#include "Hardware.h"
double rpm=0;
typedef struct timer_t
{
	int trip : 1;
	int enable : 1;
	uint8_t delay;
	int start : 1;
}timer_t;
volatile timer_t overspeedTrip;

void rpm_timer_init(void){
	
	DDRD &= ~(1<<RPM_INPUT);
	PORTD |= (1<<RPM_INPUT);
}
void rpm_timer_enable(void){
	
	TCNT1 = 0x0000;
	TCCR1B |= (1<<CS12)|(1<<CS11) ;   //T1 external clock falling edge
}
void rpm_timer_disable(void){
	
	TCCR1B &= ~((1<<CS12)|(1<<CS11)) ;
	
}

void init(void)
{
	DDRD &= ~(1<<SW_CAL);
	DDRC |=  ((1<<DEBUG_LED)|(1<<RELAY1)|(1<<RELAY2));
	DDRC |= ((1<<GND_COMMON));
	
	PORTC &= ~((1<<GND_COMMON));
	RELAY1_OFF;
	RELAY2_OFF;
	EICRA |= ((1<<ISC01)|(0<<ISC00));
	EIMSK |= (1<<INT0);
	rpm_timer_init();
	USART_init(BAUDRATE);
}

void test(void)
{
	static uint8_t state;
	
	if(SW_CAL_ON)
	{
		_delay_ms(350);
		state = (state == 1) ? 0 : 1;
	}
	
	if(state)
	{
		RELAY1_ON;
		RELAY2_OFF;
		DEBUG_LED_ON;
	}else
	{
		RELAY1_OFF;
		RELAY2_ON;
		DEBUG_LED_OFF;
	}
}

void processRPM(settings_t *settings)
{
	
	rpm_timer_disable();
	rpm = TCNT1;
	rpm = (rpm <= FREQ_MIN) ? 0 :
	((rpm >= FREQ_MAX)? 0 : rpm) ;	// limit freq between 60< freq < freq_max
	
	rpm = (rpm/(settings->numberofTeeth))*RPM_SECONDS ;			// convert to rpm
	setCurrentspeed((uint16_t)rpm);
	
	if(rpm <= FREQ_MIN)
	{
		DEBUG_LED_TOGGLE;
	}else
	{
		DEBUG_LED_OFF;
	}
	//if(rpm >= RPM_STARTER) RELAY2_ON;		// starter cutoff relay
	//else RELAY2_OFF;
	
	if(rpm >= settings->rpmOverspeed)
	{
		overspeedTrip.delay++;
	}
	
	if(overspeedTrip.delay >= (settings->tripDelay) )
	{
		overspeedTrip.enable = 1;
		overspeedTrip.delay = 0;
	}
	if(rpm >= settings->rpmOverspeed && overspeedTrip.enable == 1)
	{
		RELAY1_ON;
		overspeedTrip.trip = 1;
		overspeedTrip.enable = 0;
	}
	/*
	if(overspeedTrip.trip && rpm <= RPM_RESET)
	{
		RELAY1_OFF;
		overspeedTrip.trip = 0;
	}
	*/	
	rpm_timer_enable();

	
	_delay_ms(1000);
	
}

ISR(INT0_vect)
{
	if(rpm < currentSettings.rpmOverspeed)
	{
		RELAY1_OFF;
		overspeedTrip.trip = 0;
	}
}
int main(void) 
{
	init();

	DEBUG_LED_ON;
	_delay_ms(250);
	DEBUG_LED_OFF;
	_delay_ms(250);
	setcurrentSettings(&currentSettings);
	sei();
	while(1)
	{
		processRPM(&currentSettings);
		processSendCurrentRPM();
	}
	
	return 0;
}
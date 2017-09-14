/*
 * CILOverspeedRev3.c
 *
 * Created: 01/12/2016 15:30:12
 *  Author: ROHANP
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Hardware.h"
#include "Config.h"

double rpm = 0;
volatile uint8_t testEnabled;
volatile uint8_t portbhistory = 0xFF;     // default is high because
volatile uint8_t changedbits;

struct settings_t
{
	uint16_t rpmOverspeed;
	uint16_t numberofTeeth;
}settings_t;

struct settings_t settings;

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


void init()
{
	
	DDRB &= ~( (1<<TEST)|(1<<RESET) );
	PORTB |= ( (1<<TEST)|(1<<RESET) );
	
	DDRD &= ~( (1<<DIP_1)|(1<<DIP_2)|(1<<DIP_3) );
	PORTD |= ( (1<<DIP_1)|(1<<DIP_2)|(1<<DIP_3) );
	
	DDRB |= (1<<RELAY1);
	DDRD |= (1<<POWER_LED);
	
	POWER_LED_ON;
	rpm_timer_init();
	
	GIMSK |= (1 << PCIE0);    // set PCIE0 to enable GIMSK scan
	PCMSK0 |= ((1 << PCINT1)|(1<<PCINT2));  // set PCINT1,2 to trigger an interrupt on state change
}


void processRPM(void)
{	
	rpm_timer_disable();
	rpm = TCNT1;
	rpm = (rpm <= FREQ_MIN) ? 0 :
	((rpm >= FREQ_MAX)? 0 : rpm) ;	// limit freq between 60< freq < freq_max
	
	rpm = (rpm/(settings.numberofTeeth))*RPM_SECONDS ;			// convert to rpm
		
	uint16_t rpmOVS;
		
	if(testEnabled == 1)
	{
	    rpmOVS = settings.rpmOverspeed * 0.8;
	}
	else
	{
	    rpmOVS = settings.rpmOverspeed;
	}

	
	if(rpm >= rpmOVS )
	{
		RELAY1_ON;
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

ISR(PCINT0_vect)
{
	
	changedbits = PINB ^ portbhistory;
	portbhistory = PINB;
	
	//RELAY1_OFF;
	
	if(changedbits & (1 << PINB2))
	{
		
		/* Reset changed */
		if( RESET_ON )
		{
			/* HIGH to LOW pin change */
			
			if(rpm < settings.rpmOverspeed && testEnabled == 0)
			{
				RELAY1_OFF;
			}
		}
		else
		{
			/* LOW to HIGH pin change */
		}
	}
	
	
	if(changedbits & (1 << PINB1))
	{
		/* Test changed */
		if( TEST_ON )
		{
			/* HIGH to LOW pin change */
			testEnabled = 1;
		}
		else
		{
			/* LOW to HIGH pin change */
			testEnabled = 0;
		}
	}
}

/* This function to be used only when DIP switch is present */
#ifdef DIP_SW_PRESENT
void getSettings(void)
{
	if(DIP_1_OFF && DIP_2_OFF && DIP_3_OFF) // 0 0 0
	{
		settings.numberofTeeth = 142;
		settings.rpmOverspeed = 2160;
	}
	
	if(DIP_1_OFF && DIP_2_OFF && DIP_3_ON) // 0 0 1
	{
		settings.numberofTeeth = 142;
		settings.rpmOverspeed = 2070;
	}
	
	if(DIP_1_OFF && DIP_2_ON && DIP_3_OFF) // 0 1 0
	{
		settings.numberofTeeth = 142;
		settings.rpmOverspeed = 2480;
	}
	
	if(DIP_1_OFF && DIP_2_ON && DIP_3_ON) // 0 1 1
	{
		settings.numberofTeeth = 142;
		settings.rpmOverspeed = 2520;
	}
	
	if(DIP_1_ON && DIP_2_OFF && DIP_3_OFF) // 1 0 0
	{
		settings.numberofTeeth = 118;
		settings.rpmOverspeed = 2160;
	}
	
	if(DIP_1_ON && DIP_2_OFF && DIP_3_ON) // 1 0 1
	{
		settings.numberofTeeth = 118;
		settings.rpmOverspeed = 2070;
	}
	
	if(DIP_1_ON && DIP_2_ON && DIP_3_OFF) // 1 1 0 
	{
		settings.numberofTeeth = 118;
		settings.rpmOverspeed = 2480;
	}
	
	if(DIP_1_ON && DIP_2_ON && DIP_3_ON) // 1 1 1 -> default
	{
		settings.numberofTeeth = 118;
		settings.rpmOverspeed = 2520;
	}
}
#endif


#ifdef DIP_SW_ABSENT
void getSettings(void)
{
	settings.numberofTeeth = NUMBER_OF_TEETH;
	settings.rpmOverspeed = RPM_OVERSPEED;
}
#endif

void test(void)
{
	if(TEST_ON)
	{
		_delay_ms(300);
		RELAY1_ON;
	}
	if(RESET_ON)
	{
		_delay_ms(300);
		RELAY1_OFF;
	}
}

int main(void)
{
    init();
	sei();
	while(1)
    {
        getSettings();
		processRPM();
		changedbits = 0;
    }
	
	return 0;
}
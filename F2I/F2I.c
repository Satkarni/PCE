/*
 * _2015_08_29_F2I.c
 *
 * Created: 29-08-2015 12:30:22
 *  Author: ADMIN
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "Config.h"
#include "Hardware.h"


uint8_t overspeed_trip = 0;
uint8_t overspeed_trip_en = 0;
uint8_t overspeed_trip_delay = 0;
uint8_t overspeed_trip_start = 0;

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

void spi_Init(void){
	
	DDRB |= (1<<PIN_DF_SS);
	PORTB &= ~(1<<PIN_DF_SS);
	
	DDRB |= (1<<PIN_SS)|(1 << PIN_SCK) | (1 << PIN_MOSI);
	// SPI Enable, Master mode, f_cpu/128
	SPCR |= (1 << SPE) | (1 << MSTR)|(1<<SPR1)|(1<<SPR0);
	
}
void spi_SendByte (uint8_t databyte)
{
	// Copy data into the SPI data register
	SPDR = databyte;
	// Wait until transfer is complete
	while (!(SPSR & (1 << SPIF))){	};
	
}

void AD420_Setcurrent(uint16_t current){
	
	CS_LOW;
	spi_SendByte(current>>8);
	spi_SendByte(current);  
	CS_HIGH; 
}

void init(void)
{
	DDRD &= ~(1<<SW_CAL);
	DDRC |=  ((1<<DEBUG_LED)|(1<<AD420_CLEAR)|(1<<RELAY1)|(1<<RELAY2));
	DDRC |= ((1<<GND_COMMON));
	DDRC &= ~(1<<AD420_FAULT);
	PORTC &= ~((1<<AD420_CLEAR)|(1<<GND_COMMON));
	RELAY1_OFF;
	RELAY2_OFF;
	
	spi_Init();
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

void calibrate(void)
{
	static uint16_t current;
	
	current = 0;
	AD420_Setcurrent(current);
	do
	{
		DEBUG_LED_TOGGLE;
		_delay_ms(100);
	}while(!SW_CAL_ON);
	
	current = 0xFFFF;
	AD420_Setcurrent(current);
	do
	{
		DEBUG_LED_TOGGLE;
		_delay_ms(200);
	}while(!SW_CAL_ON);

}

void process_rpm(settings_t *set)
{ 
	   double rpm=0;
	   double current=0,scale = 0;
	   rpm_timer_disable();
	   rpm = TCNT1*2;
	   rpm = (rpm <= FREQ_MIN) ? 0 :
	   ((rpm >= FREQ_MAX)? 0 : rpm) ;	// limit freq between 60< freq < freq_max
	   
	    rpm = (rpm/set->numberofTeeth)*RPM_SECONDS ;			// convert to rpm
		
		if(set->rly1Enable == 1)
		{
			if(rpm >= set->rly1onRpm) RELAY1_ON;					// Relay 1
			if(set->rly1Resettype == 0)								// Auto reset for Relay 1
			{
				if(rpm <= set->rly1offRpm) RELAY1_OFF;
			}
		}
		
		if(set->rly2Enable == 1)
		{
			if(rpm >= set->rly2onRpm) RELAY2_ON;					// Relay 2
			if(set->rly2Resettype == 0)								// Auto reset for Relay 2
			{
				if(rpm <= set->rly2offRpm) RELAY2_OFF;
			}
		}
		
		if(set->outputType == 0)		// Current output
		{
			scale = ( 65535/(set->maxRpm - set->minRpm) );
			current = (rpm - set->minRpm)*scale;
			current = (current < 0) ? 0 : ( (current >= 0xFFFF) ? 0xFFFF : current );	// Bounding current value between 0 and 65535
			uint16_t u16_current = (uint16_t)current;
			 
			AD420_Setcurrent(u16_current); 
		}
		if(set->outputType == 1)		// Voltage output
		{
			// to do later
		}
		
		rpm_timer_enable();
		
		_delay_ms(500);
	   
}

int main(void)
{
	
	init();
	
	_delay_ms(500);
	
	if(SW_CAL_ON)
	{
		_delay_ms(2000);
		do 
		{
			calibrate();
		} while(1);
	}
	
	DEBUG_LED_ON;
	_delay_ms(500); 
	DEBUG_LED_OFF;
	_delay_ms(500);
	
	setcurrentSettings(&currentSettings);
	
	if(currentSettings.rly1Resettype == 1)
	{
		RELAY1_OFF;
	}
	if(currentSettings.rly2Resettype == 1)
	{
		RELAY2_OFF;
	}
	
	sei();
    while(1)
    {
		 process_rpm(&currentSettings);
    }

}



	
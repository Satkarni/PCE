/*
 * Exhaust_Temp_Relay.c
 *
 * Created: 16/12/2016 09:38:30
 *  Author: ROHANP
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "Hardware.h"
#include "ADC.h"
#include "Config.h"


struct tempSensor_
{
	uint8_t trip;
	uint8_t trip_en;
	uint8_t trip_delay;
	uint16_t adc;
};

struct tempSensor_ tempSensor1 =
{
	.trip = 0,
	.trip_en = 1,
	.trip_delay = 0,
	.adc = 0
};

struct tempSensor_ tempSensor2 =
{
	.trip = 0,
	.trip_en = 1,
	.trip_delay = 0,
	.adc = 0
};

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
	ADC_init();
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

void process_temp()
{
	
	
	tempSensor1.adc = ADC_read(ADC_IN1_CHANNEL);
	tempSensor2.adc = ADC_read(ADC_IN2_CHANNEL);
	  
	if(tempSensor1.adc >= ADC_TEMP1_SETPOINT)
	{
		tempSensor1.trip_delay++;
	}
	if(tempSensor1.adc >= ADC_TEMP1_SETPOINT && tempSensor1.trip_delay >= 5*TEMP1_TRIP_DELAY)
	{
		tempSensor1.trip = 1;
	}
	if(tempSensor1.trip == 1 && tempSensor1.trip_en == 1)
	{
		RELAY1_ON;
	} 
	if(tempSensor1.adc <= ADC_TEMP1_HYS)
	{
		RELAY1_OFF;
		tempSensor1.trip = 0;
		tempSensor1.trip_delay = 0;
	}
	
	
	
	
	if(tempSensor2.adc >= ADC_TEMP2_SETPOINT)
	{
		tempSensor2.trip_delay++;
	}else
	{
		tempSensor2.trip_delay = 0;
	}
	if(tempSensor2.adc >= ADC_TEMP2_SETPOINT && tempSensor2.trip_delay >= 5*TEMP2_TRIP_DELAY)
	{
		tempSensor2.trip = 1;
	}
	if(tempSensor2.trip == 1 && tempSensor2.trip_en == 1)
	{
		RELAY2_ON;
	}
	if(tempSensor2.adc <= ADC_TEMP2_HYS)
	{
		RELAY2_OFF;
		tempSensor2.trip = 0;
		tempSensor2.trip_delay = 0;
	}
	 
	 
	_delay_ms(200);
	  
}

int main(void)
{
	
	init();

	if(SW_CAL_ON)
	{
		_delay_ms(2000);
		do 
		{
			test();
		} while(1);		
	}
	
	DEBUG_LED_ON;
	_delay_ms(500); 
	DEBUG_LED_OFF;
	_delay_ms(500);
	

    while(1)
    {
		 process_temp();
    }

}



	
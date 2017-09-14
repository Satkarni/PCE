
#include <avr/io.h>
#include "ADC.h"

uint16_t EEMEM throttleMin;
uint16_t EEMEM throttleMax;
uint16_t EEMEM actuatorMax;

void ADC_init(){
	
	ADMUX |= (1<<REFS0); // Aref=Vcc
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //F_CPU / 128  sampling rate = 115.2 ks per sec
	ADC_enable();

	uint16_t tmp;
	tmp = eeprom_read_word(&throttleMin);
	if (tmp == 0xFFFF)
	{
		eeprom_update_word(&throttleMin,0);
	}

	tmp = eeprom_read_word(&throttleMax);
	if (tmp == 0xFFFF)
	{
		eeprom_update_word(&throttleMax,1023);
	}

	tmp = eeprom_read_word(&actuatorMax);
	if (tmp == 0xFFFF)
	{
		eeprom_update_word(&actuatorMax,1023);
	}

}

void ADC_enable(){
	
	MCUCR = (1<<JTD);		// Disable JTAG for using ADC4
	MCUCR = (1<<JTD);
	
	ADCSRA |= (1<<ADEN);
}

void ADC_disable(){
	
	MCUCR &= ~(1<<JTD);		// Enable JTAG after ADC is disabled
	MCUCR &= ~(1<<JTD);
	
	ADCSRA &= ~(1<<ADEN);	
}

uint16_t ADC_read(uint8_t channel){
	
	ADMUX = (ADMUX& 0xF8)|channel;  
	
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC));
	return ADC;
}


uint16_t ADC_filterRead(uint8_t channel)
{
	static uint16_t prev_value[8];
	static uint16_t current_value[8];
	static uint8_t first_run[8] = {0};

	if(first_run[channel] == 0)
	{
		if(channel == ADC_THROTTLE_CHANNEL)
		{
		}
		first_run[channel] = 1;
		prev_value[channel] = ADC_read(channel);
		return current_value[channel];
	}

	current_value[channel] = (FILTER_ALPHA*ADC_read(channel)) + ((1-FILTER_ALPHA)*prev_value[channel]);
	prev_value[channel] = current_value[channel];
	return current_value[channel];
}

void ADC_updateThrottleMin ()
{
	uint16_t tmp;
	tmp = ADC_read(ADC_THROTTLE_CHANNEL);
	if(tmp > ADC_getThrottleMax())
		return;
	else
	{
		eeprom_update_word(&throttleMin, tmp);
	}
}

void ADC_updateThrottleMax ()
{
	uint16_t tmp;
	tmp = ADC_read(ADC_THROTTLE_CHANNEL);
	if(tmp < ADC_getThrottleMin())
		return;
	else
	{
		eeprom_update_word(&throttleMax, tmp);
	}
}

void ADC_updateActuatorMax ()
{
	uint16_t tmp;
	tmp = ADC_read(ADC_DRV_CHANNEL);
	if(tmp < 400)
		return;
	else
	{
		eeprom_update_word(&actuatorMax, tmp);
	}
}

uint16_t ADC_getThrottleMin()
{
	uint16_t tmp;
	tmp = eeprom_read_word(&throttleMin);
	return tmp;
}

uint16_t ADC_getThrottleMax()
{
	uint16_t tmp;
	tmp = eeprom_read_word(&throttleMax);
	return tmp;
}

uint16_t ADC_getActuatorMax()
{
	uint16_t tmp;
	tmp = eeprom_read_word(&actuatorMax);
	return tmp;

}

void ADC_resetDefaults(void)
{
	eeprom_update_word(&throttleMin,0);
	eeprom_update_word(&throttleMax,1022);
	eeprom_update_word(&actuatorMax,887);
}


#include <avr/io.h>
#include "ADC.h"


void ADC_init(){
	
	ADMUX |= (1<<REFS0); // Aref=Vcc
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //F_CPU / 128  sampling rate = 115.2 ks per sec
	ADC_enable();
}

void ADC_enable(){
	
	ADCSRA |= (1<<ADEN);
}

void ADC_disable(){
	
	ADCSRA &= ~(1<<ADEN);	
}

uint16_t ADC_read(uint8_t channel){
	
	ADMUX = (ADMUX & 0xF8)|channel;
	
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

uint16_t ADC_filterRead(uint8_t channel)
{
	static uint16_t prev_value[8];
	uint16_t current_value;
	static uint8_t first_run[8] = {0};

	if(first_run[channel] == 0)
	{
		first_run[channel] = 1;
		prev_value[channel] = ADC_read(channel);
		return prev_value[channel];
	}

	current_value = (FILTER_ALPHA*ADC_read(channel)) + ((1-FILTER_ALPHA)*prev_value[channel]);
	prev_value[channel] = current_value;
	return current_value;
}


#include <avr/io.h>
#include "ADC.h"


void ADC_init(){
	
	ADMUX |= (1<<REFS0); // Aref=Vcc
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //F_CPU / 128  sampling rate = 115.2 ks per sec
	ADC_enable();
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


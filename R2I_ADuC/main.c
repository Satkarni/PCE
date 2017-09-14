

#include<ADuC845.h>        	
#include<stdio.h>
#include "config.h"

unsigned long ADC=0;            				// raw ADC conversion result
unsigned long ADC_MAX = ADC_MAX_INIT;

unsigned long DAC_val=0;
int i=0; 
int state = 0;
void Delay(int loop){
    int j;
    int i;
    for(i=0;i<loop;i++)
    {
        for(j=0;j<133;j++)
        {
        }
    }
}


void ee_init(){

		EADRL = EE_PAGE_ADDRESS;  // set page address == 3
		ECON = 0x01;   			  // read page 3 into EDATA1-4
	
		if( (EDATA1 == 0xFF)&&(EDATA2 == 0xFF)&&(EDATA3 == 0xFF)&&(EDATA4 == 0xFF) ){  // adc_max not set at all
			
				ECON = 0x06;	//erase all
		}
}
unsigned long ee_read(){
		
		EADRL = EE_PAGE_ADDRESS;  // set page address == 3
		ECON = 0x01;   // read page 3 into EDATA1-4
		
		return ((EDATA1<<8)|EDATA2);
}
void ee_write(unsigned long value){
	
	EADRL = EE_PAGE_ADDRESS;  // set page address == 3
	ECON = 0x01;   			  // read page 3 into EDATA1-4
	
	EDATA1 = value>>8;
	EDATA2 = value; 
	
	ECON = 0x05;   // erase page (3)
	ECON = 0x02;   // write page with EDATA1-4
	
}


void ADC_init(){
	
	ADC0CON1 = 0x2F;      			// Full Buff, unipolar, 0->2.56V 
	
	SF = 0x0D;						// ADC freq = 105 Hz		
	ICON = 0x01;    				//Enable RTD#1 current source (IEXC1 on pin 11/Ain7)
}
	
void ADC_read(){
	
	unsigned char timeout = 0;
	unsigned char timeout_f = 0;
	ADC0CON2 = 0x4A;    			// Refin+/-, Ain1->Ain2
  	ADCMODE  = 0x22;    			// Single conv, primary ADC....AIN1
	Delay(1);

	while(!RDY0 && !timeout_f)		// wait for conversion completion
	{		
			timeout++;
			Delay(1);
		
			if(timeout >= 10)
			{
				ADC_init();
				timeout_f = 1;
			}
	}  	
	
	ADC = ADC0H*0x100+ADC0M;		// ignoring ADC0L, reducing to 16 bit resolution
		
	RDY0 = 0;	
}



void test(){
				
		float calc=0;
		float i =0;

		calc = DAC_MIN;
		while(1)
		{
					
		   calc = DAC_MIN; 			
		   DAC_val = (int) calc;
		   DACH = (DAC_val>>8);
		   DACL = DAC_val;		 
	       Delay(5000); 
				 
		   calc = DAC_MAX; 			
		   DAC_val = (int) calc;
		   DACH = (DAC_val>>8);
		   DACL = DAC_val;	
	       Delay(5000); 
	
		   while(calc < DAC_MAX)
		   {
			  	DAC_val = (int) calc;
				DACH = (DAC_val>>8);
				DACL = DAC_val;
				Delay(2000);
				
				calc += 200; 	
		   }
		   calc = DAC_MIN;
		}
}



void process_relay(){
	
	/*	switch(state){
				
				case OFF:
				
					if(resistance <= (THRESHHOLD - HYSTERISIS)) {
							RELAY = ON;
							state = ON;
					}
				break;
				
				case ON:
					
					if(resistance >= THRESHHOLD){
							RELAY = OFF;
							state = OFF; 
					}
					else {
							RELAY = ON;
							state = ON;
					}	
				
				break;
				
			}*/	
}



void main(){
	 
		unsigned long resistance=0;
		float calc=0;
		
	
		PLLCON = 0x03;      // 1.57 MHz 
		DACCON  = 0x03;    // Pin 14 DAC, 12 bit mode, 0-2.5V 
		DACH = 0x00;
		DACL = 0x00;
	
		RELAY = OFF;
		state = OFF;
		SET_MAX = 1;				// P3_0 is input, internal pull up on 
		
		ee_init();
		ADC_init();	 
		ADC_MAX = ADC_MAX_INIT;
	//	test();
	 while(1)                // infinite loop
    {
      
		
			if(!SET_MAX){				// switch pressed
					
					DACCON = 0x00;  // disabling DAC
					Delay(2);
					ADC_read();
					ee_write(ADC);
					Delay(1);
					DACCON = 0x03; 
			}
			ADC_read();
			resistance = ADC;
			resistance = 65535;
			if(resistance >= ADC_SENSOR_OPEN){
				resistance = ADC_MIN;		
			}
			
			if(resistance <= ADC_MIN){
				resistance = ADC_MIN;		
			}
			
		
			 
			calc = resistance*(DAC_MAX-DAC_MIN);
			calc = calc/(ADC_MAX);
			calc = calc+DAC_MIN; 
			
			
			DAC_val = (long) calc;
			DACH = (DAC_val>>8);
			DACL = DAC_val;	
			
			ADC_MAX = ee_read();	
		
      		Delay(DELAY); 
	 
    }
}


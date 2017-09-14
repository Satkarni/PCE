/*
 * _7219_display.c
 *
 * Created: 06-07-2015 18:16:27
 *  Author: ADMIN
 */ 
#include "7219_display_Local.h"
#include <avr/io.h>
#include <util/delay.h>

void spi_init(){
	
	 DDRB |= (1<<PIN_SS);
	 PORTB &= ~(1<<PIN_SS);
	 
	 DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI);
	
	 // SPI Enable, Master mode, f_cpu/128
	 SPCR |= (1 << SPE) | (1 << MSTR)|(1<<SPR1)|(1<<SPR0);
	
}
void spiSendByte (char databyte)
{
	// Copy data into the SPI data register
	SPDR = databyte;
	_delay_us(1);
	// Wait until transfer is complete
	while (!(SPSR & (1 << SPIF))){	};
	
}

void MAX7219_writeData(char data_register, char data, unsigned char chip)
{
	DISPLAY_PORT &= ~(1<<chip);				// Send the register where the data will be stored
	_delay_us(1);
	spiSendByte(data_register);			// Send the data to be stored
	_delay_us(1);
	spiSendByte(data);
	_delay_us(1);
	DISPLAY_PORT |= (1<<chip);
	_delay_us(1);
}

void MAX7219_clearDisplay(unsigned char chip)
{
	char i = digitsInUse;
	
	do {
		
		if(chip==CHIP3){
			
			MAX7219_writeData(MAX7219_DIGIT1, MAX7219_CHAR_BLANK_NO_DECODE,chip);
			MAX7219_writeData(MAX7219_DIGIT2, MAX7219_CHAR_BLANK_NO_DECODE,chip);
			i=1;
		}
		else{
			MAX7219_writeData(i, MAX7219_CHAR_BLANK,chip);
		}
	} while (--i);
}

void MAX7219_displayNumber(uint16_t number,uint8_t chip,uint8_t side,uint8_t decimal)
{
	char negative = 0;
	
	uint16_t temp;
	uint8_t i,j;
	
	
	if (number < 0) {
		negative = 1;
		number *= -1;
	}
			
	if(side == LEFT){ i = MAX7219_DIGIT4;j=0;}
	if(side == RIGHT){ i = MAX7219_DIGIT8;j=4;}
	do {
		MAX7219_writeData(i--, MAX7219_CHAR_BLANK,chip);
	} while (i>j);
	
	if(side == LEFT){ i = MAX7219_DIGIT4;j=4;}
	if(side == RIGHT){ i = MAX7219_DIGIT8;j=8;}
	
	temp = number;		//save number
			
	if (number == 0) {
		
		if(side ==LEFT){MAX7219_writeData(MAX7219_DIGIT4, 0,chip); }
		if(side == RIGHT){MAX7219_writeData(MAX7219_DIGIT8, 0,chip);} 
		return;
	}
	
	do{		
		if((i == (j-decimal)) && (decimal != NO_DPOINT) ){
			
			
			MAX7219_writeData(i--,(number%10)|DPOINT,chip);
		} 	
		else {
			MAX7219_writeData(i--, number%10,chip);
		}
		number /= 10;
			
	} while (number);
	
	if(decimal != NO_DPOINT){			//add preceding zeroes
		
			switch(decimal){
				
				case 1:
					if(temp<10){
						MAX7219_writeData(i,0|DPOINT,chip);
					}
				break;
				case 2:
					if(temp <100 && temp>=10){
						MAX7219_writeData(i,0|DPOINT,chip);
					}
					else if(temp<10){
						MAX7219_writeData(i--,0,chip);
						MAX7219_writeData(i,0|DPOINT,chip);
					}
				
				break;
				case 3:
					if(temp <1000 && temp >= 100){
						MAX7219_writeData(i,0|DPOINT,chip);
					}
					if(temp <100 && temp>=10){
						MAX7219_writeData(i--,0,chip);
						MAX7219_writeData(i,0|DPOINT,chip);
					}
					 if(temp<10){
						MAX7219_writeData(i--,0,chip);
						MAX7219_writeData(i--,0,chip);
						MAX7219_writeData(i,0|DPOINT,chip);
					}
				break;
				
			}
	}		
	
	
	if (negative) {
		MAX7219_writeData(i, MAX7219_CHAR_NEGATIVE,chip);
	}
	
			
}

void MAX7219_displayInit(){

	DISPLAY_DIR  |= (1<<LOAD1)|(1<<LOAD2)|(1<<LOAD3)|(1<<LOAD4);
	
	uint8_t chip;
	for(chip=0;chip<3;chip++){	 
		
		  MAX7219_clearDisplay(chip);	
		  // Decode mode to "Font Code-B"
		  MAX7219_writeData(MAX7219_MODE_DECODE, 0xFF,chip);
		  
		  // Scan limit runs from 0.
		  MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, digitsInUse - 1,chip);
		  MAX7219_writeData(MAX7219_MODE_INTENSITY, 8,chip);
		  MAX7219_writeData(MAX7219_MODE_POWER, ON,chip);
		  MAX7219_writeData(MAX7219_MODE_TEST,OFF,chip);
		  
		 _delay_ms(10);
	}
		chip = 3;
		
		 MAX7219_clearDisplay(chip);
		 // No decode
		 MAX7219_writeData(MAX7219_MODE_DECODE, 0x00,chip);
		 
		 // Scan limit runs from 0.
		 MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, digitsInUse - 1,chip);
		 MAX7219_writeData(MAX7219_MODE_INTENSITY, 8,chip);
		 MAX7219_writeData(MAX7219_MODE_POWER, ON,chip);
		 MAX7219_writeData(MAX7219_MODE_TEST,OFF,chip);
		 
		 _delay_ms(10);
	
}

void MAX7219_enableChip(unsigned char chip){
	
	DISPLAY_DIR |= (1<<LOAD1)|(1<<LOAD2)|(1<<LOAD3)|(1<<LOAD4);
	
	MAX7219_writeData(MAX7219_MODE_POWER, ON,chip);
	
}
void MAX7219_disableChip(unsigned char chip){
	
	DISPLAY_DIR |= (1<<LOAD1)|(1<<LOAD2)|(1<<LOAD3)|(1<<LOAD4);
	
	MAX7219_writeData(MAX7219_MODE_POWER, OFF,chip);
	
}

void MAX7219_display7segment(uint16_t data,DISPLAY display){
	
	switch (display)
	{	
		
	case  RPM :			// chip 0, side left
		
		MAX7219_displayNumber(data,CHIP0,LEFT,NO_DPOINT);	
		break;
		
	case  WATER_TEMP :	// chip 0, side right
		
		MAX7219_displayNumber(data,CHIP0,RIGHT,NO_DPOINT);
		break;
		
	case  LUBE_OIL_PRESSURE :
		
		MAX7219_displayNumber(data,CHIP1,LEFT,LUBE_OIL_PRESSURE_DPOINT);
		break;
		
	case LUBE_OIL_TEMP :
	
	MAX7219_displayNumber(data,CHIP1,RIGHT,NO_DPOINT);
	break;
	
	case BATTERY_VGE :
	
	MAX7219_displayNumber(data,CHIP2,LEFT,BATTERY_VGE_DPOINT);
	break;
	
	case RUN_TIME :
	
	MAX7219_displayNumber(data,CHIP2,RIGHT,NO_DPOINT);
	break;
	}
}

void MAX7219_displayLED(uint16_t state, uint8_t digit){
	
	if(digit == MAX7219_DIGIT1){
		MAX7219_writeData(MAX7219_DIGIT1,state,CHIP3);
	}
	if(digit == MAX7219_DIGIT2){
		MAX7219_writeData(MAX7219_DIGIT2,(state>>4),CHIP3);
	}
}

void MAX7219_displayError(SENSOR_ERROR_CODE error_code){

	uint8_t q = 0;
	switch(error_code){
		
		case RPM_SENSOR_FAIL :
		
				q = MAX7219_DIGIT4;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP0);
				} while (--q);
				MAX7219_writeData(MAX7219_DIGIT3,ERROR,CHIP0);
				MAX7219_writeData(MAX7219_DIGIT4,RPM_SENSOR_ERROR,CHIP0);
		break;
		
		case LOP_SENSOR_OFFSCALE_LOW :
		
				q = MAX7219_DIGIT4;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP1);
				} while (--q);
				MAX7219_writeData(MAX7219_DIGIT3,ERROR,CHIP1);
				MAX7219_writeData(MAX7219_DIGIT4,LOP_SENSOR_OSL_ERROR,CHIP1);
		break;
		
		case LOP_SENSOR_OFFSCALE_HIGH :
		
				q = MAX7219_DIGIT4;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP1);
				} while (--q);
				MAX7219_writeData(MAX7219_DIGIT3,ERROR,CHIP1);
				MAX7219_writeData(MAX7219_DIGIT4,LOP_SENSOR_OSH_ERROR,CHIP1);
		break;
		
		case LOT_SENSOR_OFFSCALE_LOW :
		
				q = MAX7219_DIGIT8;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP1);
				} while (--q > MAX7219_DIGIT4);
				MAX7219_writeData(MAX7219_DIGIT7,ERROR,CHIP1);
				MAX7219_writeData(MAX7219_DIGIT8,LOT_SENSOR_OSL_ERROR,CHIP1);
		break;
		
		case LOT_SENSOR_OFFSCALE_HIGH :
		
				q = MAX7219_DIGIT8;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP1);
				} while (--q > MAX7219_DIGIT4);
				MAX7219_writeData(MAX7219_DIGIT7,ERROR,CHIP1);
				MAX7219_writeData(MAX7219_DIGIT8,LOT_SENSOR_OSH_ERROR,CHIP1);
		break;
		
		case WT_SENSOR_OFFSCALE_LOW :
		
				q = MAX7219_DIGIT8;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP0);
				} while (--q > MAX7219_DIGIT4);
				MAX7219_writeData(MAX7219_DIGIT7,ERROR,CHIP0);
				MAX7219_writeData(MAX7219_DIGIT8,WT_SENSOR_OSL_ERROR,CHIP0);
		break;
		
		case WT_SENSOR_OFFSCALE_HIGH :
		
				q = MAX7219_DIGIT8;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP0);
				} while (--q > MAX7219_DIGIT4);
				MAX7219_writeData(MAX7219_DIGIT7,ERROR,CHIP0);
				MAX7219_writeData(MAX7219_DIGIT8,WT_SENSOR_OSH_ERROR,CHIP0);
		break;
		
		case BV_SENSOR_FAIL :
		
				q = MAX7219_DIGIT4;
				do {
					MAX7219_writeData(q, MAX7219_CHAR_BLANK,CHIP2);
				} while (--q);
				MAX7219_writeData(MAX7219_DIGIT3,ERROR,CHIP2);
				MAX7219_writeData(MAX7219_DIGIT4,BV_SENSOR_ERROR,CHIP2);
		break;
		
	}
}


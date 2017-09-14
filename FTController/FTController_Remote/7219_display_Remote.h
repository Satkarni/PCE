/*
 * _7219_display.h
 *
 * Created: 06-07-2015 18:22:21
 *  Author: ADMIN
 */ 


#ifndef _7219_DISPLAY_H_
#define _7219_DISPLAY_H_

 #include <avr/io.h>

#ifndef F_CPU
#define F_CPU 14745600UL
#endif

#define DISPLAY_DIR	 DDRB
#define DISPLAY_PORT PORTB
// Outputs, pin definitions
#define PIN_SCK PORTB7
#define PIN_MOSI PORTB5
#define LOAD1 PORTB0
#define LOAD2 PORTB1
#define LOAD3 PORTB2
#define LOAD4 PORTB3
#define LED9  PORTD6
#define ON 1
#define OFF 0

#define CHIP0 0
#define CHIP1 1
#define CHIP2 2
#define CHIP3 3
#define LEFT 1
#define RIGHT 2

#define CLK_HIGH  DISPLAY_PORT |= (1<<PIN_SCK)
#define CLK_LOW   DISPLAY_PORT &= ~(1<<PIN_SCK)
#define DATA_HIGH DISPLAY_PORT |= (1<<PIN_MOSI)
#define DATA_LOW  DISPLAY_PORT &= ~(1<<PIN_MOSI)


#define MAX7219_MODE_DECODE 0x09
#define MAX7219_MODE_INTENSITY 0x0A
#define MAX7219_MODE_SCAN_LIMIT 0x0B
#define MAX7219_MODE_POWER 0x0C
#define MAX7219_MODE_TEST 0x0F
#define MAX7219_MODE_NOOP 0x00

// For 8 digits
#define digitsInUse 8
#define MAX7219_DIGIT1 0x01
#define MAX7219_DIGIT2 0x02
#define MAX7219_DIGIT3 0x03
#define MAX7219_DIGIT4 0x04
#define MAX7219_DIGIT5 0x05
#define MAX7219_DIGIT6 0x06
#define MAX7219_DIGIT7 0x07
#define MAX7219_DIGIT8 0x08

// Decimal point related
#define DPOINT 0x80
#define NO_DPOINT 0x00
#define LUBE_OIL_PRESSURE_DPOINT 0x01
#define BATTERY_VGE_DPOINT	0x01


#define MAX7219_CHAR_BLANK 0x0F
#define MAX7219_CHAR_BLANK_NO_DECODE 0x00
#define MAX7219_CHAR_NEGATIVE 0x0A

#define ERROR 					0x0B
#define RPM_SENSOR_ERROR 		1
#define LOP_SENSOR_OSL_ERROR	2
#define LOP_SENSOR_OSH_ERROR	3
#define LOT_SENSOR_OSL_ERROR	4
#define LOT_SENSOR_OSH_ERROR	5
#define WT_SENSOR_OSL_ERROR		6
#define WT_SENSOR_OSH_ERROR 	7
#define BV_SENSOR_ERROR			8

typedef enum{

	RPM,
	WATER_TEMP,
	LUBE_OIL_PRESSURE,
	LUBE_OIL_TEMP,
	BATTERY_VGE,
	RUN_TIME
	
}DISPLAY;

typedef enum {

	RPM_SENSOR_FAIL,
	LOP_SENSOR_OFFSCALE_LOW,
	LOP_SENSOR_OFFSCALE_HIGH,
	LOT_SENSOR_OFFSCALE_LOW,
	LOT_SENSOR_OFFSCALE_HIGH,
	WT_SENSOR_OFFSCALE_LOW,
	WT_SENSOR_OFFSCALE_HIGH,
	BV_SENSOR_FAIL

}SENSOR_ERROR_CODE;

void MAX7219_displayInit();
void spiSendByte (char databyte);
void MAX7219_writeData(char data_register, char data,unsigned char chip );
void MAX7219_clearDisplay(unsigned char chip);
void MAX7219_displayNumber(uint16_t number,uint8_t chip,uint8_t side,uint8_t decimal);
void MAX7219_enable(unsigned char chip);
void MAX7219_disable(unsigned char chip);
void MAX7219_display7segment(uint16_t data,DISPLAY display);
void MAX7219_displayLED(uint8_t state);
void MAX7219_displayError(SENSOR_ERROR_CODE error_code);
#endif /* _7219_DISPLAY_H_ */
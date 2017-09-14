
/*
 * LCD.h
 *
 *  Created on: 01-Feb-2017
 *      Author: piyush
 */

#ifndef LCD_H_
#define LCD_H_

#include	"spi.h"
#include <math.h>

#define		SEGA		0
#define		SEGB		1
#define		SEGC		2
#define		SEGD		3
#define		SEGE		4
#define		SEGF		5
#define		SEGG		6
#define		SEGDP		7

uint8_t LCD_digitToSeg(uint8_t number);
uint8_t LCD_pointToSeg();
void LCD_print(uint32_t number, uint8_t precision);		//<Use for cascade mode
void LCD_clear();

void LCD_printLCD1(uint8_t digit, uint8_t enableDot);	//<Use only for non_cascade	//10^4
void LCD_printLCD2(uint8_t digit, uint8_t enableDot);	//<Use only for non_cascade	//10^3
void LCD_printLCD3(uint8_t digit, uint8_t enableDot);	//<Use only for non_cascade	//10^2
void LCD_printLCD4(uint8_t digit, uint8_t enableDot);	//<Use only for non_cascade	//10^1


#endif /* LCD_H_ */


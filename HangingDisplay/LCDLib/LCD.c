
/*
 * LCD.c
 *
 *  Created on: 01-Feb-2017
 *      Author: piyush
 */

#include	"LCD.h"


uint8_t LCD_digitToSeg(uint8_t number)
{
	if(number > 9)
		return 0;

	uint8_t segmentFormat = 0;

	switch (number)
	{
	case 0:
		segmentFormat = (1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF);
		break;

	case 1:
		segmentFormat = (1<<SEGB) | (1<<SEGC);
		break;

	case 2:
		segmentFormat = (1<<SEGA) | (1<<SEGB) | (1<<SEGD) | (1<<SEGE) | (1<<SEGG);
		break;

	case 3:
		segmentFormat = (1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGG);
		break;

	case 4:
		segmentFormat = (1<<SEGB) | (1<<SEGC) | (1<<SEGF) | (1<<SEGG);
		break;

	case 5:
		segmentFormat = (1<<SEGA) | (1<<SEGC) | (1<<SEGD) | (1<<SEGF) | (1<<SEGG);
		break;

	case 6:
		segmentFormat = (1<<SEGA) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
		break;

	case 7:
		segmentFormat = (1<<SEGA) | (1<<SEGB) | (1<<SEGC);
		break;

	case 8:
		segmentFormat = (1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
		break;

	case 9:
		segmentFormat = (1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) |  (1<<SEGF) | (1<<SEGG);
		break;
	}

	return segmentFormat;
}

uint8_t LCD_pointToSeg()
{
	uint8_t returnValue = (1<<SEGDP);
	return returnValue;
}


void LCD_print(uint32_t number, uint8_t precision)
{
	CS1_LOW;
	
	if(number > 9999)
		return;

	if(precision > 3)
		return;

	if(number == 0)
	{
		spi_tranceive(LCD_digitToSeg(0) | LCD_pointToSeg());		//Put 0 as first digit.
		spi_tranceive(0);											//Shift to right by 1
		spi_tranceive(0);										    //Shift to right by 1
		spi_tranceive(0);											//Shift to right by 1
	}

	uint32_t remaining = number;
	uint8_t digit, counter = 0;

	while(remaining != 0)
	{
		digit = remaining%10;
		remaining /= 10;
		if(counter == precision)
			spi_tranceive(LCD_digitToSeg(digit) | LCD_pointToSeg());
		else
			spi_tranceive(LCD_digitToSeg(digit));

		counter += 1;
	}

	while(counter<=precision)
	{
		if(counter==precision)
			spi_tranceive(LCD_digitToSeg(0) | LCD_pointToSeg());
		else
			spi_tranceive(LCD_digitToSeg(0));

		counter += 1;
	}

	while(counter < 4)
	{
		spi_tranceive(0);
		counter += 1;
	}
	
	CS1_HIGH;
}

void LCD_clear()
{
	CS1_LOW;
	for (uint8_t i=0; i<4; i++)
		spi_tranceive(0);
	CS1_HIGH;
}

void LCD_printLCD1(uint8_t digit, uint8_t enableDot)
{
	if(digit > 9)
		return;

	CS1_LOW;
	
	if(enableDot == 1)
		spi_tranceive(LCD_digitToSeg(digit) | LCD_pointToSeg());
	else
		spi_tranceive(LCD_digitToSeg(digit));
		
	CS1_HIGH;
}

void LCD_printLCD2(uint8_t digit, uint8_t enableDot)
{
	if(digit > 9)
		return;

	CS2_LOW;
	
	if(enableDot == 1)
		spi_tranceive(LCD_digitToSeg(digit) | LCD_pointToSeg());
	else
		spi_tranceive(LCD_digitToSeg(digit));

	CS2_HIGH;
}

void LCD_printLCD3(uint8_t digit, uint8_t enableDot)
{
	if(digit > 9)
		return;

	CS3_LOW;																	

	if(enableDot == 1)
		spi_tranceive(LCD_digitToSeg(digit) | LCD_pointToSeg());
	else
		spi_tranceive(LCD_digitToSeg(digit));

	CS3_HIGH;
}

void LCD_printLCD4(uint8_t digit, uint8_t enableDot)
{
	if(digit > 9)
		return;
	
	CS4_LOW;
	
	if(enableDot == 1)
		spi_tranceive(LCD_digitToSeg(digit) | LCD_pointToSeg());
	else
		spi_tranceive(LCD_digitToSeg(digit));

	CS4_HIGH;
}


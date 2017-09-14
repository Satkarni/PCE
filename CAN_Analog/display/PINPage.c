/*
 * PINPage.c
 *
 *  Created on: 25-Aug-2016
 *      Author: piyush
 */


#include "PINPage.h"

uint8_t pinPage_reset()
{
	for(uint8_t i=0; i<4; i++)
		pin_spinBox[i] = 0;
	pinPage_HEAD = 0;
	return pinPage_HEAD;
}

void pinPage_drawStaticPage()
{
	Enter_Critical_Section
	ks0108ClearScreen();
	char digit[2];
	ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
	ks0108Puts_mod_align_center("Enter PIN",0,127,1,100);
	ks0108InvertRect(0,0,127,10);

	for(uint8_t i=0;i<4;i++)
	{
		ks0108DrawLine((i*32)+8, 20, (i*32)+24, 20, BLACK);
		ks0108DrawLine((i*32)+8, 40, (i*32)+24, 40, BLACK);
		ks0108DrawLine((i*32)+8, 20, (i*32)+8, 40, BLACK);
		ks0108DrawLine((i*32)+24, 20, (i*32)+24, 40, BLACK);
	}

	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	for(uint8_t i =0; i<4; i++)
	{
		itoa(pin_spinBox[i],digit,10);
		ks0108Puts_mod_align_center(digit,(i*32)+9,(i*32)+23,24,14);
	}
	pinPage_drawCursor();
	Leave_Critical_Section
}

void pinPage_spinBoxIncrement()
{
	char digit[2];
	pin_spinBox[pinPage_HEAD] = (pin_spinBox[pinPage_HEAD] >= 9)	?	0	:	pin_spinBox[pinPage_HEAD] + 1;
	itoa(pin_spinBox[pinPage_HEAD],digit,10);
	ks0108Puts_mod_align_center(digit,(pinPage_HEAD*32)+9,(pinPage_HEAD*32)+23,24,14);
}

void pinPage_spinBoxDecrement()
{
	char digit[2];
	pin_spinBox[pinPage_HEAD] = (pin_spinBox[pinPage_HEAD] <= 0)	?	9	:	pin_spinBox[pinPage_HEAD] - 1;
	itoa(pin_spinBox[pinPage_HEAD],digit,10);
	ks0108Puts_mod_align_center(digit,(pinPage_HEAD*32)+9,(pinPage_HEAD*32)+23,24,14);
}

uint8_t pinPage_next()
{
	pinPage_HEAD = (pinPage_HEAD >= 3)		?	0	:	pinPage_HEAD+1;
	pinPage_drawCursor();
	return pinPage_HEAD;
}

uint8_t pinPage_prev()
{
	pinPage_HEAD = (pinPage_HEAD <= 0)		?	3	:	pinPage_HEAD-1;
	pinPage_drawCursor();
	return pinPage_HEAD;
}

void pinPage_drawCursor()
{
	ks0108DrawLine(0,19,127,19,WHITE);
	ks0108DrawLine(0,41,127,41,WHITE);
	ks0108DrawLine(0,18,127,18,WHITE);
	ks0108DrawLine(0,42,127,42,WHITE);

	for(uint8_t i=0; i<4; i++)
	{
		ks0108DrawLine((i*32)+7, 20, (i*32)+7, 40, WHITE);
		ks0108DrawLine((i*32)+25, 20, (i*32)+25, 40, WHITE);
		ks0108DrawLine((i*32)+6, 19, (i*32)+6, 41, WHITE);
		ks0108DrawLine((i*32)+26, 19, (i*32)+26, 41, WHITE);
	}

	ks0108DrawLine((pinPage_HEAD*32)+7, 19, (pinPage_HEAD*32)+25,19,BLACK);
	ks0108DrawLine((pinPage_HEAD*32)+7, 41, (pinPage_HEAD*32)+25,41,BLACK);
	ks0108DrawLine((pinPage_HEAD*32)+7, 20, (pinPage_HEAD*32)+7, 41, BLACK);
	ks0108DrawLine((pinPage_HEAD*32)+25, 20, (pinPage_HEAD*32)+25, 41, BLACK);

	ks0108DrawLine((pinPage_HEAD*32)+6, 18, (pinPage_HEAD*32)+26,18,BLACK);
	ks0108DrawLine((pinPage_HEAD*32)+6, 42, (pinPage_HEAD*32)+26,42,BLACK);
	ks0108DrawLine((pinPage_HEAD*32)+6, 19, (pinPage_HEAD*32)+6, 41, BLACK);
	ks0108DrawLine((pinPage_HEAD*32)+26, 19, (pinPage_HEAD*32)+26, 41, BLACK);
}

uint8_t pinPage_checkPin()
{
	if
	(		pin_spinBox[0] == PIN_DIGIT1	&&
			pin_spinBox[1] == PIN_DIGIT2	&&
			pin_spinBox[2] == PIN_DIGIT3	&&
			pin_spinBox[3] == PIN_DIGIT4)
	{
		_delay_ms(5);								//removing the delay causes controller to crash (I don't know why)
		return 1;
	}
	else
	{
		return 0;
	}
}

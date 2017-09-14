/*
 * ConfigPage.c
 *
 *  Created on: 11-Aug-2016
 *      Author: piyush
 */

#include "ConfigPage.h"

uint8_t configPage_resetHead()
{
	if(configpage_confirmation == 2)
		configpage_head = 0;
	else
		configpage_head = CONFIG_CONFIRM;
	configparam_head = 0;
	return configpage_head;
}

void configPage_setPageTitle (uint8_t page_no, const char *label)
{
	if(page_no > MAX_CONFIG || page_no < MIN_CONFIG)
		return;

	config_page[page_no].title = label;
}

void configPage_setPageLabel1(uint8_t page_no, const char *label)
{
	if(page_no > MAX_CONFIG || page_no < MIN_CONFIG)
		return;

	config_page[page_no].label1 = label;
}

void configPage_setPageLabel2(uint8_t page_no, const char *label)
{
	if(page_no > MAX_CONFIG || page_no < MIN_CONFIG)
		return;

	config_page[page_no].label2 = label;
}

void configPage_setSensor1(uint8_t page_no, const char *label)
{
	if(page_no > MAX_CONFIG || page_no < MIN_CONFIG)
		return;

	config_page[page_no].sensor1 = label;
}

void configPage_setSensor2(uint8_t page_no,const char *label)
{
	if(page_no > MAX_CONFIG || page_no < MIN_CONFIG)
		return;

	config_page[page_no].sensor2 = label;
}

uint8_t configPage_nextPage()
{
	if(configpage_confirmation == 0)
		return CONFIG_CONFIRM;
	if(configparam_head >= MAX_PARAM)
	{
		configpage_head = (configpage_head >= MAX_CONFIG) ? MIN_CONFIG : configpage_head+1;
		configparam_head = 0;
	}
	else
	{
		configparam_head += 1;
	}
		configPage_drawCursor();
	return configpage_head;
}

uint8_t configPage_prevPage()
{
	if(configpage_confirmation == 0)
		return CONFIG_CONFIRM;
	if(configparam_head <= MIN_PARAM)
	{
		configpage_head = (configpage_head <= MIN_CONFIG) ? MAX_CONFIG : configpage_head-1;
		configparam_head = MAX_PARAM;
	}
	else
	{
		configparam_head -= 1;
	}
		configPage_drawCursor();
	return configpage_head;
}

void configPage_incrementValue()
{
	switch(configparam_head)
	{
	case 0:
		if(config_page[configpage_head].value1 <= config_page[configpage_head].max1)
		{
			config_page[configpage_head].value1 += config_page[configpage_head].offset1;
		}
		configPage_drawDynamicPage(1);
		break;

	case 1:

		if(config_page[configpage_head].value2 <= config_page[configpage_head].max2)
		{
			config_page[configpage_head].value2 += config_page[configpage_head].offset2;
		}
		configPage_drawDynamicPage(2);
		break;
	}
}


void configPage_decrementValue()
{
	switch(configparam_head)
	{
	case 0:
		if(config_page[configpage_head].value1 >= config_page[configpage_head].min1)
		{
			config_page[configpage_head].value1 -= config_page[configpage_head].offset1;
		}
		configPage_drawDynamicPage(1);
		break;

	case 1:

		if(config_page[configpage_head].value2 >= config_page[configpage_head].min2)
		{
			config_page[configpage_head].value2 -= config_page[configpage_head].offset2;
		}
		configPage_drawDynamicPage(2);
		break;
	}
}
void configPage_drawStaticPage()
{
	Enter_Critical_Section
	ks0108ClearScreen();
	if(configpage_confirmation == 0)
		configPage_drawConfirmationPage();
	else if(configpage_confirmation == 2)
	{
		ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
		ks0108DrawRect(7,20,52,20,BLACK);
		ks0108DrawRect(69,20,52,20,BLACK);

		ks0108Puts_mod_align_center(config_page[configpage_head].title,0,127,1,100);
		ks0108Puts_mod_align_center(config_page[configpage_head].label1,7,52,45,45);
		ks0108Puts_mod_align_center(config_page[configpage_head].label2,69,121,45,45);
		configPage_drawCursor();
	}
	Leave_Critical_Section
}

void configPage_drawDynamicPage(uint8_t position)
{
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	char print[16];

	switch(position)
	{
	case 0:
		readingConverter_getRealValue(config_page[configpage_head].sensor1,config_page[configpage_head].value1,print);
		ks0108Puts_mod_align_center(print,8,58,24,50);
		readingConverter_getRealValue(config_page[configpage_head].sensor2,config_page[configpage_head].value2,print);
		ks0108Puts_mod_align_center(print,70,120,24,50);
		break;

	case 1:
		readingConverter_getRealValue(config_page[configpage_head].sensor1,config_page[configpage_head].value1,print);
		ks0108Puts_mod_align_center(print,8,58,24,50);
		break;

	case 2:
		readingConverter_getRealValue(config_page[configpage_head].sensor2,config_page[configpage_head].value2,print);
		ks0108Puts_mod_align_center(print,70,120,24,50);
		break;
	}
}

void configPage_drawCursor()
{
	ks0108DrawRect(6,19,54,22,WHITE);
	ks0108DrawRect(5,18,56,24,WHITE);

	ks0108DrawRect(68,19,54,22,WHITE);
	ks0108DrawRect(67,18,56,24,WHITE);

	switch(configparam_head)
	{
	case 0:
		ks0108DrawRect(6,19,54,22,BLACK);
		ks0108DrawRect(5,18,56,24,BLACK);
		break;

	case 1:
		ks0108DrawRect(68,19,54,22,BLACK);
		ks0108DrawRect(67,18,56,24,BLACK);
		break;
	}

}

void configPage_drawConfirmationPage()
{
//	Enter_Critical_Section
	uint8_t i,j;
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center("Press Enter to",0,127,18,100);
	ks0108Puts_mod_align_center("Change Configuration",0,127,30,100);
	for(i = 10, j=117; i <= 117; i++, j--)
	{
		ks0108SetDot(i,10,BLACK);
		ks0108SetDot(j,53,BLACK);
		_delay_ms(3);
	}
	for(i = 10, j = 53; i<= 53; i++, j--)
	{
		ks0108SetDot(117,i,BLACK);
		ks0108SetDot(10,j,BLACK);
		_delay_ms(2);
	}
//	Leave_Critical_Section
}

void configPage_updateEEPROM()
{
	switch(configparam_head)
	{
	case 0:
//		Enter_Critical_Section
//		setAlarmThresholdHigh(config_page[configpage_head].sensor1,config_page[configpage_head].value1);
//		setAlarmThresholdLow(config_page[configpage_head].sensor1,config_page[configpage_head].value1);
//		eeprom_update_dword(config_page[configpage_head].addr1,config_page[configpage_head].value1);
//		Leave_Critical_Section
		break;

	case 1:
//		Enter_Critical_Section
//		setAlarmTimer(config_page[configpage_head].sensor2,config_page[configpage_head].value2);
//		clearTimerQueue();
//		initialiseTimerQueue();
//		eeprom_update_word(config_page[configpage_head].addr2,config_page[configpage_head].value2);
//		Leave_Critical_Section
		break;
	}
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108ClearScreen();
	ks0108Puts_mod_align_center("Values Saved",0,127,18,100);
	ks0108Puts_mod_align_center("Please Restart Controller",0,127,30,100);
	_delay_ms(800);
	configPage_drawStaticPage();
	configPage_drawDynamicPage(0);
}

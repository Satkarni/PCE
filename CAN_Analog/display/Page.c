/*
 * Page.c
 *
 * Created: 8/2/2016 11:56:03 AM
 *  Author: Piyush
 */ 

#include "Page.h"

uint8_t page_resetHead()
{
	page_head = MIN_PAGE;
	page_updateAllValues();
	return page_head;
}

uint8_t page_nextPage()
{
	page_head = (page_head >= MAX_PAGE) ? MIN_PAGE : page_head + 1;
	return page_head;
}

uint8_t page_prevPage()
{
	page_head = (page_head <= MIN_PAGE) ? MAX_PAGE : page_head - 1;
	return page_head;
}

void page_setLabel1(uint8_t page_no, const char* label, const char* unit)
{
	if(page_no > MAX_PAGE || page_no < MIN_PAGE)
		return;
	
	page[page_no].label1 = label;
	page[page_no].unit1 = unit;
}

void page_setLabel2(uint8_t page_no, const char* label, const char* unit)
{
	if(page_no > MAX_PAGE || page_no < MIN_PAGE)
		return;
		
	page[page_no].label2 = label;
	page[page_no].unit2 = unit;
}

void page_setSensor1(uint8_t page_no, const char *name)
{
	if(page_no > MAX_PAGE || page_no < MIN_PAGE)
		return;

	page[page_no].sensor1 = name;
}

void page_setSensor2(uint8_t page_no, const char *name)
{
	if(page_no > MAX_PAGE || page_no < MIN_PAGE)
		return;

	page[page_no].sensor2 = name;
}

void page_updateAllValues()
{
	Enter_Critical_Section
	for(uint8_t i = MIN_PAGE; i <= MAX_PAGE; i++)
	{
		readingConverter_getRealValue(page[i].sensor1,getSensorValue(page[i].sensor1),page[i].value1);
		readingConverter_getRealValue(page[i].sensor2,getSensorValue(page[i].sensor2),page[i].value2);
	}
	Leave_Critical_Section
}

void page_updateValue(const char *sensorName)
{
	Enter_Critical_Section
	for(uint8_t i = MIN_PAGE; i <= MAX_PAGE; i++)
	{
		if(strcmp(sensorName,page[i].sensor1) == 0)
		{
			readingConverter_getRealValue(page[i].sensor1,getSensorValue(page[i].sensor1),page[i].value1);
			if(i == page_head)
				setValueChangeEvent(1);
			return;
		}
		if(strcmp(sensorName,page[i].sensor2) == 0)
		{
			readingConverter_getRealValue(page[i].sensor2,getSensorValue(page[i].sensor2),page[i].value2);
			if(i == page_head)
				setValueChangeEvent(2);
			return;
		}
	}
	Leave_Critical_Section
}

void page_drawStaticPage()
{
	Enter_Critical_Section

	ks0108ClearScreen();
	ks0108DrawVertLine(63,0,40,BLACK);
	ks0108DrawHoriLine(0,40,127,BLACK);
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center(page[page_head].label1,0,63,0,50);
	ks0108Puts_mod_align_center(page[page_head].label2,64,127,0,50);
	ks0108Puts_mod_align_center(page[page_head].unit1,0,63,30,30);
	ks0108Puts_mod_align_center(page[page_head].unit2,64,127,30,30);

	Leave_Critical_Section
}

void page_drawDynamicPage(uint8_t parameter_position)
{
	Enter_Critical_Section
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	switch (parameter_position)
	{
	case 1:
		ks0108Puts_mod_align_center(page[page_head].value1,0,63,15,60);
		break;

	case 2:
		ks0108Puts_mod_align_center(page[page_head].value2,64,127,15,60);
		break;

	default:
		ks0108Puts_mod_align_center(page[page_head].value1,0,63,15,60);
		ks0108Puts_mod_align_center(page[page_head].value2,64,127,15,60);
		page_drawStatus();
		page_drawSafety();
		break;
	}
	Leave_Critical_Section
}

void page_drawSafety()
{
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	switch(page_current_safety)
	{
	case STATUS_SAFETY_DISABLED:
			ks0108Puts_mod_align_center("Safety Disabled",0,127,42,127);
			break;

	case STATUS_SAFETY_ENABLED:
			ks0108Puts_mod_align_center("Safety Enabled",0,127,42,127);
			break;
	}
}

void page_drawStatus()
{
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	switch (page_current_status)
	{
	case STATUS_OK:
		ks0108FillRect(0,53,127,10,WHITE);
		ks0108Puts_mod("Status: Panel Online",0,53,100);
		break;

	case STATUS_ALARM:
		ks0108FillRect(0,53,127,10,WHITE);
		ks0108Puts_mod("Status: Alarm(s) Active",0,53,100);
		break;

	case STATUS_TRIP:
		ks0108FillRect(0,53,127,10,WHITE);
		ks0108Puts_mod("Status: Sensor Trip",0,53,100);
		break;
	}
}

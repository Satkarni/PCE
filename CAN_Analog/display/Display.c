/*
 * Display.c
 *
 *  Created on: 12-Aug-2016
 *      Author: piyush
 */


#include "Display.h"

void display_initPages()
{
	page_setLabel1(0, "Engine Speed", "RPM");
	page_setSensor1(0, "RPM");
	page_setLabel2(0, "Oil Pressure", "bar");
	page_setSensor2(0, "LOP");

	page_setLabel1(1, "Oil Tmp", "deg C");
	page_setSensor1(1, "LOT");
	page_setLabel2(1, "Water Tmp", "deg C");
	page_setSensor2(1, "ECT");

	page_setLabel1(2, "Gear Oil Tmp", "deg C");
	page_setSensor1(2, "GrBxTmp");
	page_setLabel2(2, "Gear Oil Pr", "bar");
	page_setSensor2(2,"GrBxPr");

	page_setLabel1(3, "Exhaust Temp", "deg C");
	page_setSensor1(3,"ExTmp");
	page_setLabel2(3, "Running", "Hours");
	page_setSensor2(3,"Hours");

	page_setLabel1(4,"Battery", "Voltage");
	page_setSensor1(4, "BattVGE");
	page_setLabel2(4,"Spare", "");
	page_setSensor2(4, "");

//	configPage_setPageTitle(0,"Overspeed");
//	configPage_setSensor1(0,"RPM");
//	configPage_setPageLabel1(0,"Alarm Level");
//	config_page[0].offset1 = 10*8;
//	config_page[0].min1 = 2000*8;
//	config_page[0].max1 = 2800*8;
//	config_page[0].value1 = getAlarmThresholdHigh("RPM");
//	config_page[0].addr1 = EEP_OVS_ALARM;
//	configPage_setSensor2(0,"RPM");
//	configPage_setPageLabel2(0, "Trip Level");
//	config_page[0].offset2 = 10*8;
//	config_page[0].min2 = 2000*8;
//	config_page[0].max2 = 2800*8;
//	config_page[0].value2 = getTripThresholdHigh("RPM");
//	config_page[0].addr2 = EEP_OVS_TRIP;
//
//	configPage_setPageTitle(1,"Overspeed");
//	configPage_setSensor1(1,"Linear");
//	configPage_setPageLabel1(1,"Alarm Delay");
//	config_page[1].offset1 = 1;
//	config_page[1].min1 = 0;
//	config_page[1].max1 = 10;
//	config_page[1].value1 = getAlarmTimer("RPM");
//	config_page[1].addr1 = EEP_OVS_ALARM_DELAY;
//	configPage_setSensor2(1,"Linear");
//	configPage_setPageLabel2(1, "Trip Delay");
//	config_page[1].offset2 = 1;
//	config_page[1].min2 = 0;
//	config_page[1].max2 = 10;
//	config_page[1].value2 = getTripTimer("RPM");
//	config_page[1].addr2 = EEP_OVS_TRIP_DELAY;
//
//	configPage_setPageTitle(2,"Lube Oil Pressure");
//	configPage_setSensor1(2,"LOP");
//	configPage_setPageLabel1(2,"Alarm Level");
//	config_page[2].offset1 = 0.2*25;
//	config_page[2].max1 = 1.5*25;
//	config_page[2].min1 = 0.5*25;
//	config_page[2].value1 = getAlarmThresholdLow("LOP");
//	config_page[2].addr1 = EEP_LOP_ALARM;
//	configPage_setSensor2(2,"LOP");
//	configPage_setPageLabel2(2, "Trip Level");
//	config_page[2].offset2 = 0.2*.25;
//	config_page[2].max2 = 1.5*25;
//	config_page[2].min2 = 0.5*25;
//	config_page[2].value2 = getTripThresholdLow("LOP");
//	config_page[2].addr2 = EEP_LOP_TRIP;
//
//	configPage_setPageTitle(3,"Lube Oil Pressure");
//	configPage_setSensor1(3,"Linear");
//	configPage_setPageLabel1(3,"Alarm Delay");
//	config_page[3].offset1 = 1;
//	config_page[3].max1 = 10;
//	config_page[3].min1 = 0;
//	config_page[3].value1 = getAlarmTimer("LOP");
//	config_page[3].addr1 = EEP_LOP_ALARM_DELAY;
//	configPage_setSensor2(3,"Linear");
//	configPage_setPageLabel2(3, "Trip Delay");
//	config_page[3].offset2 = 1;
//	config_page[3].max2 = 10;
//	config_page[3].min2 = 0;
//	config_page[3].value2 = getTripTimer("LOP");
//	config_page[3].addr2 = EEP_LOP_TRIP_DELAY;
//
//	configPage_setPageTitle(4,"Lube Oil Temperature");
//	configPage_setSensor1(4,"LOT");
//	configPage_setPageLabel1(4,"Alarm Level");
//	config_page[4].offset1 = 1*32;
//	config_page[4].min1 = 90*32;
//	config_page[4].max1 = 110*32;
//	config_page[4].value1 = getAlarmThresholdHigh("LOT");
//	config_page[4].addr1 = EEP_LOT_ALARM;
//	configPage_setSensor2(4,"LOT");
//	configPage_setPageLabel2(4,"Trip Level");
//	config_page[4].offset2 = 1*32;
//	config_page[4].min2 = 90*32;
//	config_page[4].max2 = 110*32;
//	config_page[4].value2 = getTripThresholdHigh("LOT");
//	config_page[4].addr2 = EEP_LOT_TRIP;
//
//	configPage_setPageTitle(5,"Lube Oil Temperature");
//	configPage_setSensor1(5,"Linear");
//	configPage_setPageLabel1(5,"Alarm Delay");
//	config_page[5].offset1 = 1;
//	config_page[5].min1 = 0;
//	config_page[5].max1 = 10;
//	config_page[5].value1 = getAlarmTimer("LOT");
//	config_page[5].addr1 = EEP_LOT_ALARM_DELAY;
//	configPage_setSensor2(5,"Linear");
//	configPage_setPageLabel2(5,"Trip Delay");
//	config_page[5].offset2 = 1;
//	config_page[5].min2 = 0;
//	config_page[5].max2 = 10;
//	config_page[5].value2 = getTripTimer("LOT");
//	config_page[5].addr2 = EEP_LOT_TRIP_DELAY;
//
//	configPage_setPageTitle(6,"Coolant Temperature");
//	configPage_setSensor1(6,"ECT");
//	configPage_setPageLabel1(6,"Alarm Level");
//	config_page[6].offset1 = 1;
//	config_page[6].max1 = 110+40;
//	config_page[6].min1 = 90+40;
//	config_page[6].value1 = getAlarmThresholdHigh("ECT");
//	config_page[6].addr1 = (uint32_t*)EEP_ECT_ALARM;
//	configPage_setSensor2(6,"ECT");
//	configPage_setPageLabel2(6,"Trip Level");
//	config_page[6].offset2 = 1;
//	config_page[6].max2 = 110 + 40;
//	config_page[6].min2 = 90 + 40;
//	config_page[6].value2 = getTripThresholdHigh("ECT");
//	config_page[6].addr2 = EEP_ECT_TRIP;
//
//	configPage_setPageTitle(7,"Coolant Temperature");
//	configPage_setSensor1(7,"Linear");
//	configPage_setPageLabel1(7,"Alarm Delay");
//	config_page[7].offset1 = 1;
//	config_page[7].max1 = 10;
//	config_page[7].min1 = 0;
//	config_page[7].value1 = getAlarmTimer("ECT");
//	config_page[7].addr1 = EEP_ECT_ALARM_DELAY;
//	configPage_setSensor2(7,"Linear");
//	configPage_setPageLabel2(7,"Trip Delay");
//	config_page[7].offset2 = 1;
//	config_page[7].max2 = 10;
//	config_page[7].min2 = 0;
//	config_page[7].value2 = getTripTimer("ECT");
//	config_page[7].addr2 = EEP_ECT_TRIP_DELAY;
}

void display_initBrightness()
{
	DDRB |= (1<<PB7);	// PWM pin is output
	TCCR0A |= ((1<<WGM01)|(1<<WGM00)); // Fast PWM mode
	TCCR0A |= ((1<<COM0A1)|(0<<COM0A0)); // Non inverted PWM
	TCCR0A |= ((1<<CS02)|(0<<CS01)|(0<<CS00));	// 1/256 prescaler, PWM freq ~ 60 Khz
}

void display_setBrightness(uint8_t brightness_arg)
{
	OCR0A = brightness_arg;
}

void display_init()
{
	display_initBrightness();
	ks0108Init(0);
	ks0108ClearScreen();

	brightness = 255;
	display_setBrightness(255);
	dtc_received = 0;

	_delay_ms(2);

	ks0108Init(1);
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108LoadBitmap(pce_logo);

	_delay_ms(2000);
	ks0108ClearScreen();
	ks0108Init(0);

	alarmPage_initAlarms();
	display_initPages();
	display_switchHead(MAIN);

}

void display_decrementBrightness()
{
	brightness = (brightness == 0) ? 0	: (brightness - 51);
	display_setBrightness(brightness);
}

void display_incrementBrightness()
{
	brightness = (brightness == 0xFF) ? 0xFF : (brightness + 51);
	display_setBrightness(brightness);
}

void display_switchHead(enum head_type newtype)
{
	display_HEAD.type = newtype;

	switch(display_HEAD.type)
	{
	case MAIN:
		display_HEAD.value = page_resetHead();
		display_drawStaticPage();
		display_drawDynamicPage(0);
		break;

	case CONFIG:
		display_HEAD.value = configPage_resetHead();
		display_drawStaticPage();
		if(configpage_confirmation == 2)
			display_drawDynamicPage(0);
		break;

	case ALARM:
		display_HEAD.value = alarmPage_resetHead();
		display_drawStaticPage();
		display_drawDynamicPage(0);
		break;

	case PIN:
		display_HEAD.value = pinPage_reset();
		display_drawStaticPage();
		display_drawDynamicPage(0);
		break;
	}
}

void display_nextPage()
{
	uint8_t tmp;
	switch(display_HEAD.type)
	{
	case MAIN:
		display_HEAD.value = page_nextPage();
		display_drawStaticPage();
		break;

	case CONFIG:
		tmp = configPage_nextPage();
		if (tmp != display_HEAD.value)
		{
			display_HEAD.value = tmp;
			display_drawStaticPage();
		}
		break;

	case ALARM:
		display_HEAD.value = alarmPage_nextPage();
		display_drawStaticPage();
		break;

	case PIN:
		display_HEAD.value = pinPage_next();
		break;
	}
	display_drawDynamicPage(0);
}

void display_prevPage()
{
	uint8_t tmp;
	switch(display_HEAD.type)
	{
	case MAIN:
		display_HEAD.value = page_prevPage();
		display_drawStaticPage();
		break;

	case CONFIG:
		tmp = configPage_prevPage();
		if (tmp != display_HEAD.value)
		{
			display_HEAD.value = tmp;
			display_drawStaticPage();
		}
		break;

	case ALARM:
		display_HEAD.value = alarmPage_prevPage();
		display_drawStaticPage();
		break;

	case PIN:
		display_HEAD.value = pinPage_prev();
		break;
	}
	display_drawDynamicPage(0);
}

void display_drawStaticPage()
{
	switch(display_HEAD.type)
	{
	case MAIN:
		page_drawStaticPage();
		break;

	case CONFIG:
		configPage_drawStaticPage();
		break;

	case ALARM:
		alarmPage_drawStaticPage();
		break;

	case PIN:
		pinPage_drawStaticPage();
	}
}

void display_drawDynamicPage(uint8_t parameter_position)
{
	switch(display_HEAD.type)
	{
	case MAIN:
		page_drawDynamicPage(parameter_position);
		break;

	case CONFIG:
		configPage_drawDynamicPage(parameter_position);
		break;

	case ALARM:
		alarmPage_drawDynamicPage();
		break;

	case PIN:
		break;
	}
}

void display_PINIncorrect()
{
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center("Incorrect PIN",0,127,15,100);
}

void display_PINCorrect()
{
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center("PIN Correct",0,127,15,100);
}

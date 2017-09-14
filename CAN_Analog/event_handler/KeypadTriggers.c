/*
 * KeypadTriggers.c
 *
 *  Created on: 13-Aug-2016
 *      Author: piyush
 */


#include "../event_handler/KeypadTriggers.h"

void triggerRightArrow()
{
	display_nextPage();
}

void triggerLeftArrow()
{
	display_prevPage();
}

void triggerEnter()
{
	switch(display_HEAD.type)
	{
	case MAIN:
		display_switchHead(ALARM);
		break;

	case ALARM:
		display_switchHead(MAIN);
		break;

	default:
		break;
	}
//	if(display_HEAD.type == CONFIG)
//	{
//		if(configpage_confirmation == 0)
//		{
//			configpage_confirmation = 1;
//			display_switchHead(PIN);
//		}
//		if(configpage_confirmation == 2)
//		{
//			configPage_updateEEPROM();
//		}
//	}
//	else if(display_HEAD.type == PIN)
//	{
//		if(pinPage_checkPin() == 1)
//		{
//			display_PINCorrect();
//			_delay_ms(800);
//			configpage_confirmation = 2;
//			display_switchHead(CONFIG);
//		}
//		else
//		{
//			display_PINIncorrect();
//			_delay_ms(800);
//			display_switchHead(MAIN);
//		}
//	}
}

void triggerUpArrow()
{
//	if(display_HEAD.type == PIN)
//	{
//		pinPage_spinBoxIncrement();
//	}
//	else if(display_HEAD.type == CONFIG && configpage_confirmation == 2)
//	{
//		configPage_incrementValue();
//	}
//	else
//	{
//		switch(display_HEAD.type)
//		{
//		case MAIN:
//			configpage_confirmation = 0;
//			display_switchHead(CONFIG);
//			break;
//
//		case ALARM:
//			display_switchHead(MAIN);
//			break;
//
//		default:
//			break;
//		}
//	}
}

void triggerDownArrow()
{
//	if(display_HEAD.type == PIN)
//	{
//		pinPage_spinBoxDecrement();
//	}
//	else if(display_HEAD.type == CONFIG && configpage_confirmation == 2)
//	{
//		configPage_decrementValue();
//	}
//	else
//	{
//		switch(display_HEAD.type)
//		{
//		case MAIN:
//			display_switchHead(ALARM);
//			break;
//
//		case CONFIG:
//			display_switchHead(MAIN);
//			break;
//
//		default:
//			break;
//		}
//	}
}

void triggerBackReset()
{
		uint8_t i, current_alarms = 0 ,current_trips = 0;
		Enter_Critical_Section
		resetSensors();
		alarms[RPM_ALARM].enabled = (isAlarmEnabled("RPM") == 0)	?	0:1;
		alarms[LOP_ALARM ].enabled = (isAlarmEnabled("LOP") == 0)	?	0:1;
		alarms[LOT_ALARM ].enabled = (isAlarmEnabled("LOT") == 0)	?	0:1;
		alarms[ECT_ALARM ].enabled = (isAlarmEnabled("ECT") == 0)	?	0:1;
		alarms[GBP_ALARM].enabled = (isAlarmEnabled("GrBxPr") == 0)	?	0:1;
		alarms[GBT_ALARM].enabled = (isAlarmEnabled("GrBxTmp") == 0)	?	0:1;
		alarms[EXT_ALARM ].enabled = (isAlarmEnabled("ExTmp") == 0)	?	0:1;

		trips[RPM_TRIP].enabled = (isTripEnabled("RPM") == 0)	?	0:1;
		trips[LOP_TRIP].enabled = (isTripEnabled("LOP") == 0)	?	0:1;
		trips[LOT_TRIP].enabled = (isTripEnabled("LOT") == 0)	?	0:1;
		trips[ECT_TRIP].enabled = (isTripEnabled("ECT") == 0)	?	0:1;
		trips[GBP_TRIP].enabled = (isTripEnabled("GrBxPr") == 0)	?	0:1;
		trips[GBT_TRIP].enabled = (isTripEnabled("GrBxTmp") == 0)	?	0:1;
		trips[EXT_TRIP].enabled = (isTripEnabled("ExTmp") == 0)	?	0:1;

		for (i=0; i < ALARM_LIST_LENGTH; i++)
		{
			if(alarms[i].enabled == 1)
				current_alarms += 1;

			if(trips[i].enabled == 1)
				current_trips += 1;
		}
		Leave_Critical_Section
		if(current_alarms == 0)
		{
			ioHandler_disableRelayBuzzer();
		}
		else
		{
			ioHandler_enableRelayBuzzer();
		}
		if(current_trips == 0)
		{
			ioHandler_disableRelayStop();
		}
		else
		{
			ioHandler_enableRelayStop();
			ioHandler_enableRelayBuzzer();
		}

		if(current_alarms == 0 && current_trips == 0)
		{
			ioHandler_disableFaultLED();
			page_current_status = STATUS_OK;
		}

		alarmPage_generatePages();
		if(display_HEAD.type == ALARM)
			display_drawDynamicPage(0);
		else if(display_HEAD.type == MAIN)
			page_drawStatus();
}

/*
 * EventHandler.c
 *
 *  Created on: 13-Aug-2016
 *      Author: piyush
 */


#include "../event_handler/EventHandler.h"

void selectEvent()
{
	triggerKeypadEvent(scanKeypadEvent());
	triggerAlarmEvent(scanAlarmEvent());
	triggerTripEvent(scanTripEvent());
	triggerValueChangeEvent(value_change_event);
}

void setKeypadEvent(enum keypad_key key)
{
	keypad_events[key] = 1;
}

void setValueChangeEvent(uint8_t parameter_position)
{
	if(parameter_position != 1 || parameter_position != 2)
		value_change_event = parameter_position;
	else
		value_change_event = 0;
}

void setAlarmEvent (enum alarm_types alarm_type)
{
	alarm_events[alarm_type] = 1;
}

void setTripEvent(enum trip_types trip_type)
{
	trip_events[trip_type] = 1;
}

enum keypad_key scanKeypadEvent()
{
	for(uint8_t i = 1; i <= 6; i++)
	{
		if(keypad_events[i] == 1)
		{
			keypad_events[i] = 0;
			return i;
		}
	}
	return 0;
}

enum alarm_types scanAlarmEvent()
{
	for(uint8_t i = 0; i < ALARM_LIST_LENGTH; i++)
	{
		if(alarm_events[i] == 1)
		{
			alarm_events[i] = 0;
			return i;
		}
	}
	return ALARM_LIST_LENGTH;
}

enum trip_types scanTripEvent()
{
	for(uint8_t i = 0; i < TRIP_LIST_LENGTH; i++)
	{
		if(trip_events[i] == 1)
		{
			trip_events[i] = 0;
			return i;
		}
	}
	return TRIP_LIST_LENGTH;

}


void triggerKeypadEvent(enum keypad_key key)
{
	switch(key)
	{
	case RIGHT_ARROW:
		triggerRightArrow();
		break;

	case LEFT_ARROW:
		triggerLeftArrow();
		break;

	case UP_ARROW:
		triggerUpArrow();
		break;

	case DOWN_ARROW:
		triggerDownArrow();
		break;

	case ENTER:
		triggerEnter();
		break;

	case BACK_RESET:
		triggerBackReset();
		break;

	default:
		break;
	}
}

void triggerValueChangeEvent(uint8_t position)
{
	if(position == 0)
		return;

	value_change_event = 0;
	if(display_HEAD.type == MAIN)
		display_drawDynamicPage(position);
}

void triggerAlarmEvent (enum alarm_types alarm_type)
{
	if(alarm_type == ALARM_LIST_LENGTH)
		return;

	alarms[alarm_type].enabled = 1;
	alarmPage_generatePages();
	if(alarms_count > 0)
	{
		ioHandler_enableRelayBuzzer();
		ioHandler_enableFaultLED();
		if(page_current_status == STATUS_OK)
			page_current_status = STATUS_ALARM;
	}
	if(display_HEAD.type == ALARM)
		display_drawDynamicPage(0);
	else if(display_HEAD.type == MAIN)
		page_drawStatus();
}

void triggerTripEvent(enum trip_types trip_type)
{
	if(trip_type == TRIP_LIST_LENGTH)
		return;

	trips[trip_type].enabled = 1;
	alarmPage_generatePages();
	if(trips_count > 0)
	{
		ioHandler_enableRelayStop();
		ioHandler_enableRelayBuzzer();
		ioHandler_enableFaultLED();
		if(page_current_status != STATUS_TRIP)
			page_current_status = STATUS_TRIP;
	}
	if(display_HEAD.type == ALARM)
		display_drawDynamicPage(0);
	else if(display_HEAD.type == MAIN)
		page_drawStatus();
}

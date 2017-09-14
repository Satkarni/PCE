/*
 * SensorHandler.c
 *
 *  Created on: 16-Jul-2016
 *      Author: piyush
 */
#include "SensorHandler.h"

uint8_t mute = 0; /**<This variable is 1 if sensors have been muted. 0 if not.
 */

uint8_t checkSensorTrip(char *name)
{
	if(getSensorByName(name) == NULL)
		return 0;
	if((isSensorEnabled(name) == 0) || (isAllowTrip(name) == 0))
		return 0;

	uint32_t threshold;

	switch (isTripAtHigh(name))
	{

	case 0 :
		threshold = getTripThresholdLow(name);

		if(getSensorValue(name) <= threshold)
			return 1;
		else
			return 0;
		break;

	case 1 :
		threshold = getTripThresholdHigh(name);

		if(getSensorValue(name) >= threshold)
			return 1;
		else
			return 0;
		break;
	}
	return 0;

}

uint8_t checkSensorTrip_pos(uint8_t position)
{
	if(getSensorByPosition(position) == NULL)
		return 0;
	if((isSensorEnabled_pos(position) == 0) || (isAllowTrip_pos(position) == 0))
		return 0;

	uint32_t threshold;

	switch (isTripAtHigh_pos(position))
	{

	case 0 :
		threshold = getTripThresholdLow_pos(position);
		if(getSensorValue_pos(position) <= threshold)
			return 1;
		else
			return 0;
		break;

	case 1 :
		threshold = getTripThresholdHigh_pos(position);
		if(getSensorValue_pos(position) >= threshold)
			return 1;
		else
			return 0;
		break;
	}
	return 0;
}

uint8_t checkSensorAlarm(char *name)
{
	if(getSensorByName(name) == NULL)
		return 0;
	if((isSensorEnabled(name) == 0) || (isAllowAlarm(name) == 0))
		return 0;

	uint32_t threshold;

	switch (isTripAtHigh(name))
	{

	case 0 :
		threshold = getAlarmThresholdLow(name);

		if(getSensorValue(name) <= threshold)
			return 1;
		else
			return 0;
		break;

	case 1 :
		threshold = getAlarmThresholdHigh(name);

		if(getSensorValue(name) >= threshold)
			return 1;
		else
			return 0;
		break;
	}
	return 0;
}

uint8_t checkSensorAlarm_pos(uint8_t position)
{
	if(getSensorByPosition(position) == NULL)
		return 0;
	if((isSensorEnabled_pos(position) == 0) || (isAllowAlarm_pos(position) == 0))
		return 0;

	uint32_t threshold;

	switch (isTripAtHigh_pos(position))
	{

	case 0 :
		threshold = getAlarmThresholdLow_pos(position);
		if(getSensorValue_pos(position) <= threshold)
			return 1;
		else
			return 0;
		break;

	case 1 :
		threshold = getAlarmThresholdHigh_pos(position);
		if(getSensorValue_pos(position) >= threshold)
			return 1;
		else
			return 0;
		break;
	}
	return 0;
}

void processSensors()
{
	uint8_t i;
	for(i = 0; getSensorByPosition(i) != NULL; i++)
	{
		if(failureTimeout(getSensorName(i)) == 1)
		{
			enableSensorFail_pos(i, 1);
			mute = 0;
		}
		else
		{
			if(getAlarmTimer(getSensorName(i)) == 0)	
			{
				if(checkSensorAlarm_pos(i))
				{
					enableSensorAlarm_pos(i, 1);
				}
			}
			else if(alarmTimeout(getSensorName(i)) == 1)
			{
				enableSensorAlarm_pos(i, 1);
			}

			if(getTripTimer(getSensorName(i)) == 0)	
			{
				if(checkSensorTrip_pos(i))
				{
					enableSensorTrip_pos(i, 1);
				}
			}
			else if(tripTimeout(getSensorName(i)) == 1)
			{
				enableSensorTrip_pos(i, 1);
			}
		}

	}
}

uint8_t checkFailure(char *name)
{
	if(isSensorEnabled(name) == 0)
		return 0;
	uint32_t value = getSensorValue(name);
	if(value > getSensorMax(name) || value < getSensorMin(name))
		return 1;
	else
		return 0;
}

uint8_t checkFailure_pos(uint8_t position)
{
	if(isSensorEnabled_pos(position) == 0)
		return 0;
	uint32_t value = getSensorValue_pos(position);
	if(value > getSensorMax_pos(position) || value < getSensorMin_pos(position))
		return 1;
	else
		return 0;
}

uint8_t resetSensors()
{
	uint8_t i, active_alarms = 0, active_trips = 0, active_failures = 0, return_value;

	for(i = 0; getSensorByPosition(i) != NULL; i++)
	{
		if(checkSensorAlarm_pos(i) == 0)
			enableSensorAlarm_pos(i, 0);
		else if(checkSensorAlarm_pos(i) == 1)
			active_alarms += 1;

		if(checkSensorTrip_pos(i) == 0)
			enableSensorTrip_pos(i,0);
		else if(checkSensorTrip_pos(i) == 1)
			active_trips += 1;

		if(checkFailure_pos(i) == 0)
			enableSensorFail_pos(i, 0);
		else if(checkFailure_pos(i) == 1)
			active_failures += 1;
	}
	mute = 0;
	return_value = (active_alarms > active_trips) ? active_alarms : active_trips;
	return return_value;
}

uint8_t muteSensors()
{
	uint8_t i, active_alarms = 0;
	for(i = 0; getSensorByPosition(i) != NULL; i++)
	{
		if(isAlarmEnabled_pos(i) == 1)
		{
			active_alarms += 1;
		}
	}
	mute = (active_alarms > 0) ? 1:0;
	return active_alarms;
}

uint8_t isMuteActive()
{
	return mute;
}

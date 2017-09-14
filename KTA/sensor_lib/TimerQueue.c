/*
 * TimerQueue.c
 *
 *  Created on: 16-Jul-2016
 *      Author: piyush
 */

#include "TimerQueue.h"

uint8_t initialiseTimerQueue()
{
	uint8_t i;
	char name[10];
	Timer *z = NULL;

	for(i = 0; getSensorByPosition(i) != NULL; i++)
	{
		if(isSensorEnabled_pos(i) == 1)
		{
			strcpy(name, getSensorName(i));
			if(addTimer(name) == 0)
				return 0;
			z = getTimerByName(name);
			z->alarmMax = getAlarmTimer(name);
			z->tripMax = getTripTimer(name);
			z->failureMax = getFailureTimer(name);

			z->alarmCount = 0;
			z->tripCount = 0;
			z->failureCount = 0;
		}
	}
	return 1;
}

uint8_t clearTimerQueue()
{
	Timer *z = getTimerByPosition(0);
	while(z != NULL)
	{
		removeTimer(z->name);
		z = getTimerByPosition(0);
	}
	return 1;
}

uint8_t recheckTimerQueue()
{
	Timer *z = getTimerByPosition(0);

	if(z == NULL)
		return 0;

	while(z!=NULL)
	{
		if(isSensorEnabled(z->name) == 0)
			removeTimer(z->name);
		z = z->next;
	}
	return 1;
}

uint8_t clockTimerQueue()
{
	Timer *z = getTimerByPosition(0);

	if(z == NULL)
		return 0;

	while(z != NULL)
	{
		if (isTripEnabled(z->name) == 0)
		{
			if(checkSensorTrip(z->name) == 1)
			{
				if(z->tripCount < z->tripMax)
					z->tripCount += 1;
			}

			else if(checkSensorTrip(z->name) == 0)
				z->tripCount = 0;
		}

		if (isAlarmEnabled(z->name) == 0)
		{
			if(checkSensorAlarm(z->name) == 1)
			{
				if(z->alarmCount < z->alarmMax)
					z->alarmCount += 1;
			}

			else if(checkSensorAlarm(z->name) == 0)
				z->alarmCount = 0;
		}

		if (isSensorFail(z->name) == 0)
		{
			if(checkFailure(z->name) == 1)
			{
				if(z->failureCount < z->failureMax)
					z->failureCount += 1;
			}
			else if(checkFailure(z->name) == 0)
				z->failureCount = 0;
		}

		z = z->next;
	}
	return 1;
}

uint8_t alarmTimeout(char *name)
{
	Timer *z = getTimerByName(name);

	if(z == NULL)
		return 0;
		
	if(z->alarmMax == 0)
		return 0;
		
	if (z->alarmCount >= z->alarmMax)
		return 1;
	else
		return 0;
}

uint8_t tripTimeout(char *name)
{
	Timer *z = getTimerByName(name);

	if(z == NULL)
		return 0;
	
	if(z->tripMax)
		return 0;
	
	if(z->tripCount >= z->tripMax)
		return 1;
	else
		return 0;
}

uint8_t failureTimeout(char *name)
{
	Timer *z = getTimerByName(name);

	if(z == NULL)
		return 0;

	if(z->failureCount >= z->failureMax)
		return 1;
	else
		return 0;
}


/*
 * SensorsDAO.c
 *
 *  Created on: 14-Jul-2016
 *      Author: piyush
 */

#include "SensorsDAO.h"

Sensor *sensor_first = NULL;

void showAllSensors() 	//TESTING FUNCTION ONLY!! REMOVE IN FINAL BUILD
{
	Sensor *p = sensor_first;
	printf("\nSTART -> ");
	for(;p != NULL;p = p->next)
		printf("%s -> ",p->name);
	printf("END");
}

uint8_t addSensor(char *name)
{
	if(sensor_first == NULL)
	{
		sensor_first = (Sensor*)malloc(sizeof(Sensor));

		if(sensor_first == NULL)
			return 0;
		strcpy(sensor_first->name,name);
		sensor_first->allowAlarm = 1;
		sensor_first->allowTrip = 1;
		sensor_first->enabled = 1;
		sensor_first->alarmEnabled = 0;
		sensor_first->tripEnabled = 0;
		sensor_first->failureEnabled = 0;
		sensor_first->next = NULL;
	}
	else
	{
		Sensor *p = sensor_first;
		while (p->next != NULL)
			p = p->next;
		p->next = (Sensor*)malloc(sizeof(Sensor));
		if(p->next == NULL)
			return 0;
		p = p->next;
		strcpy(p->name,name);
		p->allowAlarm = 1;
		p->allowTrip = 1;
		p->enabled = 1;
		p->alarmEnabled = 0;
		p->tripEnabled = 0;
		p->failureEnabled = 0;
		p->next = NULL;
	}
	return 1;

}

Sensor* getSensorByName(char *name)
{
	if(sensor_first == NULL)
		return NULL;

	Sensor *p = sensor_first;

	while(p != NULL)
	{
		if(0 == strcmp(p->name,name))
			return p;
		else
			p = p->next;
	}
	return NULL;
}

Sensor* getSensorByPosition(uint8_t position)
{
	if(sensor_first == NULL)
		return NULL;
	Sensor *p = sensor_first;
	for(uint8_t i=0; i < position; i++)
	{
		p = p->next;
		if(p == NULL)
			return NULL;
	}
	return p;
}

uint8_t removeSensor(char *name)
{
	if (sensor_first == NULL)
		return 0;
	Sensor *p = sensor_first, *q = p->next;
	if(strcmp(name,p->name) == 0)
	{
		sensor_first = q;
		free(p);
		return 1;
	}
	while(q != NULL)
	{
		if(strcmp(name,q->name) == 0)
		{
			p->next = q->next;
			free(q);
			return 1;
		}
		p = p->next;
		q = q->next;
	}
	return 0;
}

uint8_t setTripThresholdHigh(char *name, uint32_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->thresholdHigh = value;
	return 1;
}

uint8_t setTripThresholdLow(char *name, uint32_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->thresholdLow = value;
	return 1;
}

uint8_t setAlarmThresholdLow(char *name, uint32_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->alarmThresholdLow = value;
	return 1;
}

uint8_t setAlarmThresholdHigh(char *name, uint32_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->alarmThresholdHigh = value;
	return 1;
}

uint8_t setResetDelay(char *name, uint16_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->resetDelay = value;
	return 1;
}

uint8_t setSensorValue(char *name, uint32_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->value = value;
	return 1;
}

uint8_t enableSensor(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(z->enabled != 1)
		z->enabled = 1;
	return 1;
}

uint8_t disableSensor(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(z->enabled != 0)
		z->enabled = 0;
	return 1;
}

uint8_t setSensorMinMax(char *name, uint32_t min, uint32_t max)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->minValue = min;
	z->maxValue = max;
	return 1;
}

uint8_t enableSensorTrip(char *name, uint8_t enabled)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->tripEnabled = enabled;
	else
		z->tripEnabled = 0;
	return 1;
}

uint8_t enableSensorAlarm(char *name, uint8_t enabled)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->alarmEnabled = enabled;
	else
		z->alarmEnabled = 0;
	return 1;
}

uint8_t enableSensorFail(char *name, uint8_t enabled)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->failureEnabled = enabled;
	else
		z->failureEnabled = 0;
	return 1;
}

uint8_t enableTripAtHigh(char *name, uint8_t enabled)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->tripAtHigh = enabled;
	else
		z->tripAtHigh = 0;
	return 1;
}

uint8_t allowTrip(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(z->allowTrip != 1)
		z->allowTrip = 1;
	return 1;
}

uint8_t revokeTrip(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(z->allowTrip != 0)
		z->allowTrip = 0;
	return 1;
}

uint8_t allowAlarm(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(z->allowAlarm != 1)
		z->allowAlarm = 1;
	return 1;
}

uint8_t revokeAlarm(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	if(z->allowAlarm != 0)
		z->allowAlarm = 0;
	return 1;
}

uint8_t setAlarmTimer(char *name, uint16_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->alarmTimerMax = value;
	return 1;
}

uint8_t setTripTimer(char *name, uint16_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->tripTimerMax = value;
	return 1;
}

uint8_t setFailureTimer(char *name, uint16_t value)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	z->failureTimerMax = value;
	return 1;
}

uint8_t setSensorValue_pos(uint8_t position, uint32_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->value = value;
	return 1;
}

uint8_t enableSensor_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(z->enabled != 1)
		z->enabled = 1;
	return 1;
}

uint8_t disableSensor_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(z->enabled != 0)
		z->enabled = 0;
	return 1;
}

uint8_t setSensorMinMax_pos(uint8_t position, uint32_t min, uint32_t max)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->minValue = min;
	z->maxValue = max;
	return 1;
}

uint8_t enableSensorTrip_pos(uint8_t position, uint8_t enabled)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->tripEnabled = enabled;
	else
		z->tripEnabled = 0;
	return 1;
}

uint8_t enableSensorAlarm_pos(uint8_t position, uint8_t enabled)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->alarmEnabled = enabled;
	else
		z->alarmEnabled = 0;
	return 1;
}

uint8_t enableSensorFail_pos(uint8_t position, uint8_t enabled)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->failureEnabled = enabled;
	else
		z->failureEnabled = 0;
	return 1;
}

uint8_t enableTripAtHigh_pos(uint8_t position, uint8_t enabled)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(enabled == 1 || enabled == 0)
		z->tripAtHigh = enabled;
	else
		z->tripAtHigh = 0;
	return 1;
}

uint8_t allowTrip_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(z->allowTrip != 1)
		z->allowTrip = 1;
	return 1;
}

uint8_t revokeTrip_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(z->allowTrip != 0)
		z->allowTrip = 0;
	return 1;
}

uint8_t allowAlarm_pos (uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(z->allowAlarm != 1)
		z->allowAlarm = 1;
	return 1;
}

uint8_t revokeAlarm_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	if(z->allowAlarm != 0)
		z->allowAlarm = 0;
	return 1;
}

uint8_t setTripThresholdHigh_pos(uint8_t position, uint32_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->thresholdHigh = value;
	return 1;
}

uint8_t setTripThresholdLow_pos(uint8_t position, uint32_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->thresholdLow = value;
	return 1;
}

uint8_t setAlarmThresholdLow_pos(uint8_t position, uint32_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->alarmThresholdLow = value;
	return 1;
}

uint8_t setAlarmThresholdHigh_pos(uint8_t position, uint32_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->alarmThresholdHigh = value;
	return 1;
}

uint8_t setResetDelay_pos(uint8_t position, uint16_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->resetDelay = value;
	return 1;
}

uint8_t setAlarmTimer_pos(uint8_t position, uint16_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->alarmTimerMax = value;
	return 1;
}

uint8_t setTripTimer_pos(uint8_t position, uint16_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->tripTimerMax = value;
	return 1;
}

uint8_t setFailureTimer_pos(uint8_t position, uint16_t value)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	z->failureTimerMax = value;
	return 1;
}

char* getSensorName(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return NULL;
	return z->name;
}

uint32_t getSensorValue(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->value;
}

uint32_t getSensorMin(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->minValue;
}

uint32_t getSensorMax(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->maxValue;
}

uint8_t	isSensorEnabled(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->enabled;
}

uint8_t isTripEnabled(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->tripEnabled;
}

uint8_t isAlarmEnabled(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->alarmEnabled;
}

uint8_t isSensorFail(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->failureEnabled;
}

uint8_t isTripAtHigh(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->tripAtHigh;
}

uint8_t isAllowTrip(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->allowTrip;
}

uint8_t isAllowAlarm(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->allowAlarm;
}

uint32_t getTripThresholdLow(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->thresholdLow;
}

uint32_t getTripThresholdHigh(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->thresholdHigh;
}

uint32_t getAlarmThresholdHigh(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->alarmThresholdHigh;
}

uint32_t getAlarmThresholdLow(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->alarmThresholdLow;
}

uint16_t getResetDelay(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->resetDelay;
}

uint16_t getAlarmTimer(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->alarmTimerMax;
}

uint16_t getTripTimer(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->tripTimerMax;
}

uint16_t getFailureTimer(char *name)
{
	Sensor *z = getSensorByName(name);
	if(z == NULL)
		return 0;
	return z->failureTimerMax;
}

uint16_t getAlarmTimer_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->alarmTimerMax;
}

uint16_t getTripTimer_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->tripTimerMax;
}

uint32_t getSensorValue_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->value;
}

uint32_t getSensorMin_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->minValue;
}

uint32_t getSensorMax_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->maxValue;
}

uint8_t	isSensorEnabled_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->enabled;
}

uint8_t isTripEnabled_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->tripEnabled;
}

uint8_t isAlarmEnabled_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->alarmEnabled;
}

uint8_t isSensorFail_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->failureEnabled;
}

uint8_t isTripAtHigh_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->tripAtHigh;
}

uint8_t isAllowTrip_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->allowTrip;
}

uint8_t isAllowAlarm_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->allowAlarm;
}

uint32_t getTripThresholdLow_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->thresholdLow;
}

uint32_t getTripThresholdHigh_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->thresholdHigh;
}

uint32_t getAlarmThresholdHigh_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->alarmThresholdHigh;
}

uint32_t getAlarmThresholdLow_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->alarmThresholdLow;
}


uint16_t getResetDelay_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->resetDelay;
}

uint16_t getFailureTimer_pos(uint8_t position)
{
	Sensor *z = getSensorByPosition(position);
	if(z == NULL)
		return 0;
	return z->failureTimerMax;
}

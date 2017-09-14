/*
 * AnalogScanner.c
 *
 *  Created on: 11-Mar-2017
 *      Author: piyush
 */


#include "AnalogScanner.h"

void analogScanner_init()
{
	ADC_init();

	analogScanner_initSensors();
}

void analogScanner_initSensors()
{
	double tmp;
	addSensor("GrBxPr");
	allowTrip("GrBxPr");
	allowAlarm("GrBxPr");
	enableTripAtHigh("GrBxPr",0);
	setSensorMinMax("GrBxPr",196,985);

	tmp = ((double)GEARBOX_PR_TRIP_LEVEL * 19.6608) + 196.608;
	tmp = ceil(tmp);
	setTripThresholdLow("GrBxPr",(uint32_t)tmp);

	tmp = ((double)GEARBOX_PR_ALARM_LEVEL * 19.6608) + 196.608;
	tmp = ceil(tmp);
	setAlarmThresholdLow("GrBxPr",(uint32_t)tmp);

	setAlarmTimer("GrBxPr",ALARM_DELAY);
	setTripTimer("GrBxPr",TRIP_DELAY);
	setFailureTimer("GrBxPr",5);

	addSensor("GrBxTmp");
	allowTrip("GrBxTmp");
	allowAlarm("GrBxTmp");
	enableTripAtHigh("GrBxTmp",1);
	setSensorMinMax("GrBxTmp",196,985);

//	tmp = (double)GEARBOX_TMP_TRIP_LEVEL * 2.048;									//*FOR PT100 SENSOR
	tmp = ((double)GEARBOX_TMP_TRIP_LEVEL * 5.2429) + 196.608;				//*FOR 4-20 SENSOR
	tmp = ceil(tmp);
	setTripThresholdHigh("GrBxTmp",(uint32_t)tmp);

//	tmp = (double)GEARBOX_TMP_ALARM_LEVEL * 2.048;								//*FOR PT100 SENSOR
	tmp = ((double)GEARBOX_TMP_ALARM_LEVEL * 5.2429) + 196.608;			//*FOR 4-20 SENSOR
	tmp = ceil(tmp);
	setAlarmThresholdHigh("GrBxTmp",(uint32_t)tmp);

	setAlarmTimer("GrBxTmp",ALARM_DELAY);
	setTripTimer("GrBxTmp",TRIP_DELAY);
	setFailureTimer("GrBxTmp",5);

	addSensor("ExTmp");
	revokeTrip("ExTmp");
	revokeAlarm("ExTmp");
	enableTripAtHigh("ExTmp",1);
	setSensorMinMax("ExTmp",0,1000);

	tmp = (double)EXHAUST_TMP_TRIP_LEVEL * 1.024;
	tmp = ceil(tmp);
	setTripThresholdHigh("ExTmp",(uint32_t)tmp);

	tmp = (double)EXHAUST_TMP_ALARM_LEVEL * 1.024;
	tmp = ceil(tmp);
	setAlarmThresholdHigh("ExTmp",(uint32_t)tmp);

	setAlarmTimer("ExTmp",ALARM_DELAY);
	setTripTimer("ExTmp",TRIP_DELAY);
	setFailureTimer("ExTmp",5);
}


void analogScanner_updateReading(const char *sensorName, uint32_t value)
{
	Enter_Critical_Section
	uint32_t currentValue;
	currentValue = getSensorValue(sensorName);
	if(value != currentValue)
	{
		setSensorValue(sensorName,value);
		page_updateValue(sensorName);
	}
	Leave_Critical_Section
}

void analogScanner_scan()
{
	analogScanner_updateReading("GrBxPr",ADC_filterRead(GEARBOX_PRESSURE_CHANNEL));
	analogScanner_updateReading("GrBxTmp",ADC_filterRead(GEARBOX_TEMP_CHANNEL));
	analogScanner_updateReading("ExTmp",ADC_filterRead(EXHAUST_TEMP_CHANNEL));
}

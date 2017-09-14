/*
 * SetPoints.h
 *
 *  Created on: 17-Mar-2017
 *      Author: piyush
 */

#ifndef SETPOINTS_H_
#define SETPOINTS_H_

#define		RPM_ALARM_LEVEL						3450
#define		RPM_TRIP_LEVEL							3450

#define		ECT_ALARM_LEVEL						95
#define		ECT_TRIP_LEVEL							105

#define		LOP_ALARM_LEVEL						1.2
#define		LOP_TRIP_LEVEL							0.8

#define		LOT_ALARM_LEVEL						120
#define		LOT_TRIP_LEVEL							130

#define		GEARBOX_TMP_ALARM_LEVEL		120
#define		GEARBOX_TMP_TRIP_LEVEL			130

#define		GEARBOX_PR_ALARM_LEVEL		1.2
#define		GEARBOX_PR_TRIP_LEVEL			0.8

#define		EXHAUST_TMP_ALARM_LEVEL		120				//Currently disabled
#define		EXHAUST_TMP_TRIP_LEVEL			130				//Currently disabled


#define 	ALARM_DELAY			5
#define		TRIP_DELAY				5

/**
 * ENERGISE_TO_STOP
 * If defined indicates that relay NC is trip condition.
 *
 * ENERGISE_TO_RUN
 * If defined indicates that relay NO is trip condition.
 *
 * If both are simultaneously defined, ETS will be used.
 */
//#define		ENERGISE_TO_STOP
#define		ENERGISE_TO_RUN


#endif /* SETPOINTS_H_ */

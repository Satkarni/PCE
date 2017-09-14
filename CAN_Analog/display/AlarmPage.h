/*
 * AlarmPage.h
 *
 *  Created on: 12-Aug-2016
 *      Author: piyush
 */
/**
 * @file AlarmPage.h
 * @author Piyush
 *
 * @brief Display pages for alarms
 */
#ifndef DISPLAY_ALARMPAGE_H_
#define DISPLAY_ALARMPAGE_H_

#include <string.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "../glcd/ks0108.h"
#include "../glcd/sanserif.h"


/**
 * \defgroup alarm_page Alarm Page Functions
 * @brief Contains function for displaying alarm pages
 *
 * Gets active alarms from alarm list and adds them to
 * the active alarm pages.
 *
 * @{
 */

#define 	ALARM_ENTRIES				5					///<Alarms per page

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

uint8_t alarmpage_head;										///<Contains current alarm page number

typedef struct alarm_data
{
	char value[25];												///<Alarm text to be displayed on the page
	uint8_t enabled :1;											///<Is alarm enabled

}alarm_data;

/*
 * Note: Ensure that alarm_types and trip_types are of
 * same length. This is to ensure provision for both alarm
 * and trip if required and prevent sensor library conflicts.
 * If a particular sensor does not support trip or alarm, then
 * add the entry and leave the value field blank during alarm
 * page initialization.
 */
enum alarm_types {
	RPM_ALARM,
	LOP_ALARM,
	LOT_ALARM,
	ECT_ALARM,
	GBP_ALARM,
	GBT_ALARM,
	EXT_ALARM,
	ALARM_LIST_LENGTH
}alarm_types;

enum trip_types{

	RPM_TRIP,
	LOP_TRIP,
	LOT_TRIP,
	ECT_TRIP,
	GBP_TRIP,
	GBT_TRIP,
	EXT_TRIP,
	TRIP_LIST_LENGTH
}trip_types;

alarm_data alarms[ALARM_LIST_LENGTH], trips[TRIP_LIST_LENGTH];

uint8_t activeAlarmList[5];

uint8_t max_alarm_page;

uint8_t alarms_count, trips_count;


void alarmPage_initAlarms();

void alarmPage_clearList();

/**
 * @brief Adds active alarms to alarm pages
 *
 * All alarms present in the alarms linked list will be added to
 * the alarm pages. Overwrites any previous values.
 */
void alarmPage_generatePages();

/**
 *	@brief ABSTRACT: Resets page head and list head to initial positions
 *
 *	Sets the alarm page head to zero hence setting the current page value to the
 *	first page. Also resets the list head to the first item in the active alarms list.
 *
 *	@return Current value of the alarm page head
 */
uint8_t alarmPage_resetHead();


/**
 * @brief ABSTRACT: Change to next alarm page
 *
 * Moves the alarm page head to the next page in the alarm pages.
 * If the current page is already the last page, the head is moved to
 * the first page.
 *
 * @return Value of alarm page head
 */
uint8_t alarmPage_nextPage();

/**
 * @brief ABSTRACT: Change to previous alarm page
 *
 * Moves the alarm page head to the previous page in the alarm pages.
 * If the current page is already the first page, the head is moved to
 * the last page.
 *
 * @return Value of alarm page head
 */
uint8_t alarmPage_prevPage();

/**
 * @brief Draws the basic structure of Alarm Page
 *
 * Draws the static structure of the alarm page. This function
 * is called by Display.h
 */
void alarmPage_drawStaticPage();

void alarmPage_drawDynamicPage();

///@}

#endif /* DISPLAY_ALARMPAGE_H_ */

/*
 * TimerDAO.h
 *
 *  Created on: 16-Jul-2016
 *      Author: piyush
 */

/**
 * @file TimerDAO.h
 * @brief ABSTRACTED: This library is for internal use ONLY.
 *
 * This file contains all functions related to handling data for timer queues.
 */
#ifndef TIMERDAO_H_
#define TIMERDAO_H_

#include "SensorsDAO.h"

/**
 * @defgroup timer_data Timer Data
 * ABSTRACTED: Timer data contained in every active sensor
 * @{
 */
typedef struct Timer
{
	char name[10];			///<Name of the sensor for which this timer exists

	uint16_t alarmCount;	///<Current timer count for alarm
	uint16_t tripCount;		///<Current timer count for trip
	uint16_t failureCount;	///<Current timer count for failure condition.

	uint16_t alarmMax;		///<Maximum count for alarm count
	uint16_t tripMax;		///<Maximum count for trip count
	uint16_t failureMax;	///<Maximum count for failure condition

	struct Timer *next;
}Timer;

///@}

/**
 * @defgroup timer_list Timer List Handlers
 * ABSTRACTED: Functions for accessing and handling data within the timer queue
 *
 * @{
 */

/**
 * @brief Adds a timer to the queue
 * @param name Name of the sensor for which this timer refers to
 * @return 1 for success. 0 for failure.
 */
uint8_t addTimer(char *name);

/**
 * @brief Removes a timer from the queue
 * @param name Name of the sensor for which this timer refers to
 * @return 1 for success. 0 if timer doesn't exist.
 */
uint8_t removeTimer(char *name);

/**
 * @brief Get a timer in the queue by name
 * @param name Name of the timer
 * @return Returns a pointer to the timer in the queue.
 */
Timer* getTimerByName (char *name);

/**
 * @brief Get a timer in the queue by position
 * @param position Position of the timer in the queue
 * @return Returns a pointer rto the timer in the queue.
 */
Timer* getTimerByPosition (uint8_t position);


/**
 * @brief DEBUG: Shows the timer queue in the console
 */
void showTimerQueue(); ////TESTING FUNCTION ONLY!! REMOVE IN FINAL BUILD!

///@}


#endif /* TIMERDAO_H_ */

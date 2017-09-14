/*
 * TimerQueue.h
 *
 *  Created on: 16-Jul-2016
 *      Author: piyush
 */

/**
 * @file TimerQueue.h
 * @brief Handles the timer queue
 *
 * This file contains functions used to handle the timer queue.
 * All functions in this library can safely be used externally.
 */

#ifndef TIMERQUEUE_H_
#define TIMERQUEUE_H_

#include "TimerDAO.h"
#include "SensorHandler.h"

/**
 * @defgroup timer_handler Timer Handler
 * This group contains functions used for handling the timer queue
 *
 * @{
 */

/**
 * @brief Creates a new timer queue
 * @return 1 if successful. 0 if failed.
 *
 * All enabled sensors will be added to the timer queue alongwith
 * their timer parameters. Please not that disabled sensors will
 * not be added to the queue.
 */
uint8_t initialiseTimerQueue();

/**
 * @brief Removes disabled sensors from queue
 * @return 1 if successful. 0 if failed.
 *
 * This function should be called if any sensors are disabled during
 * the course of the program, and after the timer queue has been initialised.
 * All sensors in the timer queue which have been disabled will be removed.
 */
uint8_t recheckTimerQueue();

/**
 * @brief Removes the timer queue
 * @return 1 if successful. 0 if timer queue is empty.
 *
 * All timers in the timer queue are removed, and the memory allocated
 * to the queue is freed.
 */
uint8_t clearTimerQueue();

/**
 * @brief Runs one clock cycle for the timer queue
 * @return 1 if clock ran successfully. 0 if no timer queue exists.
 *
 * Timer counts for all timers in the queue will be conditionally incremented
 * until they reach their upper limit. Note that this function will not increment
 * timers if the corresponding sensor state is already active. For example,
 * if the sensor has already tripped, the timer for sensor trip will not be incremented
 * regardless of the sensor value meeting the trip condition.
 */
uint8_t clockTimerQueue();

/**
 * @brief Checks if alarm timer has timed out
 * @param name Name of the sensor
 * @return 1 if true. 0 if false.
 *
 * Checks whether the alarm timer has hit its maximum value. For example
 * if the alarm delay is set to 10 seconds, this function will return 1 if the
 * alarm timer has reached 10.
 */
uint8_t alarmTimeout(char *name);

/**
 * @brief Checks if trip timer has timed out
 * @param name Name of the sensor
 * @return 1 if true. 0 if false.
 *
 * Checks whether the trip timer has hit its maximum value. For example
 * if the trip delay is set to 10 seconds, this function will return 1 if the
 * trip timer has reached 10.
 */
uint8_t tripTimeout(char *name);

/**
 * @brief Checks if failure timer has timed out
 * @param name Name of the sensor
 * @return 1 if true. 0 if false.
 *
 * Checks whether the failure timer has hit its maximum value. For example
 * if the failure delay is set to 10 seconds, this function will return 1 if the
 * failure timer has reached 10.
 */
uint8_t failureTimeout(char *name);

///@}

#endif /* TIMERQUEUE_H_ */

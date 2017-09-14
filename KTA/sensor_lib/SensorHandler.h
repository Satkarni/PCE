/*
 * SensorHandler.h
 *
 *  Created on: 16-Jul-2016
 *      Author: piyush
 */

/**
 * @file SensorHandler.h
 * @brief Contains all sensor management functions
 *
 * This is a public library to manage all sensors. All functions in this library
 * can be used externally, i.e. none of the functions have been abstracted.
 */

#ifndef SENSORHANDLER_H_
#define SENSORHANDLER_H_

#include "SensorsDAO.h"
#include "TimerQueue.h"

/**
 *@defgroup sensor_handler Sensor Handlers
 *Functions used for managing sensors.
 *@{
 */

/**
 * @brief Checks whether a sensor has exceeded its trip threshold
 * @param name Name of the sensor
 * @return 1 if the sensor has exceeded trip threshold, 0 if it hasn't
 *
 * If the sensor has crossed its threshold, this function will return 1, otherwise it will return 0.
 * It does not check if the sensor has already tripped or not, but only if the sensor's current value
 * is greater/lesser than its rated threshold.
 *
 */
uint8_t checkSensorTrip(char *name);

/**
 * @brief Checks whether a sensor has exceeded its trip threshold
 * @param position Position of the sensor
 * @return 1 if the sensor has exceeded trip threshold, 0 if it hasn't
 *
 * If the sensor has crossed its threshold, this function will return 1, otherwise it will return 0.
 * It does not check if the sensor has already tripped or not, but only if the sensor's current value
 * is greater/lesser than its rated threshold.
 *
 */
uint8_t checkSensorTrip_pos(uint8_t position);

/**
 * @brief Checks whether a sensor has exceeded its alarm threshold
 * @param name Name of the sensor
 * @return 1 if the sensor has exceeded alarm threshold, 0 if it hasn't
 *
 *  If the sensor has crossed its threshold, this function will return 1, otherwise it will return 0.
 * 	It does not check if the sensor is in alarm state or not, but only if the sensor's current value is
 * 	greater/lesser than its rated threshold.
 */
uint8_t checkSensorAlarm(char *name);

/**
 * @brief Checks whether a sensor has exceeded its alarm threshold
 * @param position Position of the sensor
 * @return 1 if the sensor has exceeded alarm threshold, 0 if it hasn't
 *
 *  If the sensor has crossed its threshold, this function will return 1, otherwise it will return 0.
 * 	It does not check if the sensor is in alarm state or not, but only if the sensor's current value is
 * 	greater/lesser than its rated threshold.
 */
uint8_t checkSensorAlarm_pos(uint8_t position);


/**
 * @brief Enables trip, alarm or failure state of sensors
 *
 * This function should be called at a hardware timer interrupt.
 * First, a function is checked for failure. If it has reached its timeout for
 * failure condition, then the sensor's failure state is enabled. If the sensor hasn't
 * failed, then the function will check if the sensor has reached its timeouts for alarm
 * and/or trip conditions. If it has, then it will set those respective states.
 */
void processSensors();

/**
 * @brief Checks whether a sensor has crossed its failure threshold
 * @param name Name of the sensor
 * @return 1 if the sensor has crossed its failure threshold, 0 if it hasn't
 *
 * If the sensor drops below its rated minimum value or exceeds its rated maximum value,
 * this function will return 1. It does not set or reset the failure state of a sensor, but
 * only whether the sensor has currently crossed its respective minimum or maximum values.
 */
uint8_t checkFailure(char *name);

/**
 * @brief Checks whether a sensor has crossed its failure threshold
 * @param position Position of the sensor
 * @return 1 if the sensor has crossed its failure threshold, 0 if it hasn't
 *
 * If the sensor drops below its rated minimum value or exceeds its rated maximum value,
 * this function will return 1. It does not set or reset the failure state of a sensor, but
 * only whether the sensor has currently crossed its respective minimum or maximum values.
 */
uint8_t checFailure_pos(uint8_t position);

/**
 * @brief Resets all sensors
 * @return Number of sensors currently in failure, alarm, or trip state. 0 if all sensors are OK.
 *
 * Checks if all sensors are reporting normal readings and resets their states if they are.
 * If there are any sensors currently in failed, alarm, or trip state, then it will return
 * the number of sensors in this state.
 */
uint8_t resetSensors();

/**
 * @brief Mutes all alarms
 * @return Number of active alarms. 0 if no alarms found.
 *
 * All alarms will be muted when this function is called. It does not however reset the
 * alarm state of any sensor.
 */
uint8_t muteSensors();

/**
 * @brief Checks if mute is active
 * @return 1 if mute enabled. 0 if mute disabled.
 */
uint8_t isMuteActive();

/**
 * @}
 */


#endif /* SENSORHANDLER_H_ */

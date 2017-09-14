/*
 * SensorsDAO.h
 *
 *  Created on: 14-Jul-2016
 *      Author: piyush
 */

/**
 * @file SensorsDAO.h
 * @brief Data Access Object module for Sensors
 *
 * WARNING: ALL MEMBERS DOCUMENTED AS "ABSTRACTED" ARE FOR INTERNAL USE ONLY
 * Contains the member variables for all sensors and functions to access them. Please note that all sensor parameters
 * can be accessed via the getters and setters written for them. Do not attempt to access the parameters
 * using direct linked list pointers, as this may lead to undefined behaviour.
 */

/**
 * @defgroup sensor_setters Sensor Setters
 * All setter functions for sensors. This includes setters by name, and setters by position.
 * Note that all parameter values should be changed using setters only.
 *
 * @defgroup sensor_getters Sensor Getters
 * All getter functions for sensors. This includes getters by name, and getters by position.
 * Note that all parameter values should be read using getters only.
 *
 * @defgroup sensor_list	Sensor Linked List Handlers
 * This group contains only 2 functions which can be used externally (add, remove).
 * The showSensors function is to be used for debugging/testing. All other functions are
 * essentially abstracted.
 */
#ifndef SENSORSDAO_H_
#define SENSORSDAO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Sensor
{
	/**\defgroup sensor_members	Sensor Variables
	 * Contains all variables and parameters for sensors.
	 * @{
	 */
      char name[10];			///<Name of the Sensor

      uint32_t value;			///<Current reported value of the sensor
      uint32_t minValue;		///<The minimum value a sensor can possibly report
      uint32_t maxValue;		///<The maximum value a sensor can possibly report

      uint8_t enabled :1;		///<Determines if the Sensor has been enabled or disabled
      uint8_t tripEnabled :1;	///<This is set to 1 when the sensor trips
      uint8_t alarmEnabled :1;	///<This is set to 1 when the sensor has tripped its alarm
      uint8_t failureEnabled :1;///<This is set to 1 if the sensor has failed

      uint8_t tripAtHigh :1;	///<Set to 1 if the sensor trips on exceeding upper threshold. Set to 0 if the sensor trips on dropping below lower threshold*

      uint8_t allowTrip: 1;		///<Set to 1 if the sensor is allowed to trip
      uint32_t thresholdLow;	///<Lower threshold for sensor trip
      uint32_t thresholdHigh;	///<Upper threshold for sensor trip

      uint8_t allowAlarm :1;	///<Set to 1 if the sensor is allowed to have an alarm state
      uint32_t alarmThresholdLow;///<Lower threshold for sensor alarm state
      uint32_t alarmThresholdHigh;///<Upper threshold for sensor alarm state

      uint16_t resetDelay;		///<Delay before the sensor resets (FUTURE USE. NOT IMPLEMENTED)

      uint16_t alarmTimerMax;	///<Delay before the sensor is in alarm state
      uint16_t tripTimerMax;	///<Delay before the sensor trips
      uint16_t failureTimerMax;	///<Delay before the sensor is reported to have failed

      struct Sensor *next;		///<ABSTRACTED: Points to the next sensor in linked list
      /**
       * @}
       */
}Sensor;


//Linked List Handlers
/**
 * @ingroup sensor_list
 * @brief Adds a new sensor
 *
 * @param name Name of the Sensor
 * @return 1 if successful, 0 if failed
 *
 * By default, a new sensor is always enabled. Trip and alarms are also allowed by default.
 * Note that all new sensors need to have a failure timer greater than zero to allow alarm
 * and trip timers to function properly.
 */
uint8_t addSensor(char *name);

/**
 * @ingroup sensor_list
 * @brief ABSTRACTED: Gets a sensor's position in the linked list by name.
 * @param name Name of the sensor
 * @return Returns a pointer to the sensor in the linked list. NULL if not found.
 */
Sensor* getSensorByName(char *name);

/**
 * @ingroup sensor_list
 * @brief ABSTRACTED: Gets a sensor's position in the linked list by position
 * @param position Position of the sensor in the linked list
 * @return Returns a pointer to the sensor in the linked list. NULL if not found.
 */
Sensor* getSensorByPosition(uint8_t position);

/**
 * @ingroup sensor_list
 * @brief Removes an existing sensor
 * @param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t removeSensor(char *name);


/**
 * @ingroup sensor_list
 * @brief DEBUG/TESTING: Displays a list of all existing sensors in the console
 *
 * This function is for debugging and testing only. Do not use this for any other purposes.
 */
void showAllSensors();


//Setters By Name

/**
 * @ingroup sensor_setters
 * @brief Sets the reported value of the sensor
 * @param name Name of the sensor
 * @param value Value to be set
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setSensorValue(char *name, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Enables the sensor
 * @param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t enableSensor(char *name);

/**
 * @ingroup sensor_setters
 * @brief Disables the sensor
 * @param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t disableSensor(char *name);

/**
 * @ingroup sensor_setters
 * @brief Sets the minimum value and maximum value of the sensor
 * @param name Name of the sensor
 * @param min Minimum value allowed for the sensor
 * @param max Maximum value allowed for the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the maximum and minimum values that the sensor is allowed to report, or can possibly report.
 * It does not indicate threshold values.
 */
uint8_t setSensorMinMax(char *name, uint32_t min, uint32_t max);

/**
 * @ingroup sensor_setters
 * @brief Sets sensor's trip state
 * @param name Name of the sensor
 * @param enabled 1 indicates sensor has tripped, 0 indicates it has not
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t enableSensorTrip(char *name, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Sets sensor's alarm state
 * @param name Name of the sensor
 * @param enabled 1 indicates sensor is in alarm state, 0 indicates it is not
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t enableSensorAlarm(char *name, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Sets sensor failure state
 * @param name Name of the sensor
 * @param enabled 1 indicates sensor has failed, 0 indicates it has not
 * @return 1 if successful, 0 if sensor does not exist
 *
 * Please note that sensor failure is directly related to the "minValue" and "maxValue" of the sensor.
 * Ensure that these values have been set correctly for this function to work.
 */
uint8_t enableSensorFail(char *name, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Sets the active trip/alarm threshold for sensor
 * @param name Name of the sensor
 * @param enabled 1 indicates High Trip, 0 indicates Low Trip
 * @return 1 if successful, 0 if sensor does not exist
 *
 * If this value is set to 1, the sensor will be set to trip at off-scale high. Otherwise, if it is
 * set to 0, the sensor will trip at off-scale low. This setting is shared by the alarm state.
 */
uint8_t enableTripAtHigh(char *name, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Allows the sensor to trip
 * @param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowTrip" variable to 1 indicating that the sensor is allowed to trip.
 * All sensors are allowed to trip by default on addition.
 */
uint8_t allowTrip(char *name);

/**
 * @ingroup sensor_setters
 * @brief Prevents the sensor from ever tripping.
 * param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowTrip" variable to 0 indicating that the sensor will never trip.
 * All sensors are allowed to trip by default on addition.
 */
uint8_t revokeTrip(char *name);

/**
 * @ingroup sensor_setters
 * @brief Allows the sensor to sound an alarm
 * @param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowAlarm" variable to 1 indicating that the sensor is allowed to sound an alarm.
 * All sensors are allowed to sound alarms by default on addition.
 */
uint8_t allowAlarm(char *name);

/**
 * @ingroup sensor_setters
 * @brief Prevents the sensor from ever sounding an alarm
 * @param name Name of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowAlarm" variable to 0 indicating that the sensor will never sound an alarm.
 * All sensors are allowed to sound alarms by default on addition.
 */
uint8_t revokeAlarm(char *name);

/**
 * @ingroup sensor_setters
 * @brief Sets the Upper Threshold for sensor trip
 * @param name Name of the Sensor
 * @param value The value to set for Upper Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setTripThresholdHigh(char *name, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Sets the Lower Threshold for sensor trip
 * @param name Name of the Sensor
 * @param value The value to set for Lower Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setTripThresholdLow(char *name, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Sets the Lower Threshold for sensor alarm
 * @param name Name of the Sensor
 * @param value The value to set for Lower Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setAlarmThresholdLow(char *name, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Sets the Upper Threshold for sensor alarm
 * @param name Name of the Sensor
 * @param value The value to set for Upper Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setAlarmThresholdHigh(char *name, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief FUTURE USE: Delay before the sensor resets to normal
 * @param name Name of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setResetDelay(char *name, uint16_t value);

/**
 * @ingroup sensor_setters
 * @brief Delay before the sensor sounds an alarm on crossing its threshold
 * @param name Name of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setAlarmTimer(char *name, uint16_t value);

/**
 * @ingroup sensor_setters
 * @brief Delay before the sensor trips on crossing its threshold
 * @param name Name of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setTripTimer(char *name, uint16_t value);

/**
 * @ingroup sensor_setters
 * @brief Delay before the sensor is reported to have failed
 * @param name Name of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 *
 * Please note that failure delay is indirectly related to the "minValue" and "maxValue" of the sensor.
 * Ensure that these values have been set correctly for this function to work.
 */
uint8_t setFailureTimer(char *name, uint16_t value);


//Setters By Position

/**
 * @ingroup sensor_setters
 * @brief Sets the reported value of the sensor
 * @param position Relative position of the sensor
 * @param value Value to be set
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setSensorValue_pos(uint8_t position, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Enables the sensor
 * @param position Relative position of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t enableSensor_pos(uint8_t position);

/**
 * @ingroup sensor_setters
 * @brief Disables the sensor
 * @param position Relative position of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t disableSensor_pos(uint8_t position);

/**
 * @ingroup sensor_setters
 * @brief Sets the minimum value and maximum value of the sensor
 * @param position Relative position of the sensor
 * @param min Minimum value allowed for the sensor
 * @param max Maximum value allowed for the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the maximum and minimum values that the sensor is allowed to report, or can possibly report.
 * It does not indicate threshold values.
 */
uint8_t setSensorMinMax_pos(uint8_t position, uint32_t min, uint32_t max);

/**
 * @ingroup sensor_setters
 * @brief Sets sensor's trip state
 * @param position Relative position of the sensor
 * @param enabled 1 indicates sensor has tripped, 0 indicates it has not
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t enableSensorTrip_pos(uint8_t position, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Sets sensor's alarm state
 * @param position Relative position of the sensor
 * @param enabled 1 indicates sensor is in alarm state, 0 indicates it is not
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t enableSensorAlarm_pos(uint8_t position, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Sets sensor failure state
 * @param position Relative position of the sensor
 * @param enabled 1 indicates sensor has failed, 0 indicates it has not
 * @return 1 if successful, 0 if sensor does not exist
 *
 * Please note that sensor failure is directly related to the "minValue" and "maxValue" of the sensor.
 * Ensure that these values have been set correctly for this function to work.
 */
uint8_t enableSensorFail_pos(uint8_t position, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Sets the active trip/alarm threshold for sensor
 * @param position Relative position of the sensor
 * @param enabled 1 indicates High Trip, 0 indicates Low Trip
 * @return 1 if successful, 0 if sensor does not exist
 *
 * If this value is set to 1, the sensor will be set to trip at off-scale high. Otherwise, if it is
 * set to 0, the sensor will trip at off-scale low. This setting is shared by the alarm state.
 */
uint8_t enableTripAtHigh_pos(uint8_t position, uint8_t enabled);

/**
 * @ingroup sensor_setters
 * @brief Allows the sensor to trip
 * @param position Relative position of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowTrip" variable to 1 indicating that the sensor is allowed to trip.
 * All sensors are allowed to trip by default on addition.
 */
uint8_t allowTrip_pos(uint8_t position);

/**
 * @ingroup sensor_setters
 * @brief Prevents the sensor from ever tripping.
 * param position Relative position of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowTrip" variable to 0 indicating that the sensor will never trip.
 * All sensors are allowed to trip by default on addition.
 */
uint8_t revokeTrip_pos(uint8_t position);

/**
 * @ingroup sensor_setters
 * @brief Allows the sensor to sound an alarm
 * @param position Relative position of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowAlarm" variable to 1 indicating that the sensor is allowed to sound an alarm.
 * All sensors are allowed to sound alarms by default on addition.
 */
uint8_t allowAlarm_pos (uint8_t position);

/**
 * @ingroup sensor_setters
 * @brief Prevents the sensor from ever sounding an alarm
 * @param position Relative position of the sensor
 * @return 1 if successful, 0 if sensor does not exist
 *
 * This function sets the "allowAlarm" variable to 0 indicating that the sensor will never sound an alarm.
 * All sensors are allowed to sound alarms by default on addition.
 */
uint8_t revokeAlarm_pos(uint8_t position);

/**
 * @ingroup sensor_setters
 * @brief Sets the Upper Threshold for sensor trip
 * @param position Relative position of the sensor
 * @param value The value to set for Upper Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setTripThresholdHigh_pos(uint8_t position, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Sets the Lower Threshold for sensor trip
 * @param position Relative position of the sensor
 * @param value The value to set for Lower Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setTripThresholdLow_pos(uint8_t position, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Sets the Lower Threshold for sensor alarm
 * @param position Relative position of the sensor
 * @param value The value to set for Lower Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setAlarmThresholdLow_pos(uint8_t position, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief Sets the Upper Threshold for sensor alarm
 * @param position Relative position of the sensor
 * @param value The value to set for Upper Threshold
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setAlarmThresholdHigh_pos(uint8_t position, uint32_t value);

/**
 * @ingroup sensor_setters
 * @brief FUTURE USE: Delay before the sensor resets to normal
 * @param position Relative position of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setResetDelay_pos(uint8_t position, uint16_t value);

/**
 * @ingroup sensor_setters
 * @brief Delay before the sensor sounds an alarm on crossing its threshold
 * @param position Relative position of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setAlarmTimer_pos(uint8_t position, uint16_t value);

/**
 * @ingroup sensor_setters
 * @brief Delay before the sensor trips on crossing its threshold
 * @param position Relative position of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 */
uint8_t setTripTimer_pos(uint8_t position, uint16_t value);

/**
 * @ingroup sensor_setters
 * @brief Delay before the sensor is reported to have failed
 * @param position Relative position of the sensor
 * @param value Delay (seconds)
 * @return 1 if successful, 0 if sensor does not exist
 *
 * Please note that failure delay is indirectly related to the "minValue" and "maxValue" of the sensor.
 * Ensure that these values have been set correctly for this function to work.
 */
uint8_t setFailureTimer_pos(uint8_t position, uint16_t value);



//Getters By Name

/**
 * @ingroup sensor_getters
 * @brief Returns the current sensor value
 * @param name Name of the sensor
 * @return Sensor's current value
 */
uint32_t getSensorValue(char *name);

/**
 * @ingroup sensor_getters
 * @brief Returns the currently set minimum possible value for sensor
 * @param name Name of the sensor
 * @return Least possible value the sensor can take
 */
uint32_t getSensorMin(char *name);

/**
 * @ingroup sensor_getters
 * @brief Returns the currently set maximum possible value for sensor
 * @param name Name of the sensor
 * @return Greatest possible value the sensor can take
 */
uint32_t getSensorMax(char *name);

/**
 * @ingroup sensor_getters
 * @brief Checks if sensor is enabled
 * @param name Name of the sensor
 * @return 1 if the sensor is enabled, 0 if it is disabled
 */
uint8_t	isSensorEnabled(char *name);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor has tripped
 * @param name Name of the sensor
 * @return 1 if the sensor has tripped, 0 if it hasn't
 */
uint8_t isTripEnabled(char *name);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor's alarm is enabled
 * @param name Name of the sensor
 * @return 1 if the sensor alarm is enabled, 0 if it isn't
 */
uint8_t isAlarmEnabled(char *name);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor has failed
 * @param name Name of the sensor
 * @return 1 if the sensor has failed, 0 if it hasn't
 */
uint8_t isSensorFail(char *name);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor is set to trip at OffScale High or OffScale Low
 * @param name Name of the Sensor
 * @return 1 if the sensor is set to trip at OffScale High, 0 if it is set to trip at OffScale Low
 *
 * Note that this setting is shared by sensor trip and sensor alarm
 */
uint8_t isTripAtHigh(char *name);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor is allowed to trip
 * @param name Name of the sensor
 * @return 1 if the sensor is allowed to trip, 0 if it can never trip
 */
uint8_t isAllowTrip(char *name);

/**
 *@ingroup sensor_getters
 *@brief Checks if the sensor is allowed to sound an alarm
 *@param name Name of the sensor
 *@return 1 if the sensor is allowed to sound an alarm, 0 if it can never sound an alarm
 */
uint8_t isAllowAlarm(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the Lower Threshold for sensor trip
 * @param name Name of the sensor
 * @return The current Lower Threshold for sensor trip
 */
uint32_t getTripThresholdLow(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the Upper Threshold for sensor trip
 * @param name Name of the sensor
 * @return The current Upper Threshold for sensor trip
 */
uint32_t getTripThresholdHigh(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the Upper Threshold for alarm trip
 * @param name Name of the sensor
 * @return The current Upper Threshold for alarm trip
 */
uint32_t getAlarmThresholdHigh(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the Lower Threshold for alarm
 * @param name Name of the sensor
 * @return The current Lower Threshold for alarm
 */
uint32_t getAlarmThresholdLow(char *name);

/**
 * @ingroup sensor_getters
 * @brief FUTURE: Gets the delay before sensors are reset
 * @param name Name of the sensor
 * @return The current reset delay for the sensor
 */
uint16_t getResetDelay(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the current active delay before sounding an alarm
 * @param name Name of the sensor
 * @return The current alarm delay for the sensor
 */
uint16_t getAlarmTimer(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the current active delay before the sensor trips
 * @param name Name of the sensor
 * @return The current trip delay for the sensor
 */
uint16_t getTripTimer(char *name);

/**
 * @ingroup sensor_getters
 * @brief Gets the current active delay before the sensor is reported to have failed
 * @param name Name of the sensor
 * @return The current failure report delay for the sensor
 */
uint16_t getFailureTimer(char *name);


//Getters By Position

/**
 * @ingroup sensor_getters
 * @brief Gets the name of the sensor
 * @param position Position of the sensor
 * @return The name of the sensor at "position"
 */
char* getSensorName(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Returns the current sensor value
 * @param position Position of the sensor
 * @return Sensor's current value
 */
uint32_t getSensorValue_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Returns the currently set minimum possible value for sensor
 * @param position Position of the sensor
 * @return Least possible value the sensor can take
 */
uint32_t getSensorMin_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Returns the currently set maximum possible value for sensor
 * @param position Position of the sensor
 * @return Greatest possible value the sensor can take
 */
uint32_t getSensorMax_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Checks if sensor is enabled
 * @param position Position of the sensor
 * @return 1 if the sensor is enabled, 0 if it is disabled
 */
uint8_t	isSensorEnabled_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor has tripped
 * @param position Position of the sensor
 * @return 1 if the sensor has tripped, 0 if it hasn't
 */
uint8_t isTripEnabled_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor's alarm is enabled
 * @param position Position of the sensor
 * @return 1 if the sensor alarm is enabled, 0 if it isn't
 */
uint8_t isAlarmEnabled_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor has failed
 * @param position Position of the sensor
 * @return 1 if the sensor has failed, 0 if it hasn't
 */
uint8_t isSensorFail_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor is set to trip at OffScale High or OffScale Low
 * @param position Position of the sensor
 * @return 1 if the sensor is set to trip at OffScale High, 0 if it is set to trip at OffScale Low
 *
 * Note that this setting is shared by sensor trip and sensor alarm
 */
uint8_t isTripAtHigh_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Checks if the sensor is allowed to trip
 * @param position Position of the sensor
 * @return 1 if the sensor is allowed to trip, 0 if it can never trip
 */
uint8_t isAllowTrip_pos(uint8_t position);

/**
 *@ingroup sensor_getters
 *@brief Checks if the sensor is allowed to sound an alarm
 *@param position Position of the sensor
 *@return 1 if the sensor is allowed to sound an alarm, 0 if it can never sound an alarm
 */
uint8_t isAllowAlarm_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the Lower Threshold for sensor trip
 * @param position Position of the sensor
 * @return The current Lower Threshold for sensor trip
 */
uint32_t getTripThresholdLow_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the Upper Threshold for sensor trip
 * @param position Position of the sensor
 * @return The current Upper Threshold for sensor trip
 */
uint32_t getTripThresholdHigh_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the Upper Threshold for alarm trip
 * @param position Position of the sensor
 * @return The current Upper Threshold for alarm trip
 */
uint32_t getAlarmThresholdHigh_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the Lower Threshold for alarm
 * @param position Position of the sensor
 * @return The current Lower Threshold for alarm
 */
uint32_t getAlarmThresholdLow_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief FUTURE: Gets the delay before sensors are reset
 * @param position Position of the sensor
 * @return The current reset delay for the sensor
 */
uint16_t getResetDelay_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the current active delay before sounding an alarm
 * @param position Position of the sensor
 * @return The current alarm delay for the sensor
 */
uint16_t getAlarmTimer_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the current active delay before the sensor trips
 * @param position Position of the sensor
 * @return The current trip delay for the sensor
 */
uint16_t getTripTimer_pos(uint8_t position);

/**
 * @ingroup sensor_getters
 * @brief Gets the current active delay before the sensor is reported to have failed
 * @param position Position of the sensor
 * @return The current failure report delay for the sensor
 */
uint16_t getFailureTimer_pos(uint8_t position);





#endif /* SENSORSDAO_H_ */

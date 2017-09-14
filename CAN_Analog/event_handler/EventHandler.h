/*
 * EventHandler.h
 *
 *  Created on: 13-Aug-2016
 *      Author: piyush
 */

/**
 * @file EventHandler.h
 * @author Piyush
 *
 * Contains functions for handling all events on the CAN Controller
 */
#ifndef EVENT_HANDLER_EVENTHANDLER_H_
#define EVENT_HANDLER_EVENTHANDLER_H_

/**
 * \defgroup event_handler Event Handler
 * @brief Functions and data structures for Event Handler
 *
 * @{
 */
#include "../event_handler/KeypadTriggers.h"
#include "../display/AlarmPage.h"

/**
 * @brief Byte array to hold keypad press events
 *
 * If a keypad button is pressed, the byte at the corresponding
 * enum address will be set to 1. Once the event is processed,
 * the byte will be reset to 0.
 */
uint8_t keypad_events[7], alarm_events[ALARM_LIST_LENGTH], trip_events[TRIP_LIST_LENGTH];

uint8_t value_change_event;

enum keypad_key {
	NULL_KEY,											///<No key press
	LEFT_ARROW,									///<Left arrow key press
	RIGHT_ARROW,									///<Right arrow key press
	UP_ARROW,										///<Up arrow key press
	DOWN_ARROW,									///<Down arrow key press
	ENTER,												///<Enter key press
	BACK_RESET										///<Back key press
}keypad_key;



/**
 * @brief Priority execution of all events
 *
 * All events are handled by this function on a priority basis.
 * It checks for event occurrences and triggers the corresponding
 * function to perform an action.
 */
void selectEvent();
/**
 * @brief Called every time a key-press event occurs
 *
 * The key been pressed is passed as a parameter to the function.
 * This function directly modifies the keypad_events array.
 *
 * @param key Enum value of key pressed
 */
void setKeypadEvent(enum keypad_key key);

void setValueChangeEvent(uint8_t parameter_position);

void setAlarmEvent (enum alarm_types alarm_type);

void setTripEvent(enum trip_types trip_type);

/**
 * @brief ABSTRACT: Checks for key-press events
 *
 * Reads the keypad_events array and checks for any registered
 * key-press events. If an event is detected, the key's enum value is returned,
 * and the corresponding byte in the keypad_events array is cleared.
 *
 * @return Enum value of the key which has been pressed.
 */
enum keypad_key scanKeypadEvent();

enum alarm_types scanAlarmEvent();

enum trip_types scanTripEvent();

/**
 * @brief ABSTRACT: Calls appropriate keypad triggers
 *
 * Called by the selectEvent() function. Calls the appropriate
 * key press trigger from KeypadTriggers.h.
 */
void triggerKeypadEvent(enum keypad_key key);

void triggerValueChangeEvent(uint8_t position);

void triggerAlarmEvent (enum alarm_types alarm_type);

void triggerTripEvent(enum trip_types trip_type);

///@}

#endif /* EVENT_HANDLER_EVENTHANDLER_H_ */

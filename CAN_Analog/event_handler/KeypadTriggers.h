/*
 * KeypadTriggers.h
 *
 *  Created on: 13-Aug-2016
 *      Author: piyush
 */

/**
 * @file KeypadTriggers.h
 * @author Piyush
 *
 * @brief ABSTRACT: Contains triggers for keypad events
 */
#ifndef EVENT_HANDLER_KEYPADTRIGGERS_H_
#define EVENT_HANDLER_KEYPADTRIGGERS_H_

#include "../display/Display.h"
#include "../sensor_lib/SensorHandler.h"
#include "../inputoutput/ioHandler.h"

/**
 * \defgroup keypad_triggers Keypad Triggers [ABSTRACT]
 * @brief Triggers for keypad events
 *
 * @{
 */

void triggerRightArrow();			///<Trigger for Right-Arrow key-press
void triggerLeftArrow();			///<Trigger for Left-Arrow key-press
void triggerEnter();					///<Trigger for Enter key-press
void triggerUpArrow();				///<Trigger for Up-Arrow key-press
void triggerDownArrow();			///<Trigger for Down-Arrow key-press
void triggerBackReset();			///<Trigger for Back-Reset key-press

///@}

#endif /* EVENT_HANDLER_KEYPADTRIGGERS_H_ */

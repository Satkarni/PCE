/*
 * inputHandler.h
 *
 * Created: 8/4/2016 11:47:20 AM
 *  Author: Piyush
 */ 

/**
*	@file inputHandler.h
*	@brief Input functions for MCP23S17 (Keypad Input)
*	
*	Author: Piyush
*/

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include "MCP23S17.h"
#include "../DisplayInterface.h"
#include "../Hardware.h"

#define	UP_ARROW	0x01
#define	LEFT_ARROW	0x02
#define ENTER		0x03
#define RIGHT_ARROW	0x04
#define DOWN_ARROW	0x05
#define RESET_CLEAR 0x06
#define MUTE		0x07

/**\defgroup input_handler Input Handler
*	@brief Defines functions for Keypad Input
*
*	@{
*/

enum button{
	EN_UP_ARROW,		///<Up Arrow on Keypad
	EN_DOWN_ARROW,		///<Down Arrow on Keypad
	EN_LEFT_ARROW,		///<Left Arrow on Keypad
	EN_RIGHT_ARROW,	///<Right Arrow on Keypad
	EN_ENTER,			///<Enter Button on Keypad
	EN_RESET_CLEAR,	///<Reset or Clear Button on Keypad
	EN_MUTE			///<Mute Button on Keypad
	}button;
	
volatile uint8_t interrupt_received;

uint8_t enterPressCount, ratioFlag, factoryResetFlag, alarmToggleFlag, setPointFlag;
/**
*	@brief Returns the key pressed by user
*
*	This function checks which button has been pressed on the keypad, and returns its value.
*	It's best to call this function when an interrupt on change event occurs on Port A
*	of MCP23S17.
*
*	@return Enum value of the button press
*/

void keypadInit();
void scanKeypad();

void triggerUpArrow();
void triggerDownArrow();
void triggerLeftArrow();
void triggerRightArrow();
void triggerEnter();
void triggerResetClear();
void triggerMute();
///@}


#endif /* INPUTHANDLER_H_ */

/*
 * ioHandler.h
 *
 *  Created on: 13-Aug-2016
 *      Author: piyush
 */

/**
 * @file ioHandler.h
 * @author Rohan, Piyush
 * @brief Contains functions for handling hardware I/O
 *
 * Contains functions for initialising keypad buttons, and toggling LEDs.
 */

#ifndef INPUTOUTPUT_IOHANDLER_H_
#define INPUTOUTPUT_IOHANDLER_H_

#include <avr/interrupt.h>
#include	<util/delay.h>

#include "Hardware.h"
#include "../SetPoints.h"

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

/**
 * \defgroup iohandler Input/Output Functions
 * @brief Functions for handling hardware I/O
 *
 *	@{
 */

#define SW_RIGHT		(KEYPAD_B_REG_PIN & (1<<SW_ARROW_RIGHT))	///<Check if Right Arrow is pressed
#define SW_LEFT		(KEYPAD_A_REG_PIN & (1<<SW_ARROW_LEFT))		///<Check if Left Arrow is pressed
#define SW_UP			(KEYPAD_A_REG_PIN & (1<<SW_ARROW_UP))		///<Check if Up Arrow is pressed
#define SW_DOWN		(KEYPAD_B_REG_PIN & (1<<SW_ARROW_DOWN))	///<Check if Down Arrow is pressed
#define SW_ENTER_P	(KEYPAD_A_REG_PIN & (1<<SW_ENTER))				///<Check if Enter is pressed

void ioHandler_initKeypad();															///<Initialise the hardware keypad

void ioHandler_enablePowerLED();													///<Turn ON the Power LED
void ioHandler_disablePowerLED();													///<Turn OFF the Power LED
void ioHandler_togglePowerLED();													///<Toggle the current state of the Power LED

void ioHandler_enableFaultLED();														///<Turn ON the Fault LED
void ioHandler_disableFaultLED();													///<Turn OFF the Fault LED
void ioHandler_toggleFaultLED();														///<Toggle the current state of the Fault LED
void ioHandler_blinkFaultLED(uint8_t count);

void ioHandler_enableRelayStop();
void ioHandler_disableRelayStop();
void ioHandler_enableRelayBuzzer();
void ioHandler_disableRelayBuzzer();
///	@}

#endif /* INPUTOUTPUT_IOHANDLER_H_ */

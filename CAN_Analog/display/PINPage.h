/*
 * PINPage.h
 *
 *  Created on: 25-Aug-2016
 *      Author: piyush
 */

#ifndef DISPLAY_PINPAGE_H_
#define DISPLAY_PINPAGE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "../glcd/ks0108.h"
#include "../glcd/sanserif.h"
#include "../glcd/arial_bold_14.h"
#include "../inputoutput/ioHandler.h"

/**	\defgroup PIN_page PIN Page
*	@brief Contains data and functions for handling PIN input to access controller configuration.
*
*	@{
*/

#define		PIN_DIGIT1		0										///<1st Digit of PIN
#define		PIN_DIGIT2		0										///<2nd Digit of PIN
#define		PIN_DIGIT3		0										///<3rd Digit of PIN
#define		PIN_DIGIT4		0										///<4th Digit of PIN

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

uint8_t pinPage_HEAD;											///<Contains current cursor position

uint8_t pin_spinBox[4];											///<Contains the values of the digits displayed on screen

/**
 * @brief Resets the page
 *
 * Sets all PIN input boxes to 0 and moves the cursor
 * to the first PIN box.
 *
 * @return Returns the cursor position at first PIN box.
 */
uint8_t pinPage_reset();

/**
 * @brief Increments value of PIN box
 *
 * Increments the digit contained in the PIN box which is
 * currently being referenced by the cursor. PIN box can
 * take values between 0 to 9. The increment function
 * is cyclic, i.e. will change digit to 0 if it is 9.
 */
void pinPage_spinBoxIncrement();

/**
 * @brief Decrements value of PIN box
 *
 * Decrements the digit contained in the PIN box which is
 * currently being referenced by the cursor. PIN box can
 * take values between 0 to 9. The decrement function
 * is cyclic, i.e. will change digit to 9 if it is 0.
 */
void pinPage_spinBoxDecrement();

/**
 * @brief Draws the basic layout of the PIN page.
 *
 * Draws the basic layout of the PIN page which
 * remains static on user input.
 */
void pinPage_drawStaticPage();

/**
 * @brief Draws/redraws cursor on the screen
 *
 * Draws/redraws the cursor at the position contained
 * in pinPage_HEAD.
 */
void pinPage_drawCursor();

/**
 * @brief Moves cursor to the right.
 *
 * Increments the value of pinPage_HEAD and redraws
 * the cursor. If pinPage_HEAD is at the end, then it
 * returns the HEAD to the beginning.
 *
 * @return Updated value of pinPage_HEAD.
 */
uint8_t pinPage_next();

/**
 * @brief Moves cursor to the left.
 *
 * Decrements the value of pinPage_HEAD and redraws
 * the cursor. If pinPage_HEAD is at the beginning, then it
 * returns the HEAD to the end.
 *
 * @return Updated value of pinPage_HEAD.
 */
uint8_t pinPage_prev();

/**
 * @brief Checks whether entered PIN is correct
 *
 * Will check the entered PIN against values contained
 * in the macros PIN_DIGIT1 to PIN_DIGIT4.
 *
 * @return 1 if PIN is correct, 0 if PIN is incorrect.
 */
uint8_t pinPage_checkPin();

///@}
#endif /* DISPLAY_PINPAGE_H_ */

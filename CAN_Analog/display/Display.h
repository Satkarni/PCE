/*
 * Display.h
 *
 *  Created on: 12-Aug-2016
 *      Author: piyush
 */

/**
 * @file Display.h
 * @author Rohan Pandit, Piyush
 * @brief Contains main display functions for LCD
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

#include <util/delay.h>
#include "Page.h"
#include "ConfigPage.h"
#include "AlarmPage.h"
#include "PINPage.h"
#include "../glcd/ks0108.h"
#include "../glcd/pce_logo.h"
#include "../glcd/sanserif.h"
#include "../EEPROM.h"
/**
 * \defgroup display_functions LCD Display Functions
 * @brief Contains page display handling functions
 *
 * @{
 */

enum head_type {MAIN, CONFIG, ALARM, PIN};						///<Defines all possible page sets

volatile uint8_t brightness;

uint8_t dtc_received;

typedef struct display_head_data
{
	enum head_type type;												///<Defines which page set the head points to
	uint8_t value;																///<Contains the current page number
}display_head_data;

display_head_data display_HEAD;										///<Points to a particular page


/**
 * @brief Adds main page labels to display pages
 *
 * Adds labels and units for all main display pages. These can be dynamically
 * changed if required.
 */
void display_initPages();

/**
 * @brief Initialises the display on power ON
 *
 * - Set Brightness to maximum.
 * - Display company logo
 * - Initialise main pages
 * - Switch display head to beginning
 */
void display_init();
/**
 * @brief Initialise and enable hardware PWM
 *
 * Initialises the hardware PWM used for backlight intensity control of the display.
 */
void display_initBrightness();

/**
 * @brief Sets display brightness
 *
 * Sets the hardware PWM duty to change the LCD backlight intensity
 *
 * @param brightness_arg Values between or equal to 0 and 255.
 */
void display_setBrightness(uint8_t brightness_arg);

/**
 * @brief ON INTERRUPT: Decrements the display brightness
 *
 * Decrements the hardware PWM duty by 51. This function is
 * called by an external interrupt
 */
void display_decrementBrightness();

/**
 * @brief ON INTERRUPT: Increments the display brightness
 *
 * Increments the hardware PWN duty by 51. This function is
 * called by an external interrupt
 */
void display_incrementBrightness();

/**
 * @brief Changes the head type to another page set
 *
 * Switches the head type to a different set of pages, and resets
 * its value to the first page of that page set.
 *
 * @param head_type Can take values MAIN, CONFIG, or ALARM.
 */
void display_switchHead(enum head_type);

/**
 * @brief Switches to the next page in the current page set
 */
void display_nextPage();

/**
 * @brief Switches to the previous page in the current page set
 */
void display_prevPage();

/**
 * @brief Draws a static page for the current page set
 *
 * Calls the drawStaticPage function from the page set referenced by
 * head type
 */
void display_drawStaticPage();

void display_drawDynamicPage(uint8_t parameter_position);

void display_PINIncorrect();

void display_PINCorrect();

///@}

#endif /* DISPLAY_DISPLAY_H_ */

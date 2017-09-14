/*
 * ConfigPage.h
 *
 *  Created on: 11-Aug-2016
 *      Author: piyush
 */

/**
 *	@file ConfigPage.h
 *	@author Piyush
 *	@brief Functions for config display pages
 *
 *	Contains data for configuration pages and the functions to manipulate these pages.
 *
 */

#ifndef DISPLAY_CONFIGPAGE_H_
#define DISPLAY_CONFIGPAGE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../EEPROM.h"
#include "../glcd/ks0108.h"
#include "../glcd/sanserif.h"
#include "../glcd/arial_bold_14.h"
#include "ReadingConverter.h"
#include "../inputoutput/ioHandler.h"
#include "../sensor_lib/TimerQueue.h"
#include "../sensor_lib/SensorsDAO.h"

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

/**
 *	\defgroup config_page Configuration Pages
 *	@brief Contains data and functions to access configuration pages
 *
 *	@{
 */

#define MIN_CONFIG		0		///<First config page will be at this position
#define MAX_CONFIG		0		///<Last config page will be at this position

#define MIN_PARAM		0		///<First paramater on a page
#define MAX_PARAM		1		///<Last paramater on a page

#define CONFIG_CONFIRM	250

uint8_t configpage_head;			///<Contains current config page number

uint8_t configparam_head;		///<Contains the current parameter on the page

uint8_t configpage_confirmation;

typedef struct config_data
{
	const char *title;					///<Title of the page
	const char *label1;				///<First parameter on screen
	const char *label2;				///<Second parameter on screen

	uint32_t	value1;						///<Value of first parameter
	uint32_t value2;						///<Value of second parameter

	uint32_t min1;						///<Minimum value of parameter 1
	uint32_t min2;						///<Minimum value of parameter 2

	uint32_t max1;						///<Maximum value of parameter 1
	uint32_t max2;						///<Maximum value of parameter 2

	uint16_t offset1;						///<Offset by which to change value1
	uint16_t offset2;						///<Offset by which to change value2

	uint16_t addr1;						///<EEPROM address of parameter 1
	uint16_t addr2;						///<EEPROM address of parameter 2

	const char *sensor1;				///<Sensor assigned to parameter 1
	const char *sensor2;				///<Sensor assigned to parameter 2

}config_data;

config_data config_page[MAX_CONFIG+1];

/**
 * @brief Resets the config page head to the first page
 *
 *	This will reset the config page head to the value of MIN_CONFIG.
 *	Which means that the current config page will be set to the first
 *	page of the config menu
 *
 *	@return Value of config head
 */
uint8_t configPage_resetHead();


/**
 * @brief Change to next config page
 *
 * Moves the config page head to the next page in the config menu.
 * If the current page is already the last page, the head is moved to
 * the first page.
 *
 * @return Value of config head
 */
uint8_t configPage_nextPage();

/**
 * @brief Change to the previous config page
 *
 * Moves the config page head to the previous page in the config menu.
 * If the current page is already the first page, the head is moved to
 * the last page.
 *
 * @return Value of config head
 */
uint8_t configPage_prevPage();

/**
 * @brief Draws static config page
 *
 * Draws the basic static layout of the main page. It also writes
 * the labels corresponding to the current page head.
 */
void configPage_drawStaticPage();

void configPage_drawDynamicPage(uint8_t position);

void configPage_setPageTitle (uint8_t page_no, const char *label);
void configPage_setPageLabel1(uint8_t page_no, const char *label);
void configPage_setPageLabel2(uint8_t page_no, const char *label);

void configPage_setSensor1(uint8_t page_no, const char *label);
void configPage_setSensor2(uint8_t page_no,const char *label);

void configPage_incrementValue();
void configPage_decrementValue();

void configPage_drawConfirmationPage();

void configPage_drawCursor();

void configPage_updateEEPROM();
///@}

#endif /* DISPLAY_CONFIGPAGE_H_ */

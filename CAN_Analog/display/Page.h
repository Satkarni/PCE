/*
 * Page.h
 *
 * Created: 8/2/2016 11:55:52 AM
 *  Author: Piyush
 */ 

/**
*	@file Page.h
*	@author Piyush
*	@brief Functions for main display pages
*	
*	Contains pages where readings will be shown on the controller.
*	All data displayed on the pages and access to the pages are
*	provided within this file.
*
*/
#ifndef PAGE_H_
#define PAGE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../event_handler/EventHandler.h"
#include "../glcd/ks0108.h"
#include "../glcd/sanserif.h"
#include "../glcd/arial_bold_14.h"
#include "ReadingConverter.h"

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

/**	\defgroup main_pages Main Pages
*	@brief Contains data and functions to access main display pages
*
*	@{
*/


#define		MIN_PAGE		0	///<First page will be at this position
#define		MAX_PAGE		4	///<Last page will be at this position

uint8_t page_head;				///<Contains current page number


typedef struct page_data
{
	const char *label1;					///<Label for left display parameter
	const char *label2;					///<Label for the right display parameter
	const char *unit1;					///<Unit for the left display parameter
	const char *unit2;					///<Unit for the right display parameter

	const char *sensor1;				///<Sensor to read for left display parameter
	const char *sensor2;				///<Sensor to read for right display parameter

	char value1[16];			///<Value of the left display parameter
	char value2[16];			///<Value of the right display parameter
	
}page_data;

page_data page[MAX_PAGE+1];

typedef enum
{
	STATUS_OK,
	STATUS_ALARM,
	STATUS_TRIP
}panel_status;

typedef enum
{
	STATUS_SAFETY_ENABLED,
	STATUS_SAFETY_DISABLED
}panel_safety;

panel_status page_current_status;
panel_safety page_current_safety;

/**
*	@brief Sets left display parameters for a page
*
*	This function lets you set the label and unit for the 
*	parameter to be displayed on the left side of the display
*	for a particular page.
*
*	@param page_no Page number for which to set label/unit
*	@param label Label text for the left parameter
*	@param unit	Unit text for the left parameter
*/
void page_setLabel1(uint8_t page_no, const char* label, const char* unit);

/**
*	@brief Sets right display parameters for a page
*
*	This function lets you set the label and unit for the
*	parameter to be displayed on the right side of the display
*	for a particular page.
*
*	@param page_no Page number for which to set label/unit
*	@param label Label text for the right parameter
*	@param unit	Unit text for the right parameter
*/
void page_setLabel2(uint8_t page_no, const char* label, const char* unit);

/**
 * @brief Sets sensor name for the left display parameter
 *
 * Name of the sensor should be the same as the sensor name
 * added to the sensor library. The readings from this sensor
 * will be shown on the left side of the page.
 *
 *	@param page_no Page number to which the sensor is to be assigned
 *	@param name Name of the sensor in the sensor library
 */
void page_setSensor1(uint8_t page_no, const char *name);


/**
 * @brief Sets sensor name for the right display parameter
 *
 * Name of the sensor should be the same as the sensor name
 * added to the sensor library. The readings from this sensor
 * will be shown on the right side of the page.
 *
 *	@param page_no Page number to which the sensor is to be assigned
 *	@param name Name of the sensor in the sensor library
 */
void page_setSensor2(uint8_t page_no, const char *name);

/**
*	@brief ABSTRACT: Resets the page head to first page
*	
*	This will reset the page head to the value of MIN_PAGE.
*	Which means that the current page will be set to the
*	first page.
*
*	@return Value of page_head (current page)
*/
uint8_t page_resetHead();

/**
*	@brief ABSTRACT: Change to next page
*
*	Moves the page head to the next page. If the current page
*	is already the last page, it will move the head to the first page.
*
*	@return Value of page_head (current page)
*/
uint8_t page_nextPage();

/**
*	@brief ABSTRACT: Change to next page
*
*	Moves the page head to the previous page. If the current page
*	is already the first page, it will move the head to the last page.

*	@return Value of page_head (current page)
*/
uint8_t page_prevPage();

/**
 * @brief Updates values for all sensors
 *
 * Will get the current values for all sensors. Will not, however,
 * put the values on the screen.
 */
void page_updateAllValues();

/**
 * @brief Updates the given value on screen
 *
 * Will update the value of the sensor passed as a parameter.
 * The function will call a value change event if the sensor value
 * updated is currently being displayed on screen, otherwise, it
 * will simply update the value of the sensor internally. This function
 * is usually called whenever the value of a sensor changes.
 *
 * @param Name of the sensor to be updated.
 */
void page_updateValue(const char *sensorName);

/**
 * @brief Draws static main page
 *
 * Draws the basic static layout of the main page. It also writes
 * the labels and units corresponding to the current page head.
 */
void page_drawStaticPage();

/**
 * @brief Draws the dynamic elements of the main page.
 *
 * Draws dynamic elements such as labels, values and units
 * related to the readings to be displayed on screen. This function
 * will draw different elements depending on the current value of
 * the page HEAD.
 */
void page_drawDynamicPage(uint8_t parameter_position);

/**
 * @brief Draws/redraws the controller status
 *
 * Draws or redraws the current status of the controller.
 */
void page_drawStatus();
void page_drawSafety();

///@}

#endif /* PAGE_H_ */

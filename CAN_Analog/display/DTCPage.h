/*
 * DTCPage.h
 *
 *  Created on: 19-Sep-2016
 *      Author: piyush
 */

#ifndef DISPLAY_DTCPAGE_H_
#define DISPLAY_DTCPAGE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../event_handler/EventHandler.h"
#include "../glcd/ks0108.h"
#include "../glcd/sanserif.h"
#include "../glcd/arial_bold_14.h"

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

uint8_t dtcpage_head;

typedef struct dtcPage_data
{
	uint32_t spn;
	uint8_t	fmi;
	uint8_t occ;
}dtcPage_data;

dtcPage_data dtcpage[100];

uint8_t max_dtc;

void dtcPage_generatePages();

uint8_t dtcPage_nextPage();
uint8_t dtcPage_prevPage();
uint8_t dtcPage_resetHead();
void dtcPage_drawStaticPage();


#endif /* DISPLAY_DTCPAGE_H_ */

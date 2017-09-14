/*
 * RTCHandler.h
 *
 *  Created on: 20-Sep-2016
 *      Author: piyush
 */

#ifndef RTC_RTCHANDLER_H_
#define RTC_RTCHANDLER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c_ds1307.h"


struct rtc
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
};

/* DS1307 basic functions */
void ds1307Init(void);
void ds1307Setdate(uint8_t seconds,uint8_t minutes, \
				   uint8_t hours, uint8_t day, uint8_t date, \
				   uint8_t month,uint8_t year);
void ds1307Getdate(struct rtc *data);
uint8_t ds1307Dectobcd(uint8_t data);
uint8_t ds1307Bcdtodec(uint8_t data);
void ds1307UpdateTimerInit(void);
void ds1307UpdateTimerEnable(uint16_t rate);



#endif /* RTC_RTCHANDLER_H_ */

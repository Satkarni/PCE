/*
 * RTCHandler.c
 *
 *  Created on: 20-Sep-2016
 *      Author: piyush
 */

#include "RTCHandler.h"

void ds1307Init(void)
{
	uint8_t twbr = I2C_TWBR;
	i2cInit(twbr);
}
void ds1307Setdate(uint8_t seconds,uint8_t minutes, \
				   uint8_t hours, uint8_t day, uint8_t date, \
				   uint8_t month,uint8_t year)
{
	uint8_t bcdSeconds = ds1307Dectobcd(seconds);
	uint8_t bcdMinutes = ds1307Dectobcd(minutes);
	uint8_t bcdHours = ds1307Dectobcd(hours);
	uint8_t bcdDate = ds1307Dectobcd(date);
	uint8_t bcdMonth = ds1307Dectobcd(month);
	uint8_t bcdYear = ds1307Dectobcd(year);

	i2cStart(I2C_ADDR_DS1307|I2C_WRITE);
	i2cWrite(0x00);
	i2cWrite(bcdSeconds);
	i2cWrite(bcdMinutes);
	i2cWrite(bcdHours);
	i2cWrite(day);
	i2cWrite(bcdDate);
	i2cWrite(bcdMonth);
	i2cWrite(bcdYear);
	i2cWrite(0x00);
	i2cStop();
}
void ds1307Getdate(struct rtc *data)
{
	i2cStart(I2C_ADDR_DS1307|I2C_WRITE);
	i2cWrite(0x00);
	i2cStop();

	i2cRepeatStart(I2C_ADDR_DS1307|I2C_READ);
	data->seconds = ds1307Bcdtodec(i2cReadAck() & 0x7F);
	data->minutes = ds1307Bcdtodec(i2cReadAck());
	data->hours = ds1307Bcdtodec(i2cReadAck());
	i2cReadAck();
	data->date = ds1307Bcdtodec(i2cReadAck());
	data->month = ds1307Bcdtodec(i2cReadAck());
	data->year = ds1307Bcdtodec(i2cReadNack());
	i2cStop();

}
uint8_t ds1307Dectobcd(uint8_t data)
{
	return ( (data/10)*16 + (data%10) );
}
uint8_t ds1307Bcdtodec(uint8_t data)
{
	return ( (data>>4)*10 + (data&0x0F) );
}


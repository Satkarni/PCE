/*
 * i2c_ds1307.h
 *
 * Created: 11/08/2016 11:14:34
 *  Author: ROHANP
 
 
 I2C routines for interfacing AT90CAN128 with DS1307 RTC(Real Time Clock). 
 AT90CAN128 -> I2C Master
 DS1307 -> I2C Slave
 
 */ 


#ifndef RTC_I2C_DS1307_H_
#define RTC_I2C_DS1307_H_

#include <avr/io.h>
#include <util/delay.h>
#include "../Hardware.h"

#define DS1307_12HR ( 6 )
#define DS1307_AM	( 0 )
#define DS1307_PM	( 1 )

#define	SUNDAY		( 1 )
#define MONDAY		( 2 )
#define TUESDAY		( 3 )
#define WEDNESDAY	( 4 )
#define THURSDAY	( 5 )
#define FRIDAY		( 6 )
#define SATURDAY	( 7 )


/* I2C Clock Generation */
#define I2C_SCK				( 100000UL )	//100 KHz
#define I2C_TWBR			(uint8_t)( ((F_CPU/I2C_SCK) - 16)/2 )

/* I2C Addresses */
#define I2C_ADDR_GENERAL	( 0x00 )
#define I2C_ADDR_AT90CAN128	( 0x0A )
#define I2C_ADDR_DS1307		( 0xD0 )
#define I2C_WRITE			( 0x00 )
#define I2C_READ			( 0x01 )

/* Status Codes  */
#define I2C_START			( 0x08 )
#define I2C_REP_START		( 0x10 )
#define I2C_SLAW_ACK		( 0x18 )
#define I2C_SLAW_NACK		( 0x20 )
#define I2C_DATAW_ACK		( 0x28 )
#define I2C_DATAW_NACK		( 0x30 )
#define I2C_ARBT_LOST		( 0x38 )				
#define I2C_SLAR_ACK		( 0x40 )
#define I2C_SLAR_NACK		( 0x48 )
#define I2C_DATAR_ACK		( 0x50 )
#define I2C_DATAR_NACK		( 0x58 )
#define I2C_NO_INFO			( 0xF8 )
#define I2C_BUS_ERROR		( 0x00 )

#define TWINT_FLAG_SET		( TWCR & (1<<TWINT) )
#define I2C_STATUS			( TWSR & 0xF8 ) 



/* I2C basic functions */
void i2cInit(uint8_t twbr);
void i2cStart(uint8_t slaveAddress);
void i2cRepeatStart(uint8_t slaveAddress);
void i2cStop(void);
void i2cWrite(uint8_t data);
uint8_t i2cReadAck(void);
uint8_t i2cReadNack(void);
void i2cError(void);


#endif /* RTC_I2C_DS1307_H_ */

/*
 * i2c_ds1307.c
 *
 * Created: 11/08/2016 11:14:17
 *  Author: ROHANP
 */ 

#include "i2c_ds1307.h"


void i2cInit(uint8_t twbr)
{
	TWSR = 0x00;	//No prescaler
	TWBR = twbr;
}
void i2cStart(uint8_t slaveAddress)
{
	TWCR = ( (1<<TWINT)|(1<<TWSTA)|(1<<TWEN) );
	while(!TWINT_FLAG_SET);
	
	if( I2C_STATUS != I2C_START )	
		i2cError();
	
	TWDR = slaveAddress;
	TWCR = ( (1<<TWINT)|(1<<TWEN) );
	while(!TWINT_FLAG_SET);
	
	if( (I2C_STATUS != I2C_SLAW_ACK) && (I2C_STATUS != I2C_SLAR_ACK) ) 
		i2cError();
}
void i2cRepeatStart(uint8_t slaveAddress)
{
	i2cStart(slaveAddress);
}
void i2cStop(void)
{
	TWCR = ( (1<<TWINT)|(1<<TWSTO)|(1<<TWEN) );
	while( (TWCR & (1<<TWSTO)) );
}
void i2cWrite(uint8_t data)
{
	TWDR = data;
	TWCR = ( (1<<TWINT)|(1<<TWEN) );
	
	while(!TWINT_FLAG_SET);
	
	if(I2C_STATUS != I2C_DATAW_ACK)	i2cError();
}
uint8_t i2cReadAck(void)
{
	TWCR = ( (1<<TWINT)|(1<<TWEN)|(1<<TWEA) );
	while(!TWINT_FLAG_SET);
	return TWDR;
}
uint8_t i2cReadNack(void)
{
	TWCR = ( (1<<TWINT)|(1<<TWEN) );
	while(!TWINT_FLAG_SET);
	return TWDR;
}
void i2cError(void)
{
	// This will be used later for debugging
	debugLedAltblink(2);
}


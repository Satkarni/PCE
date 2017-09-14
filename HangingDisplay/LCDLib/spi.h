/*
 * spi.h
 *
 * Created: 8/9/2016 12:25:38 PM
 *  Author: Piyush
 */ 

/*
*	@file spi.h
*	@brief SPI Interface functions
*	Author: Rohan Pandit, Piyush
*	Contains basic SPI functions for initialising and data transfer
*/

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <util/delay.h>

#define		PIN_SS		PB2
#define		PIN_MOSI	PB3
#define		PIN_SCK		PB5

#define		PIN_CS1		PC2
#define		PIN_CS2		PC3
#define		PIN_CS3		PC4
#define		PIN_CS4		PC5

#define CS1_HIGH		( PORTC |= (1<<PIN_CS1) )
#define CS1_LOW			( PORTC &= ~(1<<PIN_CS1) ) 

#define CS2_HIGH		( PORTC |= (1<<PIN_CS2) )
#define CS2_LOW			( PORTC &= ~(1<<PIN_CS2) )

#define CS3_HIGH		( PORTC |= (1<<PIN_CS3) )
#define CS3_LOW			( PORTC &= ~(1<<PIN_CS3) )

#define CS4_HIGH		( PORTC |= (1<<PIN_CS4) )
#define CS4_LOW			( PORTC &= ~(1<<PIN_CS4) )

/*	\defgroup spi SPI Functions
*	@brief Contains SPI init and transceive functions
*
*	@{
*/

/**
*	@brief Initialises the SPI Interface on AT90CAN 
*/
void spi_init();

/**
*	@brief Sends and Receives data from SPI Interface
*	@param databyte Data to send over the SPI Interface
*	@return Data received from the SPI Interface
*/
uint8_t spi_tranceive(uint8_t databyte);

///@}

#endif /* SPI_H_ */

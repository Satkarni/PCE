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

#define PIN_SS	 PB0
#define PIN_MOSI PB2
#define PIN_SCK  PB1

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

#define AD420_CS		PIN_SS
#define AD420_CS_LOW	( PORTB &= ~(1<<PIN_SS) )
#define AD420_CS_HIGH	( PORTB |= (1<<PIN_SS) )

void AD420_Calibrate(void);
void AD420_Setcurrent(uint16_t current);

#endif /* SPI_H_ */
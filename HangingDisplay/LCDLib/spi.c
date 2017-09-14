/*
 * spi.c
 *
 * Created: 8/9/2016 12:25:25 PM
 *  Author: Piyush
 */ 
#include "spi.h"

void spi_init()
{
	DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1<<PIN_SS); //Chip select
	PORTB &= ~(1<<PIN_SS);

	DDRC |= (1<<PIN_CS1) | (1<<PIN_CS2)| (1<<PIN_CS3)| (1<<PIN_CS4);
	PORTC &= ~(1<<PIN_CS1);
	
	// SPI Enable, Master mode, f_cpu/128
	SPCR |= (1 << SPE) | (1 << MSTR)|(1<<SPR1)|(1<<SPR0);
	
}

uint8_t spi_tranceive(uint8_t databyte)
{
	// Copy data into the SPI data register
	SPDR = databyte;
	_delay_us(1);
	// Wait until transfer is complete
	while (!(SPSR & (1 << SPIF))){	};
	return SPDR;
}

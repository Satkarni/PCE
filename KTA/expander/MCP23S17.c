/*
 * MCP23S17.c
 *
 * Created: 8/4/2016 2:07:09 PM
 *  Author: Piyush
 */ 

#include "MCP23S17.h"

void MCP23S17_init()
{
	DDRE |= (1<<PE3) | (0<<PE4) | (0<<PE5);		//Configuring KEYCOM pin as output, INTA and INTB as input
	PORTE |= (1<<PE3) | (1<<PE4) | (1<<PE5);		//Configuring KEYCOM pin as high

	DDRG |= (1<<PG4);	//RESET Pin of MCP23S17 driven by AT90CAN set as output
	PORTG |= (1<<PG4);	//RESET Pin of MCP23S17 set to high (inverse polarity)

	uint8_t iocon_register = 0;
	iocon_register |= (0<<IOCON_BANK) | (1<<IOCON_SEQOP) | (1<<IOCON_HAEN) | (1<<IOCON_ODR) | (0<<IOCON_INTPOL);
	MCP23S17_write(IOCONA,iocon_register);
	MCP23S17_write(IOCONB,iocon_register);

	MCP23S17_setDirectionB(OUTPUT);
	MCP23S17_write(GPIOB,0x00);
	//MCP23S17_write(GPINTENB,ENABLE);

	MCP23S17_setDirectionA(INPUT);
	MCP23S17_enablePullUpA(ENABLE);
	MCP23S17_write(GPINTENA,ENABLE);
	MCP23S17_write(DEFVALA,ENABLE);
	MCP23S17_write(INTCONA,ENABLE);

	for(uint8_t i=0;i<4;i++)
	{
		MCP23S17_read(GPIOA);
		MCP23S17_read(GPIOB);
		_delay_us(10);
	}
}

void MCP23S17_write(uint8_t register_address, uint8_t data)
{
	MCP23S17_select();
	spi_tranceive(OPCODE_WRITE);
	_delay_us(1);
	spi_tranceive(register_address);
	_delay_us(1);
	spi_tranceive(data);
	_delay_us(1);
	MCP23S17_deselect();
}

uint8_t MCP23S17_read(uint8_t register_address)
{
	uint8_t data;
	MCP23S17_select();
	spi_tranceive(OPCODE_READ);
	_delay_us(1);
	spi_tranceive(register_address);
	_delay_us(1);
	data = spi_tranceive(0x00);
	MCP23S17_deselect();
	return data;
}

void MCP23S17_setDirectionA(uint8_t direction)
{
	MCP23S17_select();
	MCP23S17_write(IODIRA,direction);
	MCP23S17_deselect();
}

void MCP23S17_setDirectionB(uint8_t direction)
{
	MCP23S17_select();
	MCP23S17_write(IODIRB,direction);
	MCP23S17_deselect();
}

void MCP23SI7_enableInterruptA(uint8_t enable)
{
	MCP23S17_select();
	MCP23S17_write(GPINTENA,enable);
	MCP23S17_deselect();
}

void MCP23S17_enableInterruptB(uint8_t enable)
{
	MCP23S17_select();
	MCP23S17_write(GPINTENB,enable);
	MCP23S17_deselect();
}

void MCP23S17_enablePullUpA(uint8_t enable)
{
	MCP23S17_select();
	MCP23S17_write(GPPUA,enable);
	MCP23S17_deselect();
}

void MCP23S17_enablePullUpB(uint8_t enable)
{
	MCP23S17_select();
	MCP23S17_write(GPPUB,enable);
	MCP23S17_deselect();
}

void MCP23S17_select()
{
	PORTB &= ~(1<<PB6);
	_delay_us(1);
}

void MCP23S17_deselect()
{
	PORTB |= (1<<PB6);
}

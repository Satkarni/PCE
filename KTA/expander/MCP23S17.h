/*
 * MCP23S17.h
 *
 * Created: 8/4/2016 2:06:53 PM
 * Author: Piyush
 */ 

/**
*	@file MCP23S17.h
*	@brief Contains Register addresses and Functions for addressing MCP23S17 registers
*	
*	Author: Piyush
*/


#ifndef MCP23S17_H_
#define MCP23S17_H_

#include <avr/interrupt.h>
#include "../Hardware.h"
#include "../spi.h"

/**\defgroup MCP_registers MCP Register Addresses
*	Register Definitions for MCP23S17
*	@{
*/

#define IODIRA			0x00		///<Data Direction Register for PORTA
#define IODIRB			0x01		///<Data Direction Register for PORTB
#define IPOLA			0x02		///<Input Polarity Register for PORTA
#define IPOLB			0x03		///<Input Polarity Register for PORTB
#define GPINTENA		0x04		///<Interrupt-on-change enable Register for PORTA
#define GPINTENB		0x05		///<Interrupt-on-change enable Register for PORTB
#define DEFVALA			0x06		///<Default Value Register for PORTA
#define DEFVALB			0x07		///<Default Value Register for PORTB
#define INTCONA			0x08		///<Interrupt-on-change control Register for PORTA
#define INTCONB			0x09		///<Interrupt-on-change control Register for PORTB
#define IOCONA			0x0A		///<Configuration register for PORTA
#define IOCONB			0x0B		///<Configuration register for PORTB
#define GPPUA			0x0C		///<100kOhm pullup resistor register for PORTA (sets pin to input when set)
#define GPPUB			0x0D		///<100kOhm pullup resistor register for PORTB (sets pin to input when set)
#define INTFA			0x0E		///<Interrupt flag Register for PORTA
#define INTFB			0x0F		///<Interrupt flag Register for PORTB
#define INTCAPA			0x10		///<Interrupt captured value Register for PORTA
#define INTCAPB			0x11		///<Interrupt captured value Register for PORTB
#define GPIOA			0x12		///<General purpose I/O Register for PORTA
#define GPIOB			0x13		///<General purpose I/O Register for PORTB
#define OLATA			0x14		///<Output latch Register for PORTA
#define OLATB			0x15		///<Output latch Register for PORTB

#define IOCON_INTPOL	0x01
#define IOCON_ODR		0x02
#define IOCON_HAEN		0x03
#define IOCON_DISSLW	0x04
#define IOCON_SEQOP		0x05
#define IOCON_MIRROR	0x06
#define IOCON_BANK		0x07

#define OPCODE_WRITE	0x40		///<OPCODE for sending data
#define OPCODE_READ		0x41		///<OPCODE for receiving data

#define OUTPUT			0x00		///<Output Direction
#define INPUT			0xFF		///<Input Direction

#define ENABLE			0xFF		///<Parameter call for ENABLE
#define DISABLE			0x00		///<Parameter call for DISABLE
/// @}

/**\defgroup MCP_read_write MCP Read Write Functions
*	Read and Write functions for MCP23S17
*	@{
*/

/**
*	@brief Writes data to a MCP23S17 register via SPI
*	@param register_address Address of the MCP23S17 register to write
*	@param data 1 byte data to write to the register
*/
void MCP23S17_write(uint8_t register_address, uint8_t data);

/**
*	@brief Reads data from a MCP23S17 register via SPI
*	@param register_address Address of the MCP23S17 register to read from
*/
uint8_t MCP23S17_read(uint8_t register_address);

/**
*	@}
*/

/**\defgroup MCP23S17_config MCP Configuration Functions
*	Configuration functions for MCP23S17
*	@{
*/

/**
*	@brief Set Direction of Port A
*	@param direction Set to INPUT or OUTPUT
*/
void MCP23S17_setDirectionA(uint8_t direction);

/**
*	@brief Set Direction of Port B
*	@param direction Set to INPUT or OUTPUT
*/
void MCP23S17_setDirectionB(uint8_t direction);


/**
*	@brief Enable Interrupt on Change for Port A
*	@param enable Set to ENABLE or DISABLE
*/
void MCP23SI7_enableInterruptA(uint8_t enable);

/**
*	@brief Enable Interrupt on Change for Port B
*	@param enable Set to ENABLE or DISABLE
*/
void MCP23S17_enableInterruptB(uint8_t enable);


/**
*	@brief Enable Pull Up for Port A
*	@param enable Set to ENABLE or DISABLE
*/
void MCP23S17_enablePullUpA(uint8_t enable);

/**
*	@brief Enable Pull Up for Port B
*	@param enable Set to ENABLE or DISABLE
*/
void MCP23S17_enablePullUpB(uint8_t enable);


void MCP23S17_select();
void MCP23S17_deselect();

void MCP23S17_init();
#endif /* MCP23S17_H_ */
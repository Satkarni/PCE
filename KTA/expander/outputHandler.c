/*
 * outputHandler.c
 *
 * Created: 8/9/2016 3:07:07 PM
 *  Author: Piyush
 */ 
#include "outputHandler.h"
#include "../Hardware.h"

void enablePowerLED()
{
	uint8_t current_data, new_data;
	current_data = MCP23S17_read(GPIOB);
	new_data = current_data | (1<<POWER_LED);
	MCP23S17_write(GPIOB,new_data);
	MCP23S17_write(GPIOB,new_data);
	MCP23S17_write(GPIOB,new_data);
}

void disablePowerLED()
{
	DEBUG_LED2_LD9_TOGGLE;
	uint8_t current_data, new_data;
	current_data = MCP23S17_read(GPIOB);
	new_data = current_data & ~(1<<POWER_LED);
	MCP23S17_write(GPIOB,new_data);
}

void togglePowerLED()
{
	uint8_t current_data, new_data;
	current_data = MCP23S17_read(GPIOB);
	new_data = current_data ^ (1<<POWER_LED);
	MCP23S17_write(GPIOB,new_data);
}

void enableFaultLED()
{
	uint8_t current_data, new_data;
	current_data = MCP23S17_read(GPIOB);
	new_data = current_data | (1<<FAULT_LED);
	MCP23S17_write(GPIOB,new_data);
}

void disableFaultLED()
{
	uint8_t current_data, new_data;
	current_data = MCP23S17_read(GPIOB);
	new_data = current_data & ~(1<<FAULT_LED);
	MCP23S17_write(GPIOB,new_data);
}

void toggleFaultLED()
{
	uint8_t current_data, new_data;
	current_data = MCP23S17_read(GPIOB);
	new_data = current_data ^ (1<<FAULT_LED);
	MCP23S17_write(GPIOB,new_data);
}

ISR(INT5_vect)
{

}

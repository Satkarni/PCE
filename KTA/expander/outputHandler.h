/*
 * outputHandler.h
 *
 * Created: 8/9/2016 3:07:24 PM
 *  Author: Piyush
 */ 

/**
*	@file outputHandler.h
*	@brief Output functions for MCP23S17 (LED Output)
*	
*	Author: Piyush
*/

#ifndef OUTPUTHANDLER_H_
#define OUTPUTHANDLER_H_

#include "MCP23S17.h"

/**\defgroup output_handler Output Handler
*	@brief Defines functions for using LEDs
*
*	@{
*/

#define POWER_LED	0x01	///<Address for Power LED
#define FAULT_LED	0x02	///<Address for Fault LED


void enablePowerLED();		///<Turn ON Power LED
void disablePowerLED();		///<Turn OFF Power LED
void togglePowerLED();		///<Toggle current state of Power LED

void enableFaultLED();		///<Turn ON Fault LED
void disableFaultLED();		///<Turn OFF Fault LED
void toggleFaultLED();		///<Toggle current state of Fault LED

///@}

#endif /* OUTPUTHANDLER_H_ */
/*
 * PCE_CAN.h
 *
 * Created: 8/2/2016 11:20:37 AM
 *  Author: Piyush
 */ 
/**
 * @file PCE_CAN.h
 * @author Piyush
 */

#ifndef PCECAN_H_
#define PCECAN_H_

//#define TESTMODE

#include "inputoutput/ioHandler.h"
#include "inputoutput/ExternalInterrupts.h"
#include "can_scanner/CANHandler.h"
#include "sensor_lib/SensorsDAO.h"
#include "display/Display.h"
#include "event_handler/EventHandler.h"
#include "AnalogScanner.h"
#include "UnitTests/Test.h"
/**
 * @brief Initialises the controller
 *
 * Initialises display and other controller components
 */
void init();
void checkSensors();


#endif /* MAIN_H_ */

/*
 * AnalogScanner.h
 *
 *  Created on: 11-Mar-2017
 *      Author: piyush
 */

/**
 * @file AnalogScanner.h
 * @author Piyush
 * @brief Contains logic for handling analog sensors
 **/

#ifndef ANALOGSCANNER_H_
#define ANALOGSCANNER_H_

#include <avr/io.h>

#include "EEPROM.h"
#include "ADC.h"
#include "display/Display.h"

#define		Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define		Leave_Critical_Section		SREG = sreg_; } while (0);

#include "sensor_lib/SensorsDAO.h"
#include "sensor_lib/SensorHandler.h"

#include "SetPoints.h"


void analogScanner_init();
void analogScanner_initSensors();

void analogScanner_updateReading(const char *sensorName, uint32_t value);
void analogScanner_scan();




#endif /* ANALOGSCANNER_H_ */

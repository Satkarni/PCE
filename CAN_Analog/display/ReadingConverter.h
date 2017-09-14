/*
 * ReadingConverter.h
 *
 *  Created on: 17-Aug-2016
 *      Author: piyush
 */

/**
 * @file ReadingConverter.h
 * @author Piyush
 *
 * Contains functions to convert the readings of existing
 * sensors to their real values.
 */

#ifndef DISPLAY_READINGCONVERTER_H_
#define DISPLAY_READINGCONVERTER_H_

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../sensor_lib/SensorsDAO.h"
#include "../sensor_lib/SensorHandler.h"
#include "../inputoutput/ioHandler.h"

/**
 * @brief Gets the real value of existing sensor
 *
 * Note that the sensor must exist to get
 * the appropriate value.
 *
 * @param name Name of the sensor
 * @param rawValue Raw sensor reading
 * @param realValue Pointer to array where real value is to be written
 *
 */
void readingConverter_getRealValue(char *name, uint32_t rawValue, char* realValue);

#endif /* DISPLAY_READINGCONVERTER_H_ */

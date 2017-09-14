/*
 * Converter.c
 *
 *  Created on: 01-Mar-2017
 *      Author: piyush
 */

#include "Converter.h"

double bin32ToDouble(uint32_t binaryNumber)
{
	uint32_t raw = binaryNumber;
	uint32_t mantissa;
	uint8_t exponent;
	int8_t sign;

	sign = (raw >> 31 == 0) ? 1 : -1;
	mantissa = (raw & 0x7FFFFF) | 0x800000;
	exponent = ((raw >> 23) & 0xFF) - 127 - 23;

	return (sign*mantissa*pow(2,exponent));
}

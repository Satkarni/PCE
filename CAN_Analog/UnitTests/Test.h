/*
 * Test.h
 *
 *  Created on: 14-Mar-2017
 *      Author: rohan
 */

#ifndef UNITTESTS_TEST_H_
#define UNITTESTS_TEST_H_

#include "../PCE_CAN.h"

void test();

void simulBlink(uint8_t count);
void altBlink(uint8_t count);
void LEDTestSetup();
void LEDTestSetdown();

#endif /* UNITTESTS_TEST_H_ */

/*
 * Calibration.h
 *
 *  Created on: 26-Oct-2016
 *      Author: piyush
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "expander/inputHandler.h"
#include "DisplayInterface.h"
#include "ADC.h"
#include "avr/wdt.h"

#define OVERSPEED_THRESHOLD	2250
#define RUN_THRESHOLD				600
#define ECT_THRESHOLD				95
#define LOP_THRESHOLD				1.0
#define AMM_THRESHOLD				5
#define PULLEY_RATIO					2.95

uint16_t bufferAddress;

void calibrationsInit();
void calibratePulleyRatio(void);
void calibrateThrottle();
void calibrateActuator();
void toggleAlarms();
void adjustSetPoints();
void incrementEngineSeconds();

uint16_t getPulleyRatio();
uint16_t getOVSThreshold();
uint16_t getRunThreshold();
uint16_t getECTThreshold();
uint16_t getLOPThreshold();
uint16_t getAMMThreshold();
uint16_t getEngineHours();
uint16_t getEngineSeconds();
uint8_t isEnabledOVSAlarm();
uint8_t isEnabledECTAlarm();
uint8_t isEnabledLOPAlarm();
uint8_t isEnabledAMMAlarm();

void factoryReset();



#endif /* CALIBRATION_H_ */

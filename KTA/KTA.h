/*
 * J1939.h
 *
 * Created: 08-08-2016 13:28:48
 *  Author: ROHANP
 */ 


#ifndef KTA_H_
#define KTA_H_

#include <avr/io.h>
#include <util/delay.h>
#include "DisplayInterface.h"
#include "sensor_lib/SensorsDAO.h"
#include "sensor_lib/SensorHandler.h"
#include "sensor_lib/TimerQueue.h"
#include "Hardware.h"
#include "expander/MCP23S17.h"
#include "expander/outputHandler.h"
#include "expander/inputHandler.h"
#include "RTC/RTCHandler.h"
#include "Timers.h"
#include "RS232.h"
#include "Calibration.h"


#define ECT_MIN	30
#define ECT_MAX	789

#define LOP_MIN	140
#define LOP_MAX	700

#define AMM_MIN	0
#define AMM_MAX	1024

#define ACTOUT_MIN_PWM		16
#define ACTIN_MIN_PWM		25

volatile uint8_t oneSecondFlag;

volatile uint16_t counter;

uint16_t ammeterOffset;

void initSensors();
void clockSensors();
void updateDateTime();
void updateRPMSensor();
void updateSensors();
void checkActuator();
void init();

#endif /* KTA_H_ */

/*
 * ExternalInterrupts.h
 *
 * Created: 8/2/2016 11:22:25 AM
 *  Author: Piyush
 */ 

/**
*	@file ExternalInterrupts.h
*	@author Rohan, Piyush
*	@brief Handles all external interrupts
*
*	All interrupts such as key presses and timers are handled by this file
*/


#ifndef EXTERNALINTERRUPTS_H_
#define EXTERNALINTERRUPTS_H_

#define TIMER1_RATE	( 15625 )				///<250ms Interrupt

#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "../display/Display.h"
#include "../event_handler/EventHandler.h"
#include "../inputoutput/ioHandler.h"
#include "../sensor_lib/HardwareTimer.h"
#include "../sensor_lib/TimerQueue.h"
#include "../sensor_lib/SensorHandler.h"

volatile uint8_t timer1sFlag, timerIntFlag;

void Ext_Int_init();
void initHardwareTimer1(void);
void enableHardwareTimer1(uint16_t rate);

#endif /* EXTERNALINTERRUPTS_H_ */

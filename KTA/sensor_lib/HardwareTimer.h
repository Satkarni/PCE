/*
 * HardwareTimer.h
 *
 *  Created on: 18-Jul-2016
 *      Author: piyush
 */


/**
 * @file HardwareTimer.h
 * @brief STUB: Contains function for hardware timer
 *
 * This function is a stub for implementing hardware timers.
 */
#ifndef HARDWARETIMER_H_
#define HARDWARETIMER_H_

#include "TimerQueue.h"
#include "SensorHandler.h"


/**
 * @brief Call at Hardware Timer Interrupt
 *
 * This function will perform all operations required in one
 * clock cycle of the timer queue. It should ideally be called
 * at a hardware timer interrupt.
 */
void timerTick()
{
	recheckTimerQueue();
	clockTimerQueue();
	processSensors();
}

#endif /* HARDWARETIMER_H_ */

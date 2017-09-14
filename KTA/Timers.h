/*
 * Timers.h
 *
 *  Created on: 26-Sep-2016
 *      Author: piyush
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Hardware.h"
#define TIMER2_RATE 125


double duty_cycle;

void initTimers();
void Timers_timer2Init(void);
void Timers_timer2Enable(uint16_t rate);
void Timers_RPMTimerInit(void);
void Timers_RPMTimerEnable(void);
void Timers_RPMTimerDisable(void);
void Timers_PWMInit();
void Timers_setDutyCycle(uint16_t duty_cycle_percentage);
void Timers_PWMEnable();
void Timers_PWMDisable();

#endif /* TIMERS_H_ */

/*
 * Modbus.h
 *
 *  Created on: 03-Mar-2017
 *      Author: piyush
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void enable_timer0_35char(uint8_t frame_ch_init);
void disable_timer0_35char();



#endif /* MODBUS_H_ */

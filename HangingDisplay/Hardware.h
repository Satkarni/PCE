/*
 * Hardware.h
 *
 *  Created on: 01-Feb-2017
 *      Author: piyush
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#define SLAVE
#define MASTER
//#define TEST_ECHO

/****************Standard Fuse bytes**********************
Low fuse:		0xFF
High fuse:		0xD1
Extended fuse:	0xFC
**********************************************************/
#define DEBUG_LED	PC0

#define DEBUG_LED_ON		( PORTC |= (1<<DEBUG_LED) )
#define DEBUG_LED_OFF		( PORTC &= ~(1<<DEBUG_LED) )
#define DEBUG_LED_TOGGLE	( PORTC ^= (1<<DEBUG_LED) )

#define		DIP1		PD5
#define 	DIP2		PD6
#define		DIP3		PD7
#define		DIP4		PB0


#define DIP1_ON		( !(PIND&(1<<DIP1)) )
#define DIP1_OFF	( PIND&(1<<DIP1) )
#define DIP2_ON		( !(PIND&(1<<DIP2)) )
#define DIP2_OFF	( PIND&(1<<DIP2) )
#define DIP3_ON		( !(PIND&(1<<DIP3)) )
#define DIP3_OFF 	( PIND&(1<<DIP3) )
#define DIP4_ON		( !(PINB&(1<<DIP4)) )
#define DIP4_OFF	( PINB&(1<<DIP4) )

void blink(uint8_t);

#endif /* HARDWARE_H_ */

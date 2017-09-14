/*
 * RS485.h
 *
 *  Created on: 24-Aug-2016
 *      Author: piyush
 */

#ifndef RS485_H_
#define RS485_H_

#include <avr/io.h>

#define BAUD_485 19200
#define BAUDRATE_485 ((F_CPU)/(BAUD_485*16UL)-1)    // set baudrate value for UBRR

#define RTS_PIN PG3
#define RTS_DIR DDRG
#define RTS_PORT PORTG

void RS485_init(uint16_t ubrr_value);
void RS485_enable(uint8_t rxenable,uint8_t txenable);
uint8_t RS485_readchar();
void RS485_writechar(uint8_t data);
void RS485_writeString(char *data);

#endif /* RS485_H_ */

/*
 * RS485.h
 *
 *  Created on: 24-Aug-2016
 *      Author: piyush
 */

#ifndef RS485_H_
#define RS485_H_

#include <avr/io.h>

#define BAUD_485 19200UL
#define BAUDRATE_485 ((F_CPU)/(BAUD_485*16UL)-1)    // set baudrate value for UBRR

#define RTS_PIN PD2
#define RTS_DIR DDRD
#define RTS_PORT PORTD

#define EVEN_PARITY

void RS485_init(uint16_t ubrr_value);
void RS485_enable(uint8_t rxenable,uint8_t txenable);
uint8_t RS485_readchar();
void RS485_writechar(uint8_t data);
void RS485_writeString(char *data);
uint16_t getCRC(uint8_t *crc_p, uint8_t length);
uint8_t checkCRC(uint8_t *buffer,uint8_t length);

#endif /* RS485_H_ */

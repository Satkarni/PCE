/*
 * RS485.h
 *
 * Created: 13-07-2015 16:14:20
 *  Author: ADMIN
 */ 


#ifndef RS485_H_
#define RS485_H_

#include <avr/io.h>

#define BAUD 19200
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)    // set baudrate value for UBRR

#define RTS_PIN PORTD2
#define RTS_DIR DDRD
#define RTS_PORT PORTD

void USART_enable(uint8_t rxenable,uint8_t txenable);
void USART_init(uint16_t ubrr_value);
uint8_t USART_readchar();
void USART_writechar(uint8_t data);
uint16_t get_CRC(volatile uint8_t *crc_p, uint8_t length);

#endif /* RS485_H_ */
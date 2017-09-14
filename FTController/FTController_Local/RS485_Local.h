/*! \file RS485.h
 * RS485.h
 *
 * Created: 13-07-2015 16:14:20
 *  Author: ADMIN
 */


#ifndef RS485_H_
#define RS485_H_

#include <avr/io.h>

/*!
  \def BAUD
  \brief Sets the baudrate for RS485

  Note that you must add a UL at the end so that compiler will not truncate the value to a 8 bit integer\n
  Example:\n
  To set a baud rate of 9600\n
  #define BAUD 9600UL\n

  To set baudrate of 19200 \n
  #define BAUD 19200UL
*/
#define BAUD 19200UL

/*!
  \def BAUDRATE
  \brief Calculation macro for setting baudrate

  Do not modify. This macro will automatically calculate value. 
*/
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)    // set baudrate value for UBRR

#define RTS_PIN PORTD4
#define RTS_DIR DDRD
#define RTS_PORT PORTD

void USART_enable(uint8_t rxenable,uint8_t txenable);
void USART_init(uint16_t ubrr_value);
uint8_t USART_readchar();
void USART_writechar(uint8_t data);
uint16_t get_CRC(uint8_t *crc_p, uint8_t length);

#endif /* RS485_H_ */

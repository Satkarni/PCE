/*
 * RS232.h
 *
 * Created: 9/6/2016 11:11:48 AM
 *  Author: Piyush
 */ 


#ifndef RS232_H_
#define RS232_H_
#include <avr/io.h>

#define BAUD_232 38400
#define BAUDRATE_232 ((F_CPU)/(BAUD_232*16UL)-1)    // set baudrate value for UBRR

#define RTS_PIN PG3
#define RTS_DIR DDRG
#define RTS_PORT PORTG

void RS232_init(uint16_t ubrr_value);
void RS232_enable(uint8_t rxenable,uint8_t txenable);
uint8_t RS232_readchar();
void RS232_writechar(uint8_t data);
void RS232_writeString(char *data);




#endif /* RS232_H_ */
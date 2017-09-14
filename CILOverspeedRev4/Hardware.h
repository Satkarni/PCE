/*
 * Hardware.h
 *
 * Created: 22/08/2016 13:08:38
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define RPM_INPUT		PORTD5
#define INT_RESET		PORTD2
#define EXT_RESET		PORTD3

#define MPU_LED			PORTD4
#define DEBUG_LED       PORTD6

#define RELAY1			PORTD7

#define DEBUG_LED_ON		( PORTD |= (1<<DEBUG_LED) )
#define DEBUG_LED_OFF		( PORTD &= ~(1<<DEBUG_LED) )
#define DEBUG_LED_TOGGLE	( PORTD ^= (1<<DEBUG_LED) )

#define MPU_LED_ON		( PORTD |= (1<<MPU_LED) )
#define MPU_LED_OFF		( PORTD &= ~(1<<MPU_LED) )
#define MPU_LED_TOGGLE	( PORTD ^= (1<<MPU_LED) )

#define RELAY1_ON		( PORTD |= (1<<RELAY1) )
#define RELAY1_OFF		( PORTD &= ~(1<<RELAY1) )
#define RELAY1_TOGGLE	( PORTD ^= (1<<RELAY1) )

#define INT_RESET_ON	( !(PIND & (1<<INT_RESET)) )
#define EXT_RESET_ON	( !(PIND & (1<<EXT_RESET)) )


#endif /* HARDWARE_H_ */

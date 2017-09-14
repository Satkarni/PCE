/*
 * Hardware.h
 *
 * Created: 01/12/2016 15:36:53
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_


#define RPM_INPUT		PORTD5

#define TEST			PORTB1
#define RESET			PORTB2
#define DIP_1			PORTD2
#define DIP_2			PORTD3
#define DIP_3			PORTD4

#define RELAY1			PORTB0
#define POWER_LED		PORTD6

#define POWER_LED_ON		( PORTD |= (1<<POWER_LED) )
#define POWER_LED_OFF		( PORTD &= ~(1<<POWER_LED) )
#define POWER_LED_TOGGLE	( PORTD ^= (1<<POWER_LED) )

#define RELAY1_ON		( PORTB |= (1<<RELAY1) )
#define RELAY1_OFF		( PORTB &= ~(1<<RELAY1) )
#define RELAY1_TOGGLE	( PORTB ^= (1<<RELAY1) )


#define TEST_ON		( !(PINB & (1<<TEST)) )
#define RESET_ON	( !(PINB & (1<<RESET)) )
#define DIP_1_ON	( !(PIND & (1<<DIP_1) )
#define DIP_1_OFF	( (PIND & (1<<DIP_1) )
#define DIP_2_ON	( !(PIND & (1<<DIP_2) )
#define DIP_2_OFF	( (PIND & (1<<DIP_2) )
#define DIP_3_ON	( !(PIND & (1<<DIP_3) )
#define DIP_3_OFF	( (PIND & (1<<DIP_3) )



#endif /* HARDWARE_H_ */
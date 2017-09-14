/*
 * Hardware.h
 *
 * Created: 22/08/2016 13:08:38
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define GND_COMMON		PORTC5

#define RPM_INPUT		PORTD5
#define SW_CAL			PORTD2
#define DEBUG_LED		PORTC4

#define RELAY1			PORTC2
#define RELAY2			PORTC3

#define DEBUG_LED_ON		( PORTC |= (1<<DEBUG_LED) )
#define DEBUG_LED_OFF		( PORTC &= ~(1<<DEBUG_LED) )
#define DEBUG_LED_TOGGLE	( PORTC ^= (1<<DEBUG_LED) )

#define RELAY1_ON		( PORTC |= (1<<RELAY1) )
#define RELAY1_OFF		( PORTC &= ~(1<<RELAY1) )
#define RELAY2_ON		( PORTC |= (1<<RELAY2) )
#define RELAY2_OFF		( PORTC &= ~(1<<RELAY2) )

#define SW_CAL_ON		( !(PIND & (1<<SW_CAL)) )



#endif /* HARDWARE_H_ */
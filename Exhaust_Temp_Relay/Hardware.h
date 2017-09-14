/*
 * Hardware.h
 *
 * Created: 15-04-2016 14:13:04
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define PIN_DF_SS		PORTB2
#define PIN_SS			PORTB0
#define PIN_MOSI		PORTB3
#define PIN_SCK			PORTB5

#define GND_COMMON		PORTC5

#define CS_LOW			PORTB &= ~(1<<PIN_SS)
#define CS_HIGH			PORTB |= (1<<PIN_SS)

#define RPM_INPUT		PORTD5
#define SW_CAL			PORTD2
#define DEBUG_LED		PORTC4

#define AD420_FAULT		PORTC0
#define AD420_CLEAR		PORTC1
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

#define ADC_IN1_CHANNEL	( 6 )
#define ADC_IN2_CHANNEL ( 7 )

#endif /* HARDWARE_H_ */
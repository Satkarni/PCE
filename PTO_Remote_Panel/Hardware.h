/*
 * Hardware.h
 *
 * Created: 28/01/2017 13:37:02
 *  Author: ROHANP
 */


#ifndef HARDWARE_H_
#define HARDWARE_H_

/******************* Output LEDs *************************/
#define POWER_LED		PORTB0
#define LOW_OIL_LED		PORTB1
#define FLT_FAIL_LED	PORTB2
#define DEBUG_LED		PORTC4

#define POWER_LED_ON		( PORTB |= (1<<POWER_LED) )
#define POWER_LED_OFF		( PORTB &= ~(1<<POWER_LED) )
#define POWER_LED_TOGGLE	( PORTB ^= (1<<POWER_LED) )

#define LOW_OIL_LED_ON		( PORTB |= (1<<LOW_OIL_LED) )
#define LOW_OIL_LED_OFF		( PORTB &= ~(1<<LOW_OIL_LED) )
#define LOW_OIL_LED_TOGGLE	( PORTB ^= (1<<LOW_OIL_LED) )

#define FLT_FAIL_LED_ON		( PORTB |= (1<<FLT_FAIL_LED) )
#define FLT_FAIL_LED_OFF	( PORTB &= ~(1<<FLT_FAIL_LED) )
#define FLT_FAIL_LED_TOGGLE	( PORTB ^= (1<<FLT_FAIL_LED) )

#define DEBUG_LED_ON		( PORTC |= (1<<DEBUG_LED) )
#define DEBUG_LED_OFF		( PORTC &= ~(1<<DEBUG_LED) )
#define DEBUG_LED_TOGGLE	( PORTC ^= (1<<DEBUG_LED) )
/********************************************************/

/******************** Input switches ********************/
#define SW_DIMMER		PORTC0
#define SW_MUTE			PORTC1
#define SW_LMP_CHECK	PORTC2
#define SW_RESET		PORTC3
#define SW_CAL			PORTC5
#define SW_DI1			PORTD0
#define SW_DI2			PORTD1
#define SW_DI3			PORTD2

#define SW_DIMMER_ON		( !(PINC & (1<<SW_DIMMER)) )
#define SW_DIMMER_OFF		( (PINC & (1<<SW_DIMMER)) )
#define SW_MUTE_ON			( !(PINC & (1<<SW_MUTE)) )
#define SW_MUTE_OFF			( (PINC & (1<<SW_MUTE)) )
#define SW_RESET_ON			( !(PINC & (1<<SW_RESET)) )
#define SW_RESET_OFF		( (PINC & (1<<SW_RESET)) )
#define SW_LMP_CHECK_ON		( !(PINC & (1<<SW_LMP_CHECK)) )
#define SW_LMP_CHECK_OFF	( (PINC & (1<<SW_LMP_CHECK)) )
#define SW_CAL_ON			( !(PINC & (1<<SW_CAL)) )
#define SW_CAL_OFF			( (PINC & (1<<SW_CAL)) )

#define SW_DI1_ON			( (PIND & (1<<SW_DI1)) )
#define SW_DI1_OFF			( !(PIND & (1<<SW_DI1)) )
#define SW_DI2_ON			( (PIND & (1<<SW_DI2)) )
#define SW_DI2_OFF			( !(PIND & (1<<SW_DI2)) )
#define SW_DI3_ON			( (PIND & (1<<SW_DI3)) )
#define SW_DI3_OFF			( !(PIND & (1<<SW_DI3)) )
/********************************************************/

#define BUZZER			PORTD7
#define BRIGHTNESS		PORTD6

#define BUZZER_ON		( PORTD |= (1<<BUZZER) )
#define BUZZER_OFF		( PORTD &= ~(1<<BUZZER) )

#define BRIGHTNESS_ON	( PORTD |= (1<<BRIGHTNESS) )
#define BRIGHTNESS_OFF	( PORTD &= ~(1<<BRIGHTNESS) )


#endif /* HARDWARE_H_ */

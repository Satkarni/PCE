/*
 * Hardware.h *
 * Created: 24-02-2016 14:05:02
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>

#define		DEBUG_REG_PORT	PORTG
#define		DEBUG_REG_DDR	DDRG
#define		DEBUG_REG_PIN	PING

#define		DEBUG_LED1		PG0
#define		DEBUG_LED2		PG1
#define		DEBUG_LED_POWER	PG2

#define		DEBUG_LED1_ON		(DEBUG_REG_PORT |= (1<<DEBUG_LED1))
#define		DEBUG_LED1_OFF		(DEBUG_REG_PORT &= ~(1<<DEBUG_LED1))
#define		DEBUG_LED1_TOGGLE		(DEBUG_REG_PORT ^= (1<<DEBUG_LED1))

#define		DEBUG_LED2_ON		(DEBUG_REG_PORT |= (1<<DEBUG_LED2))
#define		DEBUG_LED2_OFF		(DEBUG_REG_PORT &= ~(1<<DEBUG_LED2))
#define		DEBUG_LED2_TOGGLE		(DEBUG_REG_PORT ^= (1<<DEBUG_LED2))

#define		RELAY_REG_PORT	PORTA
#define		RELAY_REG_DDR	DDRA
#define		RELAY_REG_PIN	PINA

#define		RELAY1	PA6
#define		RELAY2	PA7

#define		KEYPAD_A_REG_PORT		PORTD
#define		KEYPAD_A_REG_DDR		DDRD
#define		KEYPAD_A_REG_PIN		PIND
#define		KEYPAD_B_REG_PORT		PORTE
#define		KEYPAD_B_REG_DDR		DDRE
#define		KEYPAD_B_REG_PIN		PINE


#define		SW_BRIGHTNESS_MINUS		PD0	//INT0
#define		SW_ARROW_UP				PD1 //INT1
#define		SW_ARROW_LEFT			PD2 //INT2
#define		SW_ENTER				PD3 //INT3
#define		SW_ARROW_RIGHT			PE3
#define		SW_BRIGHTNESS_PLUS		PE4 //INT4
#define		SW_ARROW_DOWN			PE5 //INT5
#define		SW_BACK					PE6 //INT6
#define		SW_MUTE					PE7 //INT7

#define		DEBUG_LED_FAULT	PE2

#define		CAN_STBY	PD4


#endif /* HARDWARE_H_ */

/*
 * Hardware.h
 *
 * Created: 05-10-2015 17:42:54
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define MPU PORTE6

#define FUEL_LEAK_DI24V								PORTA6
#define CHARGE_FAIL_DI24V							PORTA7

#define SW_LOCAL_DIR								DDRG
#define SW_LOCAL_IN 								PING
#define SW_LOCAL_RESET 								PORTG2
#define SW_LOCAL_TEST 								PORTG1
#define SW_LOCAL_MUTE 								PORTG3
#define SW_LOCAL_SET								PORTG0

#define START_INPUT									PORTD0
#define STOP_INPUT									PORTD1
#define READY_TO_START_INPUT						PORTE4


#define BUZZER_PORT									PORTG
#define BUZZER_OUTPUT								PORTG4
#define START_OUTPUT								PORTA0
#define STOP_OUTPUT									PORTA1
#define LOCKOUT_RELAY_OUTPUT						PORTD7
#define SPARE_OUTPUT								PORTA4

#define DEBUG_PORT									PORTA
#define DEBUG_LED1									PORTA2		// LED27
#define DEBUG_LED2									PORTA3		// LED28

 
#endif /* HARDWARE_H_ */
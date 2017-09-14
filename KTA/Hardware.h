/*
 * Hardware.h
 *
 * Created: 08-08-2016 14:05:02
 *  Author: ROHANP
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>

#define	DEBUG_REG_PORT	PORTG
#define	DEBUG_REG_DDR	DDRG
#define	DEBUG_REG_PIN	PING

#define	DEBUG_LED1_LD8		PG0
#define	DEBUG_LED2_LD9		PG1

#define DEBUG_LED1_LD8_ON		( DEBUG_REG_PORT |= (1<<DEBUG_LED1_LD8) )
#define DEBUG_LED1_LD8_OFF		( DEBUG_REG_PORT &= ~(1<<DEBUG_LED1_LD8) )
#define DEBUG_LED1_LD8_TOGGLE	( DEBUG_REG_PORT ^= (1<<DEBUG_LED1_LD8) )

#define DEBUG_LED2_LD9_ON		( DEBUG_REG_PORT |= (1<<DEBUG_LED2_LD9) )
#define DEBUG_LED2_LD9_OFF		( DEBUG_REG_PORT &= ~(1<<DEBUG_LED2_LD9) )
#define DEBUG_LED2_LD9_TOGGLE	( DEBUG_REG_PORT ^= (1<<DEBUG_LED2_LD9) )

#define RPM_INPUT		PE6
#define RPM_FREQ_MIN	( 60 )
#define RPM_FREQ_MAX	( 10000 )
#define RPM_NO_OF_TEETH	( 1.0 ) //default = 142.00
#define RPM_SECONDS		( 1.0 )   // default = 60.0

#define	RELAY_REG_PORT		PORTC
#define	RELAY_REG_DDR		DDRC
#define	RELAY_REG_PIN		PINC

#define	RELAY1_WARNING_LAMP	PC0
#define	RELAY2_BUZZER		PC1
#define	RELAY3_LOCKOUT		PC2
#define	RELAY4_RUNNING		PC3
#define	RELAY5_SPARE		PC4	

#define RELAY1_WARNING_LAMP_ON		( RELAY_REG_PORT |= (1<<RELAY1_WARNING_LAMP) )
#define RELAY1_WARNING_LAMP_OFF		( RELAY_REG_PORT &= ~(1<<RELAY1_WARNING_LAMP) )
#define RELAY2_BUZZER_ON			( RELAY_REG_PORT |= (1<<RELAY2_BUZZER) )
#define RELAY2_BUZZER_OFF			( RELAY_REG_PORT &= ~(1<<RELAY2_BUZZER) )
#define RELAY3_LOCKOUT_ON			( RELAY_REG_PORT |= (1<<RELAY3_LOCKOUT) )
#define RELAY3_LOCKOUT_OFF			( RELAY_REG_PORT &= ~(1<<RELAY3_LOCKOUT) )
#define RELAY4_RUNNING_ON			( RELAY_REG_PORT |= (1<<RELAY4_RUNNING) )
#define RELAY4_RUNNING_OFF			( RELAY_REG_PORT &= ~(1<<RELAY4_RUNNING) )
#define RELAY5_SPARE_ON				( RELAY_REG_PORT |= (1<<RELAY5_SPARE) )
#define RELAY5_SPARE_OFF			( RELAY_REG_PORT &= ~(1<<RELAY5_SPARE) )

#define KEYPAD_RESET		PG4
#define KEYPAD_BRIGHTNESS	PE3
#define KEYPAD_INTA			PE4
#define KEYPAD_INTB			PE5
#define KEYPAD_CS			PB6

#define DI_CHARGE_FAIL		PC5
#define DI_CHARGE_FAIL_ON	( !(PINC & (1<<DI_CHARGE_FAIL)) )
#define DI_CHARGE_FAIL_OFF	( (PINC & (1<<DI_CHARGE_FAIL)) )

#define DI_REG_PORT		PORTA
#define DI_REG_DDR		DDRA
#define DI_REG_PIN		PINA

#define DI_START_KEYSW	PA0
#define DI_STOP_KEYSW	PA1
#define DI_RUN			PA2
#define DI_IDLE			PA3
#define DI_SPARE_5V_1	PA4
#define DI_SPARE_5V_2	PA5
#define DI_SPARE_24V_1	PA6
#define DI_SPARE_24V_2	PA7

#define DI_START_KEYSW_ON	( !(DI_REG_PIN & (1<<DI_START_KEYSW)) )
#define DI_START_KEYSW_OFF	(  (DI_REG_PIN & (1<<DI_START_KEYSW)) )
#define DI_STOP_KEYSW_ON	( !(DI_REG_PIN & (1<<DI_STOP_KEYSW)) )
#define DI_STOP_KEYSW_OFF	(  (DI_REG_PIN & (1<<DI_STOP_KEYSW)) )

#define DI_RUN_ON			( !(DI_REG_PIN & (1<<DI_RUN)) )
#define DI_RUN_OFF			( (DI_REG_PIN & (1<<DI_RUN)) )
#define DI_IDLE_ON			( !(DI_REG_PIN & (1<<DI_IDLE)) )
#define DI_IDLE_OFF			( (DI_REG_PIN & (1<<DI_RUN)) )

#define DI_SPARE_5V_1_ON	( !(DI_REG_PIN & (1<<DI_SPARE_5V_1)) )
#define DI_SPARE_5V_1_OFF	( (DI_REG_PIN & (1<<DI_SPARE_5V_1)) )
#define DI_SPARE_5V_2_ON	( !(DI_REG_PIN & (1<<DI_SPARE_5V_2)) )
#define DI_SPARE_5V_2_OFF	( (DI_REG_PIN & (1<<DI_SPARE_5V_2)) )

#define DI_SPARE_24V_1_ON	( !(DI_REG_PIN & (1<<DI_SPARE_24V_1)) )
#define DI_SPARE_24V_1_OFF	( (DI_REG_PIN & (1<<DI_SPARE_24V_1)) )
#define DI_SPARE_24V_2_ON	( !(DI_REG_PIN & (1<<DI_SPARE_24V_2)) )
#define DI_SPARE_24V_2_OFF	( (DI_REG_PIN & (1<<DI_SPARE_24V_2)) )

#define PWM_OUT PB5
#define ACT_DIR_OUT	(PORTC &= ~(1<<DRV_DIRECTION))
#define ACT_DIR_IN		(PORTC |= (1<<DRV_DIRECTION))

#define DRV_DIRECTION PC6


#define	Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define	Leave_Critical_Section		SREG = sreg_; } while (0);

void debugLedInit(void);
void debugLed1LD8blink(uint8_t count);
void debugLed2LD9blink(uint8_t count);
void debugLedAltblink(uint8_t count);
void debugLedSimulblink(uint8_t count);
uint8_t getdebugLed1LD8state(void);
uint8_t getdebugLed2LD9state(void);
void testDebugLED(void);

void debugRelayInit(void);
void debugRelaySimulcycle(uint8_t count);
void testRelay(void);

void debugRpmTimerInit(void);
void debugRpmTimerEnable(void);
void debugRpmTimerDisable(void);
void debugRpmProcess(void);

void debugDIinit(void);
uint8_t debugGetDI(void);
void testDI(void);
void testRPM(void);
void testMain(void);

#endif /* HARDWARE_H_ */

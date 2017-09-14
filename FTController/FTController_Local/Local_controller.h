
#ifndef _LOCAL_CONTROLLER_V1_1_H_
#define _LOCAL_CONTROLLER_V1_1_H_


#define F_CPU 14745600UL


#include <util/delay.h>
#include <avr/io.h>
#include "7219_display_Local.h"
#include "RS485_Local.h"
#include "Sensors.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>


#define SYSTEM_TIMER_OCR_VAL 7200		// 1/1024 PRESCALER, 500 MS CYCLE

#define DISPLAY_MAX_VAL 9999

#define STARTER_TIMEOUT 20				// value/2 = N sec timeout for engaging starter, (20/2 = 10s)
#define STOP_TIMEOUT	180
#define LOP_SENSOR_ENABLE_DELAY 20		// 10 sec delay for enabling lop sensor
#define RPM_SENSOR_FAIL_DELAY	20		// 10 sec delay for rpm sensor fail trip
#define BAT_VGE_ALARM_DELAY		10		// 5 sec delay for battery vge low
	

#define MPU PORTE6

#define EEP_HOUR_ADDRESS		0x00			// first byte stores hours elapsed
#define EEP_MIN_ADDRESS 		0x04			// minutes elapsed address is stored here

#define EEP_MIN_ADDRESS_OFFSET	0x0A
#define EEP_MAX_ADDRESS			0xFF			// Bytes 10- 255 will store minutes elapsed

#define RUNTIME_MAX_VAL 		9999
#define MIN_MAX_VAL				60
#define EEP_MIN_COUNT 			120 			//depends on system timer, eep_min_count = 60000/system tick(ms)
#define EEP_HOUR_COUNT			60				// will be 60 in final

#define FUEL_LEAK_DI24V		PORTA6
#define CHARGE_FAIL_DI24V	PORTA7

#define SW_LOCAL_DIR	DDRG
#define SW_LOCAL_IN 	PING
#define SW_LOCAL_RESET 	PORTG0
#define SW_LOCAL_TEST 	PORTG1
#define SW_LOCAL_MUTE 	PORTG2
#define SW_LOCAL_SET	PORTG3


#define START_INPUT			PORTD0
#define STOP_INPUT			PORTD1
#define LOC_REMOTE_INPUT	PORTE4


#define BUZZER_PORT				PORTG
#define BUZZER_OUTPUT			PORTG4
#define START_OUTPUT			PORTA0
#define STOP_OUTPUT				PORTA1
#define LOCKOUT_RELAY_OUTPUT	PORTD7
#define SPARE_OUTPUT			PORTA4

#define DEBUG_PORT			PORTA
#define DEBUG_LED1			PORTA2		// LED27
#define DEBUG_LED2			PORTA3		// LED28

#define CHARGER_LED			8
#define REMOTE_STATE_LED	9
#define REMOTE_STATE_TO_LED 0x4000
#define LOCKOUT_LED			10
#define LOCKOUT_TO_REMOTE	0x2000
#define STOP_LED			11
#define OVERSPEED_LED		7
#define LOP_LOW_LED			6
#define COM_ERROR_LED		5
#define WATER_TEMP_HIGH_LED 4
#define LOT_HIGH_LED		3
#define START_LED			2
#define FUEL_LEAK_LED		1
#define BAT_VGE_LOW_LED		0

#define BUZZER_BUZZER_STATE				0x0400
#define OVERSPEED_TRIP_BUZZER_STATE		0x0200
#define LOP_TRIP_BUZZER_STATE			0x0100
#define OVERSPEED_ALARM_BUZZER_STATE	0x0080
#define LOP_ALARM_BUZZER_STATE			0x0040
#define COM_ERROR_ALARM_BUZZER_STATE	0x0020
#define WT_ALARM_BUZZER_STATE			0x0010
#define LOT_ALARM_BUZZER_STATE			0x0008
#define CF_ALARM_BUZZER_STATE			0x0004
#define FL_ALARM_BUZZER_STATE			0x0002
#define BV_ALARM_BUZZER_STATE			0x0001

#define FRAME_SIZE			21
#define FRAME_END			0xAA

#define READY_TO_RECEIVE 255
#define PROCESS_ALL_SENSORS 1
#define ONLY_PROCESS_RPM_LOP 2

void system_init_main();
void test_display_main();
void system_timer_enable();
void enable_rpm_timer();
  

typedef struct{
	
	uint8_t alarm;
	uint8_t alarmC1;
	uint8_t alarmC2;
	uint8_t trip;
	uint8_t tripC1;
	uint8_t tripC2;
	uint8_t buzzer;
	uint8_t buzzer1;
	uint8_t buzzer2;
	uint8_t enable;
	uint8_t out_of_range;
	
}SENSOR;



#endif

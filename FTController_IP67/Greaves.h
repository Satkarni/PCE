/*
 * Greaves.h
 *
 * Created: 05-10-2015 16:07:21
 *  Author: ROHANP
 */ 


#ifndef GREAVES_H_
#define GREAVES_H_

#define F_CPU 14745600UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "7219_display_Local.h"
#include "Hardware.h"
#include "Sensors.h"

#define SYSTEM_TIMER_OCR_VAL 7200		// 1/1024 PRESCALER, 500 MS CYCLE

#define DISPLAY_MAX_VAL 9999

#define STARTER_TIMEOUT 20				// value/2 = N sec timeout for engaging starter, (20/2 = 10s)
#define STOP_TIMEOUT	20

#define LOP_SENSOR_ENABLE_DELAY 20		// 10 sec delay for enabling lop sensor
#define RPM_SENSOR_FAIL_DELAY   20		// 10 sec delay for rpm sensor fail
#define BAT_VGE_ALARM_DELAY		10		// 5 sec delay for low battery vge


#define EEP_HOUR_ADDRESS		0x00			// first byte stores hours elapsed
#define EEP_MIN_ADDRESS 		0x04			// minutes elapsed address is stored here

#define EEP_MIN_ADDRESS_OFFSET	0x0A
#define EEP_MAX_ADDRESS			0xFF			// Bytes 10- 255 will store minutes elapsed

#define RUNTIME_MAX_VAL 		9999
#define MIN_MAX_VAL				60
#define EEP_MIN_COUNT 			120 			//depends on system timer, eep_min_count = 60000/system tick(ms)
#define EEP_HOUR_COUNT			60				// will be 60 in final

// DIGIT2
#define SPARE_LED1				2048
#define SPARE_LED2	 			1024
#define CHARGE_FAIL_LED			512
#define FUEL_LEAK_LED			256

//DIGIT 1 
#define OVERSPEED_LED			128
#define LOP_LOW_LED				64
#define LOT_HIGH_LED			32
#define WATER_TEMP_HIGH_LED		16
#define BAT_VGE_LOW_LED			8
#define START_LED				4
#define STOP_LED				2
#define LOCKOUT_LED 			1

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

void system_init_main();
void system_timer_enable();
void system_timer_disable();
void rpm_timer_init();
void rpm_timer_enable();

void ADC_init();
void ADC_enable();
inline void ADC_disable();
uint16_t ADC_read(uint8_t channel);

void test_display_main();
void test_simple();

void process_rpm();
void process_lop();
void process_lot(uint8_t init);
void process_wt(uint8_t init);
void process_bv(uint8_t init);
void process_fuel_leak();
void process_charge_fail();
void process_runtime();
void eeprom_init();

#endif /* GREAVES_H_ */


#ifndef _REMOTE_CONTROLLER_H_
#define _REMOTE_CONTROLLER_H_


#define F_CPU 14745600UL

#include <util/delay.h>
#include <avr/io.h>
#include "7219_display_Remote.h"
#include "RS485_Remote.h"
#include <avr/interrupt.h>

#define BUZZER_RELAY	  PORTD4
#define BUZZER_SWITCH	  PORTD3
#define BUZZER_PORT		  PORTD
#define BUZZER_DIR	      DDRD
#define BUZZER_INPUT	  PIND

DISPLAY display;

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
	
}REMOTE_SENSOR;


#define FRAME_SIZE				21
#define FRAME_END				0xAA
#define POLLING_RATE			7200
#define COM_TIMEOUT_PACKET		28800 // com_timeout = 14400*x ( x is seconds) ....e.g. for 2 seconds com_timeout = 28800
#define COM_TIMEOUT_FRAME		4320  // timeout between bytes in one frame = 300 ms	
#define COM_TIMEOUT_INIT		43200 // On power on, com_timeout buzzer if comm not established in 3 seconds	

#define COM_ERROR_LED			5
#define REMOTE_FROM_LOCAL		0x4000
#define REMOTE_STATE_LED		2
#define LOCKOUT_FROM_LOCAL		0x2000
#define LOCKOUT_LED				PORTD6
#define BUZZER_FROM_LOCAL		0x0F00

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

#endif
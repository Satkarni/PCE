/*
 * Config.h
 *
 * Created: 24-02-2016 16:34:40
 *  Author: ROHANP
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>
#include <avr/eeprom.h>

#define LOP_TRIP_DF			( 80 )	// kPa  (multiply BAR by 100, example->  0.8 bar = 80 kPa, 2 bar = 200 kPa)
#define WT_TRIP_DF			( 105 )	// deg C
#define FAULT_ENABLE_DF		(500)	// rpm
#define FAULT_DISABLE_DF	(300)	// rpm

#define LOP_TRIP_DELAY_DF			( 5 )	// sec
#define WT_TRIP_DELAY_DF			( 5 )	// sec
#define FAULT_ENABLE_DELAY_DF		( 10 )	// sec


#define EEP_LOP_TRIP_ADDR			(0x02)
#define EEP_LOP_TRIP_DELAY_ADDR		(0x05)
#define EEP_WT_TRIP_ADDR			(0x08)
#define EEP_WT_TRIP_DELAY_ADDR		(0x0B)
#define EEP_FLT_EN_ADDR				(0x0E)
#define EEP_FLT_EN_DELAY_ADDR		(0x11)
#define EEP_FLT_DIS_ADDR			(0x14)

typedef enum
{
	INC,
	DEC,
	DISPLAY,
	SET
}COMMAND;
typedef enum
{
	LEFT,
	RIGHT	
}SELECT;

typedef enum
{
	PARAM_LOP_TRIP,
	PARAM_LOP_TRIP_DELAY,
	PARAM_WT_TRIP,
	PARAM_WT_TRIP_DELAY,
	PARAM_FAULT_ENABLE,
	PARAM_FAULT_ENABLE_DELAY,
	PARAM_FAULT_DISABLE
}param_label;

typedef struct  
{
	param_label plabel;
	uint16_t dfault;
	uint16_t min;
	uint16_t max;
	uint16_t step;
	int prm;
}param;
		
param lop_trip;
param lop_trip_delay;
param wt_trip;
param wt_trip_delay;
param flt_enable;
param flt_enable_delay;
param flt_disable;

void config_init();
void config_draw_cursor(SELECT sel);
void config_main();
void config_page_update(uint8_t page_nu,SELECT sel,COMMAND cmd);

#endif /* CONFIG_H_ */
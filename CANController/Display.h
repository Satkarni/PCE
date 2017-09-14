/*
 * Display.h
 *
 * Created: 05-03-2016 11:03:09
 *  Author: ROHANP
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "ks0108.h"
#include "arial.h"
#include "times_new_roman.h"
#include "sanserif.h"
#include "arial_bold_14.h"
#include "pce_logo.h"
#include "math.h"
#include "J1939.h"

#define	Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define	Leave_Critical_Section		SREG = sreg_; } while (0); 

#define UPDATE_RATE	( 31250 ) //31250

#define SW_RIGHT	(KEYPAD_B_REG_PIN & (1<<SW_ARROW_RIGHT))
#define SW_LEFT		(KEYPAD_A_REG_PIN & (1<<SW_ARROW_LEFT))
#define SW_UP		(KEYPAD_A_REG_PIN & (1<<SW_ARROW_UP))
#define SW_DOWN		(KEYPAD_B_REG_PIN & (1<<SW_ARROW_DOWN))
#define SW_ENTER_P	(KEYPAD_A_REG_PIN & (1<<SW_ENTER))

#define LAYOUT_STATUS_X		3
#define LAYOUT_STATUS_Y		53
#define LAYOUT_MSG_X		30
#define LAYOUT_MSG_Y		43

#define LAYOUT_LABEL1_X		10
#define LAYOUT_LABEL1_Y		0
#define LAYOUT_LABEL2_X		73
#define LAYOUT_LABEL2_Y		0


#define LAYOUT_UNIT1_X		30
#define LAYOUT_UNIT1_Y		30
#define LAYOUT_UNIT2_X		83
#define LAYOUT_UNIT2_Y		30


#define LAYOUT_PARAM1_X		20
#define LAYOUT_PARAM1_Y		15
#define LAYOUT_PARAM2_X		83
#define LAYOUT_PARAM2_Y		15

#define PAGE_MAX	9
#define PAGE_MIN	1
#define PAGE_CONFIG_MIN	11
#define PAGE_CONFIG_MAX 12

#define MSG_MAX_NUM		10
#define MSG_LOP_TRIP	0
#define MSG_WT_TRIP		1
#define MSG_SAFETY_EN	2
#define MSG_SAFETY_DIS	3
#define MSG_CONFIG_DIS	4

volatile uint8_t in_config,in_config_old;
volatile uint8_t page_num;

static uint8_t old_page_n;

uint8_t reset_page_update;

typedef struct  
{
	volatile uint16_t acc;
	volatile unsigned int en: 1;
	volatile unsigned int done:1;
	volatile unsigned int oneshot: 1;
}timer;

typedef struct
{
	uint32_t spn;
	uint8_t fmi;
	uint8_t occ;
	
}DM;

DM DM1[100],DM2[100];

typedef enum
{
	READY,
	RUNNING,
	TRIP,
	CAN_TIMEOUT,
	CAN_ERROR, 
	VALUE_SAVED,
	DEFAULT_STATUS
}STATUS;

typedef enum
{
	SAFETY_DISABLED,
	SAFETY_ENABLED,
	OVERSPEED_ALARM,
	ECT_HIGH,
	LOP_LOW,
	COOLANT_LEVEL_LOW,
	DEFAULT
}ALARM_MSG;

typedef struct
{
	uint16_t	rpm;

	int			act_eng_torque;
	int			dem_eng_torque;
	
	uint8_t		acc_pedal_pos1;
	uint8_t		eng_percent_load;
	uint8_t		max_torque_percent;
	
	uint16_t	soft_version;
	uint16_t	soft_sub_version;
	
	int			ect;
	uint16_t	lot;
	int			fuel_temp;
	
	uint8_t		lop;
	uint8_t		oil_level;
	uint8_t		coolant_level;
	
	uint8_t		eng_protection_shutoff;
	uint8_t		eng_shutoff_override;;
	
	uint32_t	hours;
	
	uint32_t	trip_fuel;
	uint32_t	total_fuel;
	
	uint32_t	spn;
	uint8_t		fmi;
	
	uint8_t		boost_pressure;
	uint8_t		boost_temperature;
	uint16_t	batvge;
	
	volatile unsigned int	mute : 1;
	volatile unsigned int	reset : 1;
	volatile unsigned int	fault_enable : 1;
	volatile unsigned int	fault_disable : 1;
	
	STATUS		status;
	ALARM_MSG	alarm_msg;
	char		*message[4];	
	timer		fault;
}Engine_Data;

Engine_Data engine_data;

typedef struct 
{
	uint8_t page_nu;
	char* label1;
	char* label2;
	char* label3;
	char* label4;
	char* unit1;
	char* unit2;
	char* unit3;
	char* unit4;
	uint16_t param1;
	uint16_t param2;
	uint16_t param3;
	uint16_t param4;
}page;

typedef struct  
{
	uint8_t page_nu;
	char* label1;
	char* label2;
	char* unit1;
	char* unit2;
	uint16_t param1;
	uint16_t param2;
}config_page;

typedef struct
{
	//uint8_t max_pages;
	//uint8_t page_number;
	uint8_t page_increment : 1;
	uint8_t page_decrement : 1;
	uint8_t DM_number;
	uint8_t refresh : 1;
}Diagnostic;

Diagnostic DG,DG2;

void display_init(uint8_t bright);
void display_PWM_init();
void display_PWM_set_duty(uint8_t brightness_arg);

void display_update_init();
void display_update_enable(uint16_t rate);
void display_update_disable();
void display_loading_text();
void display_function_text(uint8_t msg);
void display_loading_bar();
void display_update_load(uint8_t lag, uint8_t initial, uint8_t final);

void display_page_update(Engine_Data* p_en_data,uint8_t page_n);
void display_page_draw(page* ppage);
void display_hours_draw();
void display_page_select(uint8_t page_num);
void display_fancy_effect();
void display_can_failure();

void display_active_dtc();
void display_config_menu(uint8_t highlight);
void display_config_page(page *cpage);
void display_about_page();

void j1939_request_hours();


void display_param_label(char* label,uint8_t label_num);
void display_param_unit(char* unit,uint8_t unit_num);
void display_message(ALARM_MSG index);
void display_status(STATUS status);

#endif /* DISPLAY_H_ */
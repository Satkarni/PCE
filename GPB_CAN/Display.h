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
#include "pce_glcd.h"
#include "arial_bold_14.h"
#include "pce_logo.h"

#define	Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define	Leave_Critical_Section		SREG = sreg_; } while (0); 

#define UPDATE_RATE	( 31250 )

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

#define PAGE_MAX	2
#define PAGE_MIN	1
#define PAGE_CONFIG_MIN	11
#define PAGE_CONFIG_MAX 17

#define MSG_MAX_NUM		10
#define MSG_LOP_TRIP	0
#define MSG_WT_TRIP		1
#define MSG_SAFETY_EN	2
#define MSG_SAFETY_DIS	3
#define MSG_CAN_TIMEOUT	4
#define MSG_CAN_ERROR	5 

volatile uint8_t in_config,in_config_old;
volatile uint8_t page_num;
volatile uint8_t in_version,in_version_old;

typedef struct  
{
	volatile uint16_t acc;
	volatile unsigned int en: 1;
	volatile unsigned int done:1;
	volatile unsigned int oneshot: 1;
}timer;

typedef enum
{
	READY,
	RUNNING,
	TRIP,
	VALUE_SAVED,
	DEFAULT_STATUS
}STATUS;

typedef struct
{
	uint8_t		lop;
	int			ect;
	uint16_t	rpm;
	uint16_t	batvge;
	volatile unsigned int	mute : 1;
	volatile unsigned int	reset : 1;
	volatile unsigned int	fault_enable : 1;
	volatile unsigned int	fault_disable : 1;
	STATUS		status;
	uint8_t		idx;
	char		*message[6];	
	timer		fault;
}Engine_Data;

Engine_Data engine_data;

typedef struct 
{
	uint8_t page_nu;
	char* label1;
	char* label2;
	char* unit1;
	char* unit2;
	uint16_t param1;
	uint16_t param2;
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


void display_init(uint8_t bright,uint16_t rate);
void display_PWM_init();
void display_PWM_set_duty(uint8_t brightness_arg);

void display_update_init();
void display_update_enable(uint16_t rate);
void display_update_disable();

void display_page_update(Engine_Data* p_en_data,uint8_t page_n,uint8_t j1939_timeout_f);
void display_page_draw(page* ppage);
void display_page_select(uint8_t page_num);


void display_param_label(char* label,uint8_t label_num);
void display_param_unit(char* unit,uint8_t unit_num);
void display_message(char* message,uint8_t clear);
void display_status(STATUS status);

#endif /* DISPLAY_H_ */
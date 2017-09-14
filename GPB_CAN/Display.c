/*
 * Display.c
 *
 * Created: 05-03-2016 11:03:24
 *  Author: ROHANP
 */ 


#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "Display.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

volatile uint8_t brightness = 0xFF;


page page1 =  { .page_nu = 1, .label1 = "Lube Oil Pr.", .label2 = "Water Temp", .unit1 = "bar", .unit2 = "deg C"};
page page2 =  { .page_nu = 2, .label1 = "Speed", .label2 = "Battery", .unit1 = "rpm", .unit2 = "Volts"};

page config_page1 =  { .page_nu = PAGE_CONFIG_MIN, .label1 = "LOP Trip", .label2 = "Delay", .unit1 = "bar", .unit2 = "sec" };
page config_page2 =  { .page_nu = PAGE_CONFIG_MIN +1, .label1 = "WT Trip", .label2 = "Delay", .unit1 = "deg C", .unit2 = "sec"};
page config_page3 =  { .page_nu = PAGE_CONFIG_MIN +2, .label1 = "Fault Enable", .label2 = "Delay", .unit1 = "rpm", .unit2 = "sec"};	
page config_page4 =  { .page_nu = PAGE_CONFIG_MIN +3, .label1 = "Fault Disable", .label2 = "Spare", .unit1 = "rpm", .unit2 = "sec"};	
page config_page5 =  { .page_nu = PAGE_CONFIG_MIN +4, .label1 = "Spare1", .label2 = "Delay", .unit1 = "deg C", .unit2 = "sec"};	
page config_page6 =  { .page_nu = PAGE_CONFIG_MIN +5, .label1 = "Spare2", .label2 = "Delay", .unit1 = "deg C", .unit2 = "sec"};	
page config_page7 =  { .page_nu = PAGE_CONFIG_MAX, .label1 = "Spare3", .label2 = "Delay", .unit1 = "deg C", .unit2 = "sec"};		

void display_init(uint8_t bright,uint16_t rate)
{
	
	Enter_Critical_Section
	engine_data.message[MSG_LOP_TRIP] =		"LOP Very Low Trip";
	engine_data.message[MSG_WT_TRIP] =		"WT Very High Trip", 
	engine_data.message[MSG_SAFETY_EN] =	"Safety Enabled";    
	engine_data.message[MSG_SAFETY_DIS] =	"Safety Disabled";   
	engine_data.message[MSG_CAN_TIMEOUT] =	"Canbus Timeout";
	engine_data.message[MSG_CAN_ERROR] =	"Canbus Error";
	engine_data.idx = MSG_MAX_NUM;
	engine_data.status = READY;
	Leave_Critical_Section
	
	display_PWM_init();
	display_PWM_set_duty(bright);
	
	_delay_ms(2);				// little delay for GLCD
	ks0108Init(1);
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108LoadBitmap(pce_logo);
	_delay_ms(2000);
	ks0108ClearScreen();
	ks0108Init(0);
	
	display_update_init();
	display_update_enable(rate);
	page_num = PAGE_MIN;
	display_page_select(page_num);
	
}

void display_PWM_init()
{
	DDRB |= (1<<PB7);	// PWM pin is output
	TCCR0A |= ((1<<WGM01)|(1<<WGM00)); // Fast PWM mode
	TCCR0A |= ((1<<COM0A1)|(0<<COM0A0)); // Non inverted PWM
	TCCR0A |= ((1<<CS02)|(0<<CS01)|(0<<CS00));	// 1/256 prescaler, PWM freq ~ 60 Khz
	
}

void display_PWM_set_duty(uint8_t brightness_arg)
{
	OCR0A = brightness_arg;
}
 
ISR(INT0_vect) // Brightness --
{
	_delay_ms(300);
	brightness = (brightness == 0) ? 0	: (brightness - 51);
	display_PWM_set_duty(brightness);
}

ISR(INT4_vect) // Brightness ++
{
	brightness = (brightness == 0xFF) ? 0xFF : (brightness + 51);
	display_PWM_set_duty(brightness);
}

void display_update_init()
{
	TCCR1B |= ((0<<WGM13)|(1<<WGM12));		// CTC Mode
}

void display_update_enable(uint16_t rate)
{
	Enter_Critical_Section
		OCR1A = rate;
	Leave_Critical_Section
	TIMSK1 |= (1<<OCIE1A);
	TCCR1B |= ((1<<CS12)|(0<<CS11)|(0<<CS10));	// clk/256  prescaler
}

void display_update_disable()
{
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));	// clk/64  prescaler
	TIMSK1 &= ~(1<<OCIE1A);
}

void display_status(STATUS status)
{
	
	switch(status)
	{
		case READY :
			ks0108Puts_mod("Ready",LAYOUT_STATUS_X+35,LAYOUT_STATUS_Y,50);
		break;
		
		case RUNNING :
			ks0108Puts_mod("Run",LAYOUT_STATUS_X+35,LAYOUT_STATUS_Y,50);
		break;
		
		case TRIP :
			ks0108Puts_mod("Trip",LAYOUT_STATUS_X+35,LAYOUT_STATUS_Y,50);
		break;
		
		case VALUE_SAVED :
			ks0108Puts_mod("Value Saved",LAYOUT_STATUS_X+35,LAYOUT_STATUS_Y,50);
		break;
		
		case DEFAULT_STATUS :
			ks0108FillRect(LAYOUT_STATUS_X+35,LAYOUT_STATUS_Y,50,9,WHITE);
		break;
		
		default:
		break;
	}
	
}

void display_param_label(char* label,uint8_t label_num)
{
	if(label_num == 1)
	{
		//ks0108Puts_mod(label,LAYOUT_LABEL1_X,LAYOUT_LABEL1_Y,60-LAYOUT_LABEL1_X);
		ks0108Puts_mod_align_center(label,0,63,0,50);
	}
	if(label_num == 2)
	{
		//ks0108Puts_mod(label,LAYOUT_LABEL2_X,LAYOUT_LABEL2_Y,125-LAYOUT_LABEL2_X);
		ks0108Puts_mod_align_center(label,64,127,0,50);
	}
}

void display_param_unit(char* unit,uint8_t unit_num)
{
	if(unit_num == 1)
	{
		//ks0108Puts_mod(unit,LAYOUT_UNIT1_X,LAYOUT_UNIT1_Y,60-LAYOUT_UNIT1_X);
		ks0108Puts_mod_align_center(unit,0,63,LAYOUT_UNIT1_Y,60-LAYOUT_UNIT1_X);
	}
	if(unit_num == 2)
	{
		//ks0108Puts_mod(unit,LAYOUT_UNIT2_X,LAYOUT_UNIT2_Y,125-LAYOUT_UNIT2_X);
		ks0108Puts_mod_align_center(unit,64,127,LAYOUT_UNIT1_Y,60-LAYOUT_UNIT1_X);
	}
}

void display_page_draw(page *ppage)
{
	Enter_Critical_Section
	
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	
	display_param_label(ppage->label1,1);
	display_param_unit(ppage->unit1,1);
	
	ks0108DrawVertLine(63,0,40,BLACK);
	ks0108DrawHoriLine(0,40,127,BLACK);
	
	display_param_label(ppage->label2,2);
	display_param_unit(ppage->unit2,2);
	
	ks0108Puts_mod("Status : ",LAYOUT_STATUS_X,LAYOUT_STATUS_Y,50);
	
	Leave_Critical_Section
}
 
void display_page_update(Engine_Data* p_en_data, uint8_t page_n,uint8_t j1939_timeout_f)
{
	Enter_Critical_Section
	
	char ect_char[16];
	char lop_char[16];
	char rpm_char[16];
	char bat_vge_char[16];
	double bat_vge_f, lop_f,rpm_f;	
	static uint8_t old_idx,old_page_n;
	static STATUS status_old;
	switch(page_n)
	{
		case 1 : //LOP and ECT
			if(j1939_timeout_f)
			{
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				strcpy(lop_char,"No data");
				strcpy(ect_char,"No data");
			}else
			{
				lop_f = (p_en_data->lop)*4;			// lop_f is double in kPa
				lop_f /= 100;						// convert to bar
				dtostrf(lop_f,4,2,lop_char);
				itoa(((p_en_data->ect)-40),ect_char,10);
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);	
			}
			
			ks0108Puts_mod_align_center(lop_char,0,63,LAYOUT_PARAM1_Y,60);
			ks0108Puts_mod_align_center(ect_char,64,127,LAYOUT_PARAM2_Y,60);
		
		break;
		
		case 2 : //RPM and Bat Vge
		
			if(j1939_timeout_f)
			{
				
				strcpy(rpm_char,"No data");
				strcpy(bat_vge_char,"No data");
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
			}else
			{
				rpm_f = (p_en_data->rpm)/8.0;
				dtostrf(rpm_f,1,0,rpm_char);
				bat_vge_f = (p_en_data->batvge);
				bat_vge_f *= 0.05;
				dtostrf(bat_vge_f,4,2,bat_vge_char);
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);	
			}
	
			ks0108Puts_mod_align_center(rpm_char,0,63,LAYOUT_PARAM1_Y,60);
			ks0108Puts_mod_align_center(bat_vge_char,64,127,LAYOUT_PARAM2_Y,60);
		break;	
	}
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	if(p_en_data->status != status_old || in_config_old || page_n != old_page_n || in_version_old)
	{
		display_status(p_en_data->status);
		status_old = p_en_data->status;
	}
	
	if(p_en_data->idx != MSG_MAX_NUM && (p_en_data->idx != old_idx || in_config_old || page_n != old_page_n || in_version_old) )
	{
		ks0108FillRect(LAYOUT_MSG_X,LAYOUT_MSG_Y,127,9,WHITE);
		ks0108Puts_mod_align_center(p_en_data->message[p_en_data->idx],0,127,LAYOUT_MSG_Y,127);
		old_idx = p_en_data->idx;
		old_page_n = page_n;
		in_config_old = 0;
		in_version_old = 0;
	}
	
	Leave_Critical_Section
}

void display_page_select(uint8_t page_n)
{
	Enter_Critical_Section
	switch(page_n)
	{
		case PAGE_MIN:
				display_page_draw(&page1);
		break;
		
		case PAGE_MIN+1:
				display_page_draw(&page2);
		break;
		
		case PAGE_CONFIG_MIN :
				display_page_draw(&config_page1);
		break;
		
		case PAGE_CONFIG_MIN + 1:
				display_page_draw(&config_page2);
		break;
		
		case PAGE_CONFIG_MIN + 2:
				display_page_draw(&config_page3);
		break;
		
		case PAGE_CONFIG_MIN + 3:
				display_page_draw(&config_page4);
		break;
		
		case PAGE_CONFIG_MIN + 4:
				display_page_draw(&config_page5);
		break;
		
		case PAGE_CONFIG_MIN + 5:
				display_page_draw(&config_page6);
		break;
		
		case PAGE_CONFIG_MAX:
				display_page_draw(&config_page7);
		break;
	}
	Leave_Critical_Section
}
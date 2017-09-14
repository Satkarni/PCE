/*
 * Config.c
 *
 * Created: 12-03-2016 14:36:33
 *  Author: ROHANP
 */ 
#ifndef F_CPU
#define F_CPU ( 16000000UL )
#endif

#include "Config.h"
#include "Display.h"
#include "Hardware.h"

#include <stdlib.h>
#include <math.h>
#include <avr/eeprom.h>
#include <util/delay.h>

param lop_trip =			{ PARAM_LOP_TRIP,LOP_TRIP_DF,60,100,4,0};
param lop_trip_delay =		{ PARAM_LOP_TRIP_DELAY, LOP_TRIP_DELAY_DF,2,8,1,0};
param wt_trip =				{ PARAM_WT_TRIP,  WT_TRIP_DF,94,100,1,0};
param wt_trip_delay =		{ PARAM_WT_TRIP_DELAY, WT_TRIP_DELAY_DF,2,8,1,0};
param flt_enable =			{ PARAM_FAULT_ENABLE, FAULT_ENABLE_DF,500,600,10,0};
param flt_enable_delay =	{ PARAM_FAULT_ENABLE_DELAY, FAULT_ENABLE_DELAY_DF,2,10,1,0};
param flt_disable =			{ PARAM_FAULT_DISABLE, FAULT_DISABLE_DF,200,300,10,0};


void config_init()
{
	Enter_Critical_Section
	
	volatile uint16_t tmp = 0;
	tmp = eeprom_read_word((uint16_t*)EEP_LOP_TRIP_ADDR);
	lop_trip.prm =			(tmp == 0xFFFF /*|| tmp == 0*/) ? lop_trip.dfault : tmp;
	
	tmp = eeprom_read_word((uint16_t*)EEP_LOP_TRIP_DELAY_ADDR);
	lop_trip_delay.prm =	(tmp == 0xFFFF /*|| tmp == 0*/) ? lop_trip_delay.dfault : tmp;
	
	tmp = eeprom_read_word((uint16_t*)EEP_WT_TRIP_ADDR);
	wt_trip.prm =			(tmp == 0xFFFF /*|| tmp == 0*/ ) ? wt_trip.dfault : tmp;
	
	tmp = eeprom_read_word((uint16_t*)EEP_WT_TRIP_DELAY_ADDR);
	wt_trip_delay.prm =		(tmp == 0xFFFF /*|| tmp == 0*/) ? wt_trip_delay.dfault : tmp;
	
	tmp = eeprom_read_word((uint16_t*)EEP_FLT_EN_ADDR);
	flt_enable.prm =		(tmp == 0xFFFF /*|| tmp == 0*/) ? flt_enable.dfault : tmp;
	
	tmp = eeprom_read_word((uint16_t*)EEP_FLT_EN_DELAY_ADDR);
	flt_enable_delay.prm =	(tmp == 0xFFFF /*|| tmp == 0*/ ) ? flt_enable_delay.dfault : tmp;
	
	tmp = eeprom_read_word((uint16_t*)EEP_FLT_DIS_ADDR);
	flt_disable.prm =		(tmp == 0xFFFF /*|| tmp == 0*/) ? flt_disable.dfault : tmp;
	
	Leave_Critical_Section
}


void config_draw_cursor(SELECT sel)
{
	ks0108FillRect(10,28,40,1,WHITE);
	ks0108FillRect(73,28,40,1,WHITE);
	
	if(sel == LEFT) ks0108DrawHoriLine(18,28,25,BLACK);
	if(sel == RIGHT) ks0108DrawHoriLine(81,28,25,BLACK);
}

void config_page_update(uint8_t page_nu, SELECT sel,COMMAND cmd)
{
	char cparam1[16];
	char cparam2[16];
	uint16_t param1=0,param2=0;
	float lop_f;
	STATUS	config_status = DEFAULT_STATUS;
	switch(page_nu)
	{
		case PAGE_CONFIG_MIN :
				
				
				if(sel == LEFT) // LOP Trip
				{
					if(cmd == INC)
					{
						lop_trip.prm = (lop_trip.prm >= lop_trip.max) ? lop_trip.max : lop_trip.prm + lop_trip.step;
					}else if(cmd == DEC)
					{
						lop_trip.prm = (lop_trip.prm <= lop_trip.min) ? lop_trip.min : lop_trip.prm - lop_trip.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_LOP_TRIP_ADDR,lop_trip.prm);
						config_status = VALUE_SAVED;
					} 
				}
				if(sel == RIGHT) // LOP Trip Delay
				{
					if(cmd == INC)
					{
						lop_trip_delay.prm = (lop_trip_delay.prm >= lop_trip_delay.max) ? lop_trip_delay.max : lop_trip_delay.prm + lop_trip_delay.step;
					}else if(cmd == DEC)
					{
						lop_trip_delay.prm = (lop_trip_delay.prm <= lop_trip_delay.min) ? lop_trip_delay.min : lop_trip_delay.prm - lop_trip_delay.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_LOP_TRIP_DELAY_ADDR,lop_trip_delay.prm);
						config_status = VALUE_SAVED;
					}
				}
				
				param1 = lop_trip.prm;
				param2 = lop_trip_delay.prm;
		break;
		
		case PAGE_CONFIG_MIN + 1:
				if(sel == LEFT) // WT Trip
				{
					if(cmd == INC)
					{
						wt_trip.prm = (wt_trip.prm >= wt_trip.max) ? wt_trip.max : wt_trip.prm + wt_trip.step;
					}else if(cmd == DEC)
					{
						wt_trip.prm = (wt_trip.prm <= wt_trip.min) ? wt_trip.min : wt_trip.prm - wt_trip.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_WT_TRIP_ADDR,wt_trip.prm);
						config_status = VALUE_SAVED;
					}
				}
				if(sel == RIGHT) // WT Trip Delay
				{
					if(cmd == INC)
					{
						wt_trip_delay.prm = (wt_trip_delay.prm >= wt_trip_delay.max) ? wt_trip_delay.max : wt_trip_delay.prm + wt_trip_delay.step;	
					}else if(cmd == DEC)
					{
						wt_trip_delay.prm = (wt_trip_delay.prm <= wt_trip_delay.min) ? wt_trip_delay.min : wt_trip_delay.prm - wt_trip_delay.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_WT_TRIP_DELAY_ADDR,wt_trip_delay.prm);
						config_status = VALUE_SAVED;
					}
				}
				param1 = wt_trip.prm;
				param2 = wt_trip_delay.prm;
		break;
		
		case PAGE_CONFIG_MIN + 2:
				if(sel == LEFT) // Fault Enable 
				{
					if(cmd == INC)
					{
						flt_enable.prm = (flt_enable.prm >= flt_enable.max) ? flt_enable.max : flt_enable.prm + flt_enable.step;
					}else if(cmd == DEC)
					{
						flt_enable.prm = (flt_enable.prm <= flt_enable.min) ? flt_enable.min : flt_enable.prm - flt_enable.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_FLT_EN_ADDR,flt_enable.prm);
						config_status = VALUE_SAVED;
					}
				}
				if(sel == RIGHT) // Fault Enable Delay
				{
					if(cmd == INC)
					{
						flt_enable_delay.prm = (flt_enable_delay.prm >= flt_enable_delay.max) ? flt_enable_delay.max : flt_enable_delay.prm + flt_enable_delay.step;
					}else if(cmd == DEC)
					{
						flt_enable_delay.prm = (flt_enable_delay.prm <= flt_enable_delay.min) ? flt_enable_delay.min : flt_enable_delay.prm - flt_enable_delay.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_FLT_EN_DELAY_ADDR,flt_enable_delay.prm);
						config_status = VALUE_SAVED;
					}
				}
				param1 = flt_enable.prm;
				param2 = flt_enable_delay.prm;
		break;
		
		case PAGE_CONFIG_MIN + 3:
				if(sel == LEFT) // Fault Disable
				{
					if(cmd == INC)
					{
						flt_disable.prm = (flt_disable.prm >= flt_disable.max) ? flt_disable.max : flt_disable.prm + flt_disable.step;
					}else if(cmd == DEC)
					{
						flt_disable.prm = (flt_disable.prm <= flt_disable.min) ? flt_disable.min : flt_disable.prm - flt_disable.step;
					}else if(cmd == SET)
					{
						eeprom_update_word((uint16_t*)EEP_FLT_DIS_ADDR,flt_disable.prm);
						config_status = VALUE_SAVED;
					}
				}
				param1 = flt_disable.prm;
				
		break;
		
		case PAGE_CONFIG_MIN + 4:
		
		break;
		
		case PAGE_CONFIG_MIN + 5:
		
		break;
		
	}
	
	if(param1 == lop_trip.prm)
	{
		lop_f = (float)(param1*0.01);
		roundf(lop_f*10.0f)/10.0f;
	}else
	{
		itoa(param1,cparam1,10);
	}
	
	itoa(param2,cparam2,10);
	
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center(cparam1,0,63,LAYOUT_PARAM1_Y,60);
	ks0108Puts_mod_align_center(cparam2,64,127,LAYOUT_PARAM2_Y,60);
	
	ks0108SelectFont(sanserif_9,ks0108ReadFontData, BLACK);
	display_status(config_status);
	
}
void config_main()
{
	display_update_disable();
	page_num = PAGE_CONFIG_MIN;
	display_page_select(page_num);

	SELECT select = LEFT;
	COMMAND command = DISPLAY;
	config_page_update(page_num,select,command);
	config_draw_cursor(select);
	while(in_config)
	{
		
		uint8_t keyp = 0;
		
		if (SW_RIGHT)
		{
			_delay_ms(300);	
			if(select == LEFT)
			{
				select = RIGHT;	
				command = DISPLAY;
				keyp = 1;
			}else
			{
				select = (page_num == (PAGE_CONFIG_MAX)) ? RIGHT : LEFT;
				command = DISPLAY;
				page_num = (page_num >= PAGE_CONFIG_MAX) ? PAGE_CONFIG_MAX : page_num + 1;
				keyp = 1;
			}
		}
		else if(SW_LEFT)
		{
			_delay_ms(300);
			if(select == RIGHT)
			{
				select = LEFT; 
				command = DISPLAY;
				keyp = 1;
			}else
			{
				select = LEFT;
				command = DISPLAY;
				page_num = (page_num <= PAGE_CONFIG_MIN) ? PAGE_CONFIG_MIN : page_num - 1;
				keyp = 1;
			}
			
		} 
		else if(SW_UP)
		{
			_delay_ms(300);
			command = INC;
			keyp = 1;
		}
		else if(SW_DOWN)
		{
			_delay_ms(300);
			command = DEC;
			keyp = 1;
		}
		else if(SW_ENTER_P)
		{
			_delay_ms(300);
			command = SET;
			keyp = 1;
		} 
		
		if(keyp)
		{
			display_page_select(page_num);
			config_page_update(page_num,select,command);
			config_draw_cursor(select);
		}
		
	}
}
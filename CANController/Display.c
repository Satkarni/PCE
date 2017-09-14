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


volatile uint8_t brightness = 0xFF;
volatile uint8_t hours_counter = 0;
volatile uint8_t past_dtc_request = 0;
volatile uint8_t diagnostics_counter = 0;



page page1 =  { .page_nu = 1, .label1 = "Speed", .label2 = "Oil Pressure", .unit1 = "RPM", .unit2 = " bar "};
page page2 =  { .page_nu = 2, .label1 = "Coolant Temp", .label2 = "Oil Temp", .unit1 = "deg C", .unit2 = "deg C"};
page page3 =  {	.page_nu = 3, .label1 = "Boost Pressure", .label2 = "Boost Temp", .unit1 = " bar ", .unit2 = "deg C"};
page page4 =  { .page_nu = 4, .label1 = "Coolant Level", .label2 = "Oil Level", .unit1 = "", .unit2 = " % "};
page page5 =  { .page_nu = 5, .label1 = "Eng Torque", .label2 = "Eng Torque", .unit1 = "Actual % ", .unit2 = "Demand % "};
page page6 =  { .page_nu = 6, .label1 = "Acc Pedal Pos", .label2 = "Fuel Temp", .unit1 = "%", .unit2 = "deg C"};
page page7 =  { .page_nu = 7, .label1 = "Eng Load", .label2 = "Max Torque", .unit1 = "%", .unit2 = "Available %"};
page page8 =  { .page_nu = 8, .label1 = "Total Hrs.", .label2 = "Total Rev", .unit1 = "Hours", .unit2 = "Rev"};
page page9 =  { .page_nu = 9, .label1 = "Eng Protect", .label2 = "Eng Shutoff", .unit1 = "Shutoff", .unit2 = "Override"};

page page_config1 =  { .page_nu = PAGE_CONFIG_MIN, .label1 = "", .label2 = "", .unit1 = "", .unit2 = "" };
page page_config2 =	 { .page_nu = PAGE_CONFIG_MIN+1, .label1 = "", .label2 = "", .unit1 = "", .unit2 = ""};
			

void display_init(uint8_t bright)
{
	
	Enter_Critical_Section   	
	engine_data.status = READY;
	Leave_Critical_Section
	
	display_PWM_init();
	ks0108Init(0);
	ks0108ClearScreen();
	for(uint8_t i=0;i<255;i++)
	{
		display_PWM_set_duty(i);
		_delay_ms(8);
	}
	_delay_ms(2);				// little delay for GLCD
	
	ks0108Init(1);
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108LoadBitmap(pce_logo);
	
	_delay_ms(2000);
	display_PWM_set_duty(bright);
	ks0108ClearScreen();
	ks0108Init(0);
	
	display_update_init();
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
	uint8_t i;
	i = brightness;
	brightness = (brightness == 0) ? 0	: (brightness - 51);
	for(;i>brightness;i--)
	{
		display_PWM_set_duty(i);
		_delay_ms(7);
	}
	//display_PWM_set_duty(brightness);
}

ISR(INT4_vect) // Brightness ++
{
	uint8_t i;
	i = brightness;
	brightness = (brightness == 0xFF) ? 0xFF : (brightness + 51);
	for(;i<brightness;i++)
	{
		display_PWM_set_duty(i);
		_delay_ms(7);
	}
	//display_PWM_set_duty(brightness);
}

void display_loading_text()
{
	Enter_Critical_Section
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
	ks0108Puts_mod_align_center("Loading",0,127,1,100);
	ks0108InvertRect(0,0,127,10);
	Leave_Critical_Section
}

void display_function_text(uint8_t msg)
{
	Enter_Critical_Section
	ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
	switch (msg)
	{
		case 0:
		ks0108FillRect(0,15,127,10,WHITE);
		ks0108Puts_mod_align_center("Initialising I/O Ports",0,127,15,100);
		break;
		
		case 1:
		ks0108FillRect(0,15,127,10,WHITE);
		ks0108Puts_mod_align_center("Registering J1939 Filters",0,127,15,100);
		break;
		
		case 2:
		ks0108FillRect(0,15,127,10,WHITE);
		ks0108Puts_mod_align_center("Calibrating Interface Elements",0,127,15,100);
		break;
		
		case 3:
		ks0108ClearScreen();
		ks0108Puts_mod_align_center("Diagnostics",0,127,15,100);
		ks0108Puts_mod_align_center("and",0,127,25,100);
		ks0108Puts_mod_align_center("Configuration",0,127,35,100);
		break;
	}
	Leave_Critical_Section
}

void display_loading_bar()
{
	Enter_Critical_Section
	ks0108DrawHoriLine(22,40,87,BLACK);
	ks0108DrawHoriLine(22,45,87,BLACK);
	ks0108DrawVertLine(22,40,5,BLACK);
	ks0108DrawVertLine(109,40,5,BLACK);
	
	Leave_Critical_Section
	
}

void display_update_load(uint8_t lag, uint8_t initial, uint8_t final)
{
	Enter_Critical_Section
	uint8_t z;
	uint8_t x1,x2,x;
	x1 = initial + 22;
	x2 = final + 22;
	for (x=22;x1>=x;x++)
		ks0108DrawVertLine(x,40,5,BLACK);
		
	for (x=x1;x2>=x;x++)
	{
		ks0108DrawVertLine(x,40,5,BLACK);
		for(z=0;z<lag;z++)
			_delay_ms(1);
	}
	Leave_Critical_Section
}

void display_fancy_effect()
{
	uint8_t i;
	for(i=0;i<=63;i++)
	{
		ks0108SetDot((i*2)-1,10,BLACK);
		ks0108SetDot(126-(i*2),53,BLACK);
		ks0108SetDot(5,i,BLACK);
		ks0108SetDot(121,63-i,BLACK);
		ks0108SetDot(i*2,10,BLACK);
		ks0108SetDot(127-(i*2),53,BLACK);
		_delay_ms(10);
	}
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
		
		case CAN_TIMEOUT :
		break;
		
		case CAN_ERROR :
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
	Leave_Critical_Section
}

void display_hours_draw()
{
	Enter_Critical_Section
	
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center("Hours of Operation",0,127,0,100);
	ks0108DrawHoriLine(0,40,127,BLACK);
	
	Leave_Critical_Section	
}

void display_config_page(page *cpage)
{
	Enter_Critical_Section
	
	switch (cpage->page_nu)
	{
		case (PAGE_CONFIG_MIN) :
			ks0108ClearScreen();
			ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
			ks0108Puts_mod_align_center("Active DTCs (SPN Conv: v2)",0,127,1,100);
			ks0108Puts_mod("SPN",23,11,5);
			ks0108Puts_mod("FMI",60,11,5);
			ks0108Puts_mod("OCC",90,11,5);
			ks0108InvertRect(0,10,127,8);
		break;
		
		case (PAGE_CONFIG_MIN+1) :
			ks0108ClearScreen();
			ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
			ks0108Puts_mod_align_center("Past DTCs (SPN Conv: v2)",0,127,1,100);
			ks0108Puts_mod("SPN",23,11,5);
			ks0108Puts_mod("FMI",60,11,5);
			ks0108Puts_mod("OCC",90,11,5);
			ks0108InvertRect(0,10,127,8);
		break;
	}
	Leave_Critical_Section	
}

void display_config_menu(uint8_t highlight)
{
	Enter_Critical_Section
	if(0 == highlight)
	{
		ks0108Puts_mod_align_center("Clear Active DTC",0,127,42,100);
		ks0108InvertRect(0,41,127,9);
		ks0108FillRect(0,52,127,9,WHITE);
		ks0108Puts_mod_align_center("Clear Past DTC",0,127,53,100);
		
	}
	else if(1 == highlight)
	{
		ks0108Puts_mod_align_center("Clear Past DTC",0,127,53,100);
		ks0108InvertRect(0,52,127,9);
		ks0108FillRect(0,41,127,9,WHITE);
		ks0108Puts_mod_align_center("Clear Active DTC",0,127,42,100);
	}
	Leave_Critical_Section
}

void display_about_page()
{
	Enter_Critical_Section
	ks0108ClearScreen();
	ks0108Puts_mod_align_center("About",0,127,1,100);
	ks0108InvertRect(0,0,127,10);
	ks0108Puts_mod_align_center("Power Control Engineers",0,127,12,100);
	ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
	ks0108Puts_mod("Software Version: 1.0",0,22,100);
	ks0108Puts_mod("Authors:",0,32,100);
	ks0108Puts_mod("Rohan Pandit",38,32,100);
	ks0108Puts_mod("Piyush Puranik",38,42,100);
	ks0108Puts_mod("Contact: +91 20 25291011",0,52,100);
	Leave_Critical_Section
}

void display_active_dtc()
{
	Enter_Critical_Section
	uint8_t i,j;
	int8_t first = 0,last = 0;
	char tempspn[6],tempfmi[3],tempocc[3];
	char occ[4];
	char srno_char[4];
	char totalno_char[4];
	static uint8_t old_fmi[5], old_occ[5], nodata, page_number, max_pages;
	static uint32_t old_spn[5];
	
	if(1 == DG.refresh)
	{
		for(i = 0; i < 5; i++)
		{
			old_spn[i] = 0;
			old_occ[i] = 0;
			old_fmi[i] = 0;
		}
		page_number = 1;
		max_pages = 1;
		DG.refresh = 0;
	}
	
	max_pages = (uint8_t)ceil((double)DG.DM_number/4);
	
	if (1 == DG.page_increment)
	{
		page_number = (page_number < max_pages) ? page_number+1 : max_pages;
		DG.page_increment = 0;
		nodata = 1;
		for(i = 0; i < 5; i++)
		{
			old_spn[i] = 0;
			old_occ[i] = 0;
			old_fmi[i] = 0;
		}
	}
	if (1 == DG.page_decrement)
	{
		page_number = (page_number > 1) ? page_number-1 : 1;
		DG.page_decrement = 0;
		nodata = 1;
		for(i = 0; i < 5; i++)
		{
			old_spn[i] = 0;
			old_occ[i] = 0;
			old_fmi[i] = 0;
		}
	}
	
	if (DG.DM_number < 4)
	{
		first = 0;
		last = DG.DM_number;
	}
	else
	{
		if(1 == page_number)
		{
			ks0108DrawPageArrow(0);
			first = 0;
			last = 3;
		}
		else if(1 < page_number && page_number < max_pages)
		{
			ks0108DrawPageArrow(0);
			ks0108DrawPageArrow(1);
			first = (page_number - 1)*4;
			last = (DG.DM_number < (first+4)) ? DG.DM_number : first+4;
		}
		else if (page_number == max_pages)
		{
			ks0108DrawPageArrow(1);
			first = (page_number - 1)*4;
			last = DG.DM_number-1;
		}
	}
	
	if (0 == DG.DM_number)
	{
		ks0108Puts_mod_align_center("NO DATA",0,127,31,100);
		nodata = 1;
	}
	else
	{
		if (1 == nodata)
		{
			ks0108FillRect(0,20,127,43,WHITE);
			nodata = 0;
		}
		for (j=0,i=first;i<=last;i++,j++)
		{
			
			itoa(page_number,srno_char,10);
			itoa(max_pages,totalno_char,10);
			
			ks0108Puts_mod(srno_char,118,30,5);
			ks0108DrawHoriLine(116,39,8,BLACK);
			ks0108Puts_mod(totalno_char,118,42,5);
			
			if (DM1[i].spn != old_spn[i])
			{
				dtostrf(DM1[i].spn,1,0,tempspn);
				ks0108FillRect(25,((j+1)*10)+11,15,6,WHITE);
				ks0108Puts_mod(tempspn,25,((j+1)*10)+11,15);
				old_spn[i] = DM1[i].spn;
			}
			if (DM1[i].fmi != old_fmi[i])
			{
				itoa(DM1[i].fmi,tempfmi,10);
				//ks0108FillRect(60,((i+1)*9)+11,20,5,WHITE);
				ks0108Puts_mod(tempfmi,65,((j+1)*10)+11,15);
				old_fmi[i] = DM1[i].fmi;
			}
			if (DM1[i].occ != old_occ[i])
			{
				itoa(DM1[i].occ,tempocc,10);
				//ks0108FillRect(90,((i+1)*9)+11,20,5,WHITE);
				ks0108Puts_mod(tempocc,95,((j+1)*10)+11,15);
				old_occ[i] = DM1[i].occ;
			}
			
			//itoa(DM1[i].conversion,conv,10);
			//ks0108Puts_mod(conv,70,((i+1)*9)+2,10);
		}
	}
	Leave_Critical_Section
}

void display_past_dtc()
{
	Enter_Critical_Section
	uint8_t i,j;
	int8_t first = 0,last = 0;
	char tempspn[6],tempfmi[3],tempocc[3];
	char occ[4];
	char srno_char[4];
	char totalno_char[4];
	static uint8_t old_fmi[5], old_occ[5], nodata, page_number, max_pages;
	static uint32_t old_spn[5];
	
	if(1 == DG2.refresh)
	{
		for(i = 0; i < 5; i++)
		{
			old_spn[i] = 0;
			old_occ[i] = 0;
			old_fmi[i] = 0;
		}
		page_number = 1;
		max_pages = 1;
		DG2.refresh = 0;
	}
	
	max_pages = (uint8_t)ceil((double)DG2.DM_number/4);
	
	if (1 == DG2.page_increment)
	{
		page_number = (page_number < max_pages) ? page_number+1 : max_pages;
		DG2.page_increment = 0;
		nodata = 1;
		for(i = 0; i < 5; i++)
		{
			old_spn[i] = 0;
			old_occ[i] = 0;
			old_fmi[i] = 0;
		}
	}
	if (1 == DG2.page_decrement)
	{
		page_number = (page_number > 1) ? page_number-1 : 1;
		DG2.page_decrement = 0;
		nodata = 1;
		for(i = 0; i < 5; i++)
		{
			old_spn[i] = 0;
			old_occ[i] = 0;
			old_fmi[i] = 0;
		}
	}
	
	if (DG2.DM_number < 4)
	{
		first = 0;
		last = DG2.DM_number;
	}
	else
	{
		if(1 == page_number)
		{
			ks0108DrawPageArrow(0);
			first = 0;
			last = 3;
		}
		else if(1 < page_number && page_number < max_pages)
		{
			ks0108DrawPageArrow(0);
			ks0108DrawPageArrow(1);
			first = (page_number - 1)*4;
			last = (DG2.DM_number < (first+4)) ? DG2.DM_number : first+4;
		}
		else if (page_number == max_pages)
		{
			ks0108DrawPageArrow(1);
			first = (page_number - 1)*4;
			last = DG2.DM_number-1;
		}
	}
	
	if (0 == DG2.DM_number)
	{
		ks0108Puts_mod_align_center("NO DATA",0,127,31,100);
		nodata = 1;
	}
	else
	{
		if (1 == nodata)
		{
			ks0108FillRect(0,20,127,43,WHITE);
			nodata = 0;
		}
		for (j=0,i=first;i<=last;i++,j++)
		{
			
			itoa(page_number,srno_char,10);
			itoa(max_pages,totalno_char,10);
			
			ks0108Puts_mod(srno_char,118,30,5);
			ks0108DrawHoriLine(116,39,8,BLACK);
			ks0108Puts_mod(totalno_char,118,42,5);
			
			if (DM2[i].spn != old_spn[i])
			{
				dtostrf(DM2[i].spn,1,0,tempspn);
				ks0108Puts_mod(tempspn,25,((j+1)*10)+11,15);
				old_spn[i] = DM2[i].spn;
			}
			if (DM2[i].fmi != old_fmi[i])
			{
				itoa(DM2[i].fmi,tempfmi,10);
				//ks0108FillRect(60,((i+1)*9)+11,20,5,WHITE);
				ks0108Puts_mod(tempfmi,65,((j+1)*10)+11,15);
				old_fmi[i] = DM2[i].fmi;
			}
			if (DM2[i].occ != old_occ[i])
			{
				itoa(DM2[i].occ,tempocc,10);
				//ks0108FillRect(90,((i+1)*9)+11,20,5,WHITE);
				ks0108Puts_mod(tempocc,95,((j+1)*10)+11,15);
				old_occ[i] = DM2[i].occ;
			}
			
			//itoa(DM2[i].conversion,conv,10);
			//ks0108Puts_mod(conv,70,((i+1)*9)+2,10);
		}
	}
	Leave_Critical_Section
}

void display_can_failure()
{
	Enter_Critical_Section
	ks0108ClearScreen();
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center(" CAN BUS Failure",0,127,10,127);
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	ks0108Puts_mod_align_center("Check CAN Connections",0,127,40,127);
	Leave_Critical_Section
}

void display_message(ALARM_MSG index)
{
	switch(index)
	{
		case 0 :
			ks0108Puts_mod_align_center("Safety Disabled",0,127,42,127);
		break;
		
		case 1 :
			ks0108Puts_mod_align_center("Safety Enabled",0,127,42,127);
		break;
		
		case 2 :
			ks0108Puts_mod_align_center("Overspeed Alarm",0,127,42,127);
		break;
		
		case 3:
			ks0108Puts_mod_align_center("Coolant Temp High",0,127,42,127);
		break;
		
		case 4 :
			ks0108Puts_mod_align_center("Oil Pressure Low",0,127,42,127);
		break;
		
		case 5 :
			ks0108Puts_mod_align_center("Coolant Level Low",0,127,42,127);
		break;
	}
}

void display_page_update(Engine_Data* p_en_data, uint8_t page_n)
{
	Enter_Critical_Section
	
	char rpm_char[16];
	char act_eng_torque_char[16];
	char dem_eng_torque_char[16];
	
	float acc_pedal_f;
	char acc_pedal_char[16];
	char eng_percent_load_char[16];
	
	char ect_char[16];
	char fuel_temp_char[16];
	float lot_f;
	char lot_char[16];
	
	char lop_char[16];
	char oil_level_char[16];
	float oil_level_f;
	float coolant_level_f;
	char coolant_level_char[16];
	
	char trip_fuel_char[16];
	char total_fuel_char[16];
	float trip_fuel_f, total_fuel_f;
	
	char hours_char[16];
	char rev_char[16];
	float hours_f, rev_f;
	
	char boost_pressure_char[16];
	float boost_pressure_f;
	char boost_temperature_char[16];
	
	float bat_vge_f, lop_f;	
	static uint8_t old_idx;
	static STATUS status_old;
	static uint32_t old_parameter1 = 0xFFFFFFFF, old_parameter2 = 0xFFFFFFFF;
	
	char tmp1[2];
	
	char spn_string[6];
	char fmi_string[2];
	
	uint32_t tmp18;
	
	if(page_n < 11)
	{
		static ALARM_MSG old_alarm_msg = DEFAULT;
		if (old_alarm_msg != p_en_data->alarm_msg || old_page_n != page_n)
		{
			display_message(p_en_data->alarm_msg);
			old_alarm_msg = p_en_data->alarm_msg;
			old_page_n = page_n;
		}
	}
	else
	{
		old_page_n = 0;
	}
	if(reset_page_update == 1)
	{
		old_page_n = 0;
		reset_page_update = 0;
		display_message(p_en_data->alarm_msg);
	}
	switch(page_n)
	{
		case 1 : //Engine Speed, Oil Pressure
			
			if (old_parameter1 != p_en_data->rpm || old_parameter2 != p_en_data->lop || old_page_n != page_n)
			{
				if(p_en_data->rpm > 64255)
				{
					strcpy(rpm_char,"No Data");
					//p_en_data->rpm = 0;
				}	
				else
				{
					itoa((p_en_data->rpm)*0.125,rpm_char,10);
				}
				if (p_en_data->lop > 250)
				{
					strcpy(lop_char,"No Data");
				}
				else
				{
					lop_f = (float)(p_en_data->lop)*0.04;
					dtostrf(lop_f,4,2,lop_char);
				}
				
				old_parameter1 = p_en_data->rpm;
				old_parameter2 = p_en_data->lop;
				old_page_n = page_n;

				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				ks0108Puts_mod_align_center(rpm_char,0,63,LAYOUT_PARAM1_Y,60);
				ks0108Puts_mod_align_center(lop_char,64,127,LAYOUT_PARAM1_Y,60);
			}
			//
		break;
		
		case 2 : //Oil Temp, Coolant Temp
		
		if (old_parameter1 != p_en_data->ect || old_parameter2 != p_en_data->lot || old_page_n != page_n)
		{
			if(p_en_data->ect > 250)
			{
				strcpy(ect_char,"No Data");
			}
			else
			{
				itoa(((p_en_data->ect)-40),ect_char,10);
			}
			if (p_en_data->lot > 64255)
			{
				strcpy(lot_char,"No Data");
			}
			else
			{
				lot_f = (float)(p_en_data->lot)*0.03125;
				lot_f -= 273;
				dtostrf(lot_f,1,0,lot_char);
			}
			
			old_parameter1 = p_en_data->ect;
			old_parameter2 = p_en_data->lot;
			old_page_n = page_n;
			
			ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
			ks0108Puts_mod_align_center(ect_char,0,63,LAYOUT_PARAM1_Y,60);
			ks0108Puts_mod_align_center(lot_char,64,127,LAYOUT_PARAM2_Y,60);
			
			
		}
		break;
		
		case 3 : //Boost Pressure, Boost Temperature
		
		if (old_parameter1 != p_en_data->boost_pressure || old_parameter2 != p_en_data->boost_temperature || old_page_n != page_n)
		{
			if (p_en_data->boost_temperature > 250)
				strcpy(boost_temperature_char,"No Data");
			else
				itoa(((p_en_data->boost_temperature)-40),boost_temperature_char,10);
				
			if (p_en_data->boost_pressure > 250)
				strcpy(boost_pressure_char,"No Data");
			else
			{
				boost_pressure_f = (float)(p_en_data->boost_pressure)*0.02;
				dtostrf(boost_pressure_f,4,2,boost_pressure_char);
			}
			
			old_parameter1 = p_en_data->boost_pressure;
			old_parameter2 = p_en_data->boost_temperature;
			old_page_n = page_n;
			
			ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
			ks0108Puts_mod_align_center(boost_pressure_char,0,63,LAYOUT_PARAM1_Y,60);
			ks0108Puts_mod_align_center(boost_temperature_char,64,127,LAYOUT_PARAM1_Y,60);	
		}
		break;
		
		case 4 : // Oil Level, Coolant Level
		
				if (old_parameter1 != p_en_data->oil_level || old_parameter2 != p_en_data->coolant_level || old_page_n != page_n)
				{
					if (p_en_data->oil_level > 250)
					{
						strcpy(oil_level_char,"No Data");
					}
					else
					{
						oil_level_f = (float)(p_en_data->oil_level)*0.4;
						dtostrf(oil_level_f,3,1,oil_level_char);
					}
					
					if(p_en_data->coolant_level == 1)
					{
						strcpy(coolant_level_char,"Ok");
					}
					if(p_en_data->coolant_level == 2)
					{
						strcpy(coolant_level_char,"Low");
					}
					
					old_parameter1 = p_en_data->oil_level;
					old_parameter2 = p_en_data->coolant_level;
					old_page_n = page_n;
					
					ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
					ks0108Puts_mod_align_center(oil_level_char,64,127,LAYOUT_PARAM2_Y,60);
					ks0108Puts_mod_align_center(coolant_level_char,0,63,LAYOUT_PARAM2_Y,60);	
				}
			
		break;	
		
		
		case 5 : //Engine Torque
		
			
			if (old_parameter1 != p_en_data->act_eng_torque || old_parameter2 != p_en_data->dem_eng_torque || old_page_n != page_n)
			{
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				
				itoa((p_en_data->act_eng_torque)-125,act_eng_torque_char,10);
				if(250<p_en_data->act_eng_torque)
					strcpy(act_eng_torque_char,"No Data");
				ks0108Puts_mod_align_center(act_eng_torque_char,0,63,LAYOUT_PARAM2_Y,60);
				
				itoa((p_en_data->dem_eng_torque)-125,dem_eng_torque_char,10);
				if(250<p_en_data->act_eng_torque)
					strcpy(dem_eng_torque_char,"No Data");
				ks0108Puts_mod_align_center(dem_eng_torque_char,64,127,LAYOUT_PARAM2_Y,60);
				
				old_parameter1 = p_en_data->act_eng_torque;
				old_parameter2 = p_en_data->dem_eng_torque;
				old_page_n = page_n;
			}
			
			
		
		break;
		
		case 6: //Accelerator Pedal Position, Fuel Temperature
			
			if (old_parameter1 != p_en_data->acc_pedal_pos1 || old_parameter2 != p_en_data->fuel_temp || old_page_n != page_n)
			{
				if (p_en_data->acc_pedal_pos1 > 250)
				{
					strcpy(acc_pedal_char,"No Data");
				}
				else
				{
					acc_pedal_f = (float)(p_en_data->acc_pedal_pos1)*0.4;
					dtostrf(acc_pedal_f,2,2,acc_pedal_char);
				}
				if (p_en_data->fuel_temp > 250)
				{
					strcpy(fuel_temp_char,"No Data");
				} 
				else
				{
					itoa(((p_en_data->fuel_temp)-40),fuel_temp_char,10);
				}
				
				
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				ks0108Puts_mod_align_center(acc_pedal_char,0,63,LAYOUT_PARAM1_Y,60);
				ks0108Puts_mod_align_center(fuel_temp_char,64,127,LAYOUT_PARAM1_Y,60);
				
				old_parameter1 = p_en_data->acc_pedal_pos1;
				old_parameter2 = p_en_data->fuel_temp;
				old_page_n = page_n;
			}
			
		break;
		
		case 7 : //Engine Load Percentage, Maximum Available Torque Percentage
		
			if (old_parameter1 != p_en_data->max_torque_percent || old_parameter2 != p_en_data->eng_percent_load || old_page_n != page_n)
			{
				acc_pedal_f = (float)(p_en_data->max_torque_percent)*0.4;
				dtostrf(acc_pedal_f,2,2,acc_pedal_char);
				
				itoa((p_en_data->eng_percent_load),eng_percent_load_char,10);
				
				if(125<p_en_data->eng_percent_load)
				strcpy(eng_percent_load_char,"No Data");
				
				if(100<acc_pedal_f)
				strcpy(eng_percent_load_char,"No Data");
				
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				ks0108Puts_mod_align_center(eng_percent_load_char,0,63,LAYOUT_PARAM2_Y,60);
				ks0108Puts_mod_align_center(acc_pedal_char,64,127,LAYOUT_PARAM2_Y,60);
				
				old_parameter1 = p_en_data->max_torque_percent;
				old_parameter2 = p_en_data->eng_percent_load;
				old_page_n = page_n;
			}

		break;
		
		case 8 : //Hours of Operation
			hours_counter++;
			if(4 < hours_counter)
			{
				tmp18 = 0x18;
				hours_counter = 0;
				j1939_request(	tmp18<<24 | (PGN_HOURS<<8) | 0xFF );
			}
			if (old_parameter1 != p_en_data->hours || old_page_n != page_n)
			{
				hours_f = (float)(p_en_data->hours)*0.05;
				dtostrf(hours_f,3,2,hours_char);
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				ks0108Puts_mod_align_center(hours_char,0,127,LAYOUT_PARAM1_Y,120);
				
				old_parameter1 = p_en_data->hours;
				old_page_n = page_n;
			}
		
		break;
		
		case 9 : //Engine Shutoff
			ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
			if (old_parameter1 != p_en_data->eng_protection_shutoff || old_parameter2 != p_en_data->eng_shutoff_override || old_page_n != page_n)
			{
				switch(p_en_data->eng_protection_shutoff)
				{
					case 0:
					ks0108Puts_mod_align_center("Off",0,63,LAYOUT_PARAM2_Y,60);
					break;
					
					case 1:
					ks0108Puts_mod_align_center("On",0,63,LAYOUT_PARAM2_Y,60);
					break;
					
					case 2:
					ks0108Puts_mod_align_center("Error",0,63,LAYOUT_PARAM2_Y,60);
					break;
					
					case 3:
					ks0108Puts_mod_align_center("N/A",0,63,LAYOUT_PARAM2_Y,60);
					break;
				}
				
				switch(p_en_data->eng_shutoff_override)
				{
					case 0:
					ks0108Puts_mod_align_center("Off",64,127,LAYOUT_PARAM2_Y,60);
					break;
					
					case 1:
					ks0108Puts_mod_align_center("On",64,127,LAYOUT_PARAM2_Y,60);
					break;
					
					case 2:
					ks0108Puts_mod_align_center("Error",64,127,LAYOUT_PARAM2_Y,60);
					break;
					
					case 3:
					ks0108Puts_mod_align_center("N/A",64,127,LAYOUT_PARAM2_Y,60);
					break;
				}
				
				old_parameter1 = p_en_data->eng_protection_shutoff;
				old_parameter2 = p_en_data->eng_shutoff_override;
				old_page_n = page_n;
			}
		
		break;
		
		case 11:
			display_active_dtc();
			old_parameter1 = 0xFFFFFFFF;
			old_parameter2 = 0xFFFFFFFF;
		break;
			
		case 12:
			tmp18 = 0x18;
			past_dtc_request++;
			if(past_dtc_request > 10)
			{
				past_dtc_request = 0;
				j1939_request(tmp18<<24 | PGN_DM2<<8 | 0xFF );
			}
			display_past_dtc();	
			old_parameter1 = 0xFFFFFFFF;
			old_parameter2 = 0xFFFFFFFF;
		break;
		
		case 25 :
		
		break;
			
			
		
		
	}
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
	/*
	if(p_en_data->status != status_old || in_config_old || page_n != old_page_n)
	{
		display_status(p_en_data->status);
		status_old = p_en_data->status;
	}
	
	if(p_en_data->idx != MSG_MAX_NUM && (p_en_data->idx != old_idx || in_config_old || page_n != old_page_n) )
	{
		ks0108FillRect(LAYOUT_MSG_X,LAYOUT_MSG_Y,127,9,WHITE);
		ks0108Puts_mod_align_center(p_en_data->message[p_en_data->idx],0,127,LAYOUT_MSG_Y,127);
		old_idx = p_en_data->idx;
		old_page_n = page_n;
		in_config_old = 0;
	}
	*/
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
		
		case PAGE_MIN+2:
			display_page_draw(&page3);
		break;
			
		case PAGE_MIN+3:
			display_page_draw(&page4);
		break;
				
		case PAGE_MIN+4:
			display_page_draw(&page5);
		break;
					
		case PAGE_MIN+5:
			display_page_draw(&page6);
		break;
		
		case PAGE_MIN+6:
			display_page_draw(&page7);
		break;
		
		case PAGE_MIN+7:
			display_hours_draw();
			//display_page_draw(&page8);
		break;
		
		case PAGE_MIN+8:
			display_page_draw(&page9);
		break;
		
		case PAGE_CONFIG_MIN:
			display_config_page(&page_config1);
		break;
		
		case PAGE_CONFIG_MIN+1:
			display_config_page(&page_config2);
		break;
		
		//case PAGE_CONFIG_MAX:
			//display_about_page();
		//break;
		
		/*
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
		*/
	}
	Leave_Critical_Section
}
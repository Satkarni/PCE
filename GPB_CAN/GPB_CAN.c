/*
 * Copyright:      Power Control Engineers  
 * Author:         Rohan Pandit
 * Remarks:        
 * Version:        01
 * Description:    Main routine for Goa Police Boat controller
 * 
 */

#include "GPB_CAN.h"

volatile uint8_t j1939_timeout_flag;

void Ext_Int_init()
{
	EICRA |= ((1<<ISC01)|(1<<ISC00)) ;  //Rising edge INT0 (brightness--)
	EICRB |= ((1<<ISC71)|(1<<ISC70)|(1<<ISC61)|(1<<ISC60)|(1<<ISC41)|(1<<ISC40)); // Rising edge INT7 (mute), INT4 ( brightness++), INT6 (Back)
	EIMSK |= ((1<<INT0)|(1<<INT4)|(1<<INT6)|(1<<INT7)); // INT0, INT4, INT6, INT7 ext interrupt enabled
}
ISR(INT7_vect) // Mute buzzer
{
	engine_data.mute = 1;
	
}

ISR(INT6_vect) // Back key or Reset key
{
	if(in_config)
	{
		in_config = 0;
		in_config_old = 1;
		page_num = PAGE_MIN;
		display_page_select(page_num);
		display_update_enable(UPDATE_RATE);
	}
	else if(in_version)
	{
		in_version = 0;
		in_version_old = 1;
		page_num = PAGE_MIN;
		display_page_select(page_num);
		display_update_enable(UPDATE_RATE);
	}else
	{
		engine_data.reset = 1;
	}
}

ISR(TIMER1_COMPA_vect)		// Main update interrupt 
{
	
	display_page_update(&engine_data,page_num,j1939_timeout_flag);
	
	if(ect_sensor.tmr.en)
	{
		ect_sensor.tmr.acc++;
		if(ect_sensor.tmr.acc > (wt_trip_delay.prm)*2)
		{
			ect_sensor.tmr.done = 1;
			ect_sensor.tmr.oneshot = 1;
			ect_sensor.tmr.acc = 0;
		}
	}else
	{
		ect_sensor.tmr.done = 0;
		ect_sensor.tmr.acc = 0;
	}
	
	if(lop_sensor.tmr.en)
	{
		lop_sensor.tmr.acc++;
		if(lop_sensor.tmr.acc > (lop_trip_delay.prm)*2)
		{
			lop_sensor.tmr.done = 1;
			ect_sensor.tmr.oneshot = 1;
			lop_sensor.tmr.acc = 0;
		}
	}else
	{
		lop_sensor.tmr.done = 0;
		lop_sensor.tmr.acc = 0;
	}
	
	switch(engine_data.status)
	{
			
		case READY :	
			if(engine_data.fault_enable)		// fault enable timer
			{
				engine_data.fault.acc++;
				if(engine_data.fault.acc > (flt_enable_delay.prm)*2)
				{
					engine_data.fault.done = 1;
					engine_data.fault.oneshot = 1;
					engine_data.fault.acc = 0;
				}
			}else
			{
				engine_data.fault.done = 0;
				engine_data.fault.acc = 0;
			}
		break;
		
		case RUNNING :
				engine_data.fault.done = 0;
				engine_data.fault.acc = 0;
		break;
		
		case TRIP :
				if(!(ect_sensor.trip || lop_sensor.trip))
				{
					engine_data.status = READY;
				}
		break;
		
		default:
		break;
	}
}

ISR(TIMER3_COMPA_vect)		// CAN Timeout interrupt
{
	static uint8_t j1939_timeout;
	
	if( ++j1939_timeout == J1939_TIMEOUT )		// CAN Timeout
	{
		j1939_timeout_flag = 1;
		j1939_timeout = 0;
	}
}	  
void j1939_timeout_init()
{
	TCCR3B |= ((0<<WGM33)|(1<<WGM32));		// CTC Mode
}

void j1939_timeout_enable(uint16_t rate)
{
	Enter_Critical_Section
	OCR3A = rate;
	Leave_Critical_Section
	TIMSK3 |= (1<<OCIE3A);
	TCCR3B |= ((1<<CS32)|(0<<CS31)|(0<<CS30));	// clk/256  prescaler
}

void j1939_timeout_disable()
{
	TCCR3B &= ~((1<<CS32)|(1<<CS31)|(1<<CS30));	
	TIMSK3 &= ~(1<<OCIE3A);
	Enter_Critical_Section
		TCNT3 = 0;
	Leave_Critical_Section
	j1939_timeout_flag = 0;
}
void j1939_set_filter(uint8_t mob_num,uint32_t pgn,uint32_t pgn_mask)
{
	Enter_Critical_Section
		can_filter_t param_filter;
		
		param_filter.id = pgn<<8;
		param_filter.mask = pgn_mask<<8;
		param_filter.flags.extended = 0x3;
		
		can_set_filter(mob_num,&param_filter);
	Leave_Critical_Section
}
void j1939_scan(Engine_Data* pengine)
{
	can_t msg;
	can_get_message(&msg);
	
	Enter_Critical_Section
		
		uint16_t rpm_check,bat_vge_check;
	
		switch( (msg.id)&(PGN_MASK<<8) ) 
		{
		
			case  (PGN_EEC1<<8):
				
				rpm_check = msg.data[4];
				rpm_check = rpm_check<<8;
				rpm_check |= msg.data[3];
				pengine->rpm = rpm_check;
				j1939_timeout_disable();
			break;
		
			case (PGN_ET1<<8):
				
				pengine->ect = msg.data[0];
				j1939_timeout_disable();
			break;
		
			case (PGN_VEP1<<8) :
				bat_vge_check = msg.data[5];
				bat_vge_check = bat_vge_check << 8;
				bat_vge_check |= msg.data[4];
				pengine->batvge = bat_vge_check;
				j1939_timeout_disable();
			break; 
		
			case (PGN_EFL_P1<<8) :
		
				pengine->lop = msg.data[3];
				j1939_timeout_disable();
			break;
		
		/*	case (PGN_IC1<<8) :  // only for dev, delete in production
				pengine->lop = msg.data[1];
				j1939_timeout_disable();
			break; */
		}
	Leave_Critical_Section
	
}
void process_data(Engine_Data* pengine)
{
		
	
	if(j1939_timeout_flag)
	{
		Enter_Critical_Section
			pengine->idx = MSG_CAN_TIMEOUT;
		Leave_Critical_Section
	}else
	{
			ect_sensor.trip = 0;
			lop_sensor.trip = 0;
		Enter_Critical_Section	
			if((pengine->ect - 40) > wt_trip.prm )
			{
				ect_sensor.tmr.en = 1;
			}else
			{
				ect_sensor.tmr.en = 0;
				ect_sensor.tmr.oneshot = 0;
			}
			if((pengine->lop) < (lop_trip.prm)/4 && lop_sensor.en)
			{
				lop_sensor.tmr.en = 1;
			}else
			{
				lop_sensor.tmr.en = 0;
				lop_sensor.tmr.oneshot = 0;
			}
	
		
			if(ect_sensor.tmr.done || ect_sensor.tmr.oneshot)
			{
				ect_sensor.trip = 1;
			}
		
			if(lop_sensor.tmr.done || lop_sensor.tmr.oneshot)
			{
				lop_sensor.trip = 1;
			}
		Leave_Critical_Section
		
			ect_sensor.tripC1 = (ect_sensor.trip && ect_sensor.en) || (ect_sensor.tripC1 && !(pengine->reset));
			ect_sensor.tripC2 = ect_sensor.tripC1  && ((pengine->mute) || (ect_sensor.tripC2 && (!ect_sensor.trip || !pengine->reset)));	
			ect_sensor.buzzer = ect_sensor.tripC1 && !(ect_sensor.tripC2) ;
		
		
			lop_sensor.tripC1 = (lop_sensor.trip && lop_sensor.en ) || (lop_sensor.tripC1 && !(pengine->reset));
			lop_sensor.tripC2 = lop_sensor.tripC1 && ((pengine->mute) || (lop_sensor.tripC2 && (!lop_sensor.trip || !pengine->reset)));
			lop_sensor.buzzer = lop_sensor.tripC1 && !(lop_sensor.tripC2) ;
		
			pengine->mute = 0;
			pengine->reset = 0;
		
			if(ect_sensor.buzzer || lop_sensor.buzzer)
			{
				RELAY_REG_PORT |= (1<<RELAY2);
			}else
			{
				RELAY_REG_PORT &= ~(1<<RELAY2);
			}
		
			if(ect_sensor.tripC1 || lop_sensor.tripC1 )
			{
				RELAY_REG_PORT |= (1<<RELAY1);
				KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
				pengine->status = TRIP;
			
				if(ect_sensor.tripC1)
				{ 
					pengine->idx = MSG_WT_TRIP;
				}
				if(lop_sensor.tripC1)
				{
					pengine->idx = MSG_LOP_TRIP;
				}
			
			
				lop_sensor.en = 0; // only applicable for lop as physical value depends on rpm
			}else
			{
				RELAY_REG_PORT &= ~(1<<RELAY1);
				KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);		
			
				Enter_Critical_Section
				
					if(pengine->rpm > flt_enable.prm)
					{
						if(pengine->idx == MSG_CAN_TIMEOUT && !j1939_timeout_flag)
						{
							pengine->idx = MSG_SAFETY_EN;
						}
						pengine->fault_enable = 1;
						pengine->fault_disable = 0;
					
					}
					if(pengine->fault.done)
					{
						ect_sensor.en = 1;
						lop_sensor.en = 1;
						pengine->idx = MSG_SAFETY_EN;
						pengine->status = RUNNING;
					}
				
					if(pengine->rpm < flt_disable.prm)
					{
						pengine->fault_enable = 0; 
						pengine->fault_disable = 1;
						pengine->fault.oneshot = 0;
						pengine->idx = MSG_SAFETY_DIS;
						pengine->status = READY;
					}	
				Leave_Critical_Section	
					if(pengine->fault_disable)
					{
						switch(pengine->status)
						{
							case READY :
								ect_sensor.en = 0;
								lop_sensor.en = 0;
							break;
						
							case RUNNING : //normal stop
								ect_sensor.en = 0;
								lop_sensor.en = 0;
							break;
						
							case TRIP :
								ect_sensor.en = ect_sensor.trip;
								lop_sensor.en = lop_sensor.trip;
							break;	
						
							case VALUE_SAVED:
							break;
						
							case DEFAULT_STATUS:
							break;
						
							default:
							break; 
						}
					
					}	
			}
	}
} 
void init() 
{

	DEBUG_REG_DDR |= ((1<<DEBUG_LED1)|(1<<DEBUG_LED2)|(1<<DEBUG_LED_POWER));
	RELAY_REG_DDR |= ((1<<RELAY1)|(1<<RELAY2));
	KEYPAD_B_REG_DDR |= (1<<DEBUG_LED_FAULT);
	
	KEYPAD_A_REG_DDR &= ~((1<<SW_ARROW_LEFT)|(1<<SW_ARROW_UP)|(1<<SW_BRIGHTNESS_MINUS)|(1<<SW_ENTER));
	KEYPAD_B_REG_DDR &= ~((1<<SW_BRIGHTNESS_PLUS)|(1<<SW_ARROW_DOWN)|(1<<SW_ARROW_RIGHT)|(1<<SW_BACK)|(1<<SW_MUTE));
	
	Ext_Int_init();
	//ADC_init();
	config_init();
	display_init(0xFF,UPDATE_RATE);
	
	DEBUG_REG_PORT |= (1<<DEBUG_LED_POWER);
	
	DDRD |= (1<<CAN_STBY);
	PORTD &= ~(1<<CAN_STBY);
	
	can_init(BITRATE_250_KBPS);
	
	j1939_set_filter(0,PGN_EEC2,PGN_MASK);
	j1939_set_filter(1,PGN_EEC1,PGN_MASK);
	j1939_set_filter(2,PGN_DM1,PGN_MASK);
	j1939_set_filter(3,PGN_SOFT,PGN_MASK);
	j1939_set_filter(4,PGN_HOURS,PGN_MASK);
	
	j1939_set_filter(5,PGN_CI,PGN_MASK);
	j1939_set_filter(6,PGN_ET1,PGN_MASK);
	j1939_set_filter(7,PGN_EFL_P1,PGN_MASK);
	j1939_set_filter(8,PGN_LFE1,PGN_MASK);
	j1939_set_filter(9,PGN_IC1,PGN_MASK);
	
	j1939_set_filter(10,PGN_VEP1,PGN_MASK);
	j1939_set_filter(11,PGN_WFI,PGN_MASK);
	j1939_set_filter(12,PGN_AMB,PGN_MASK);
	j1939_set_filter(13,PGN_LFC,PGN_MASK);
	j1939_set_filter(14,PGN_TCO1,PGN_MASK);
	
	j1939_timeout_init();
}

int main(){
	 
	init();	
	sei();
	
	for(;;)
	{
		
		if(in_config)
		{
				//config
				if(engine_data.rpm < 300)
				{
					config_init();
					config_main();
				}else
				{
					in_config = 0;
				}
				
		}
		else if(in_version)
		{
				display_update_disable();
				ks0108ClearScreen();
				ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);
				ks0108DrawRoundRect(5, 0, 117, 63, 8, BLACK);
				ks0108Puts_mod_align_center("Engine Safety Unit",6,117,6,110);
				ks0108DrawHoriLine(5,20,117,BLACK);
				ks0108DrawRect(15,25,96,13,BLACK);
				ks0108DrawVertLine(63,25,13,BLACK);
				ks0108Puts_mod_align_center("Version",16,62,28,45);
				ks0108Puts_mod_align_center(REVISION,64,105,28,45);
				ks0108Puts_mod_align_center("Power Control Engineers",6,117,45,110);
				while(in_version){}
			
		}else
		{
				j1939_timeout_enable(J1939_TIMEOUT_RATE);
				static uint8_t old_page_num;
				if(SW_RIGHT)
				{
					_delay_ms(250);
					page_num = (page_num >= PAGE_MAX ) ? PAGE_MAX: page_num + 1;
					if(page_num != old_page_num || in_config_old || in_version_old)
					{
						display_page_select(page_num);
						old_page_num = page_num;
					}
				
				}
				else if(SW_ENTER_P )
				{
					_delay_ms(500);
					if(SW_RIGHT)
					{
					
						in_config = 1;
						in_config_old = 0;
					}
					else if(SW_LEFT)
					{
						in_version = 1;
						in_version_old = 0;
					}
				}
				else if(SW_LEFT)
				{
					_delay_ms(250);
					page_num = (page_num <= PAGE_MIN) ? 1 : page_num - 1;
					if(page_num != old_page_num || in_config_old || in_version_old)
					{
						display_page_select(page_num);
						old_page_num = page_num;
					}
				}
				else
				{
				
					j1939_scan(&engine_data);
					process_data(&engine_data);
				}
		}
		
	}
	
}


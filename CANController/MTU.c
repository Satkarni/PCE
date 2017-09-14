/*
 * Copyright:      Power Control Engineers  
 * Author:         Rohan Pandit
 * Remarks:        
 * Version:        01
 * Description:    Main routine for MTU controller
 * 
 */

/*
DTC Conversion Method (Version) 1:
Byte 1: 8 most significant bits of 16 most significant bits of SPN
Byte 2: 8 least significant bits of 16 most significant bits of SPN
Byte 3: 3 most significant bits of byte contain the 3 least significant bits of SPN
5 least significant bits of byte contain the FMI
Byte 4: most significant bit of byte contains CM
7 least significant bits of byte contains OC

DTC Conversion Method (Version) 2:
Byte 1: 8 least significant bits of 16 most significant bits of SPN
Byte 2: 8 most significant bits of 16 most significant bits of SPN
Byte 3: 3 most significant bits of byte contain the 3 least significant bits of SPN
5 least significant bits of byte contain the FMI
Byte 4: most significant bit of byte contains CM
7 least significant bits of byte contains OC

DTC Conversion Method (Version) 3:
Byte 1: 8 least significant bits of SPN
Byte 2: 8 second byte of SPN
Byte 3: 3 most significant bits of byte contain the 3 most significant bits of SPN
5 least significant bits of byte contain the FMI
Byte 4: most significant bit of byte contains CM
7 least significant bits of byte contains OC
*/

#include "MTU.h"

uint8_t dtc_received = 0, empty_run = 0;
uint8_t update_status=1, current_status;
long empty_scan_count = 0;

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
	if(PAGE_CONFIG_MIN <= page_num)
	{
		in_config = 0;
		in_config_old = 1;
		page_num = PAGE_MIN;
		display_page_select(page_num);
		display_update_enable(UPDATE_RATE);
		update_status = 1;
	}
	else
	{
		engine_data.reset = 1;
	}
	if (dtc_received == 1)
		dtc_received = 0;
}

ISR(TIMER1_COMPA_vect)		// Main update interrupt 
{
	
	display_page_update(&engine_data,page_num);
	
	if(ect_sensor.tmr.en)
	{
		ect_sensor.tmr.acc++;
		if(ect_sensor.tmr.acc > WT_TRIP_DELAY_DF*2 )//(wt_trip_delay.prm)*2)
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
		if(lop_sensor.tmr.acc > LOP_TRIP_DELAY_DF*2 )//(lop_trip_delay.prm)*2)
		{
			lop_sensor.tmr.done = 1;
			lop_sensor.tmr.oneshot = 1;
			lop_sensor.tmr.acc = 0;
		}
	}else
	{
		lop_sensor.tmr.done = 0;
		lop_sensor.tmr.acc = 0;
	}
	
	if(rpm_sensor.tmr.en)
	{
		rpm_sensor.tmr.acc++;
		if(rpm_sensor.tmr.acc > RPM_TRIP_DELAY_DF*2 )//(lop_trip_delay.prm)*2)
		{
			rpm_sensor.tmr.done = 1;
			rpm_sensor.tmr.oneshot = 1;
			rpm_sensor.tmr.acc = 0;
		}
	}else
	{
		rpm_sensor.tmr.done = 0;
		rpm_sensor.tmr.acc = 0;
	}
	
	if(coolant_level_sensor.tmr.en)
	{
		coolant_level_sensor.tmr.acc++;
		if(coolant_level_sensor.tmr.acc > COOLANT_LEVEL_TRIP_DELAY_DF*2) //coolant_level_trip_delay.prm*2 )
		{
			coolant_level_sensor.tmr.done = 1;
			coolant_level_sensor.tmr.oneshot = 1;
			coolant_level_sensor.tmr.acc = 0;
		}
	}else
	{
		coolant_level_sensor.tmr.done = 0;
		coolant_level_sensor.tmr.acc = 0;
	}
	switch(engine_data.status)
	{		
		case READY :	
			if(engine_data.fault_enable)		// fault enable timer
			{
				engine_data.fault.acc++;
				if(engine_data.fault.acc > FAULT_ENABLE_DELAY_DF*2 )//(flt_enable_delay.prm)*2)
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
		
		/*case TRIP :
				if(!(ect_sensor.trip || lop_sensor.trip))
				{
					engine_data.status = READY;
				}
		break;*/
	}
	
	
}

uint16_t getCoolantlevel(uint8_t channel)
{
	uint16_t coolantLevel = 0;
	coolantLevel = ADC_read(channel);
	return coolantLevel;
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

uint8_t checkStatus()
{
	
			if (dtc_received == 0)
			{
				KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
				return 0;
			}
			else if(dtc_received == 1)
			{
				KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
				return 1;
			}
			else
			{
				return 0;
			}
}

void setStatus(uint8_t currentStatus)
{
	Enter_Critical_Section
	switch(currentStatus)
	{
		case 0:
			ks0108FillRect(0,53,127,10,WHITE);
			ks0108Puts_mod("Status: All Systems Online",0,53,100);
		break;
		
		case 1:
			ks0108FillRect(0,53,127,10,WHITE);
			ks0108Puts_mod("Status: DTC Received",0,53,100);
		break;
		
		case 2:
			ks0108FillRect(0,53,127,10,WHITE);
			ks0108Puts_mod("Status: CAN Bus Failure",0,53,100);
			
		break;
	}
	Leave_Critical_Section
}

void j1939_clear_DTC(uint8_t DMtype)
{
	can_t msg_tx;
	
	Enter_Critical_Section
		uint32_t pgn = 0;
		switch (DMtype)
		{
			case 11:
				pgn = REQ_DM11;
			break;
			
			case 3:
				pgn = REQ_DM3;
			break;
			
		}
		msg_tx.id = 0x18EA00FF; //59904
		
		msg_tx.flags.rtr = 0;
		msg_tx.flags.extended = 1;
		
		msg_tx.length = 3;
		
		msg_tx.data[0] = pgn>>8;
		msg_tx.data[1] = pgn>>16;
		msg_tx.data[2] = 0x00;
		
		can_send_message(&msg_tx);
		
	Leave_Critical_Section
}

void j1939_send_ack(uint32_t PGN, uint8_t control_byte)
{
	can_t ack_tx;
	
	Enter_Critical_Section
	uint32_t temp;
	temp = PGN;
	
	ack_tx.id = 0x18E80000;
	
	ack_tx.flags.rtr = 0;
	ack_tx.flags.extended = 1;
	
	ack_tx.length = 8;
	
	ack_tx.data[0] = control_byte;
	ack_tx.data[1] = 0x00;
	ack_tx.data[2] = 0xFF;
	ack_tx.data[3] = 0xFF;
	ack_tx.data[4] = 0xFF;
	ack_tx.data[5] = temp & 0xFF;
	ack_tx.data[6] = (temp>>8) & 0xFF;
	ack_tx.data[7] = (temp>>16) & 0xFF;
	
	can_send_message(&ack_tx);
	
	Leave_Critical_Section
}

void j1939_request(uint32_t pgn)
{
	can_t msg_tx;
	
	Enter_Critical_Section
	
	//uint32_t pgn_temp = pgn;
	
	msg_tx.id = 0x18EA00FF; //59904
	
	msg_tx.flags.rtr = 0;
	msg_tx.flags.extended = 1;
	
	msg_tx.length = 3;
	
	msg_tx.data[0] = pgn>>8;
	msg_tx.data[1] = pgn>>16;
	msg_tx.data[2] = 0x00;
	
	can_send_message(&msg_tx);
	
	Leave_Critical_Section
}

void j1939_scan(Engine_Data *pengine)
{
	
	
	can_t msg;
	can_get_message(&msg);
	
	Enter_Critical_Section
		int ect_old;
		int buffer_count;
		static uint8_t old_sequence;
		//uint16_t rpm_check,ect_check,rpm_old;
		uint16_t rx_pdus,rx_pduf,rx_src;
		uint16_t pgn;
		uint16_t i,j;
		
		char spn_char[10];
		char fmi_char[8];
		uint32_t tmp;
		
		uint8_t *ptr = (uint8_t *) &tmp;
		
		
		rx_pduf = (msg.id>>16) & 0xFF;
		rx_pdus = (msg.id>>8) & 0xFF;
		rx_src = msg.id & 0xFF;
		if(240 > rx_pduf)
			pgn = rx_pduf<<8 & 0xFF00;
		else
			pgn = rx_pduf<<8 | rx_pdus;
		
		
		switch( pgn ) 
		{
			
			//######### Set RPM and Engine Torque###############
		
			case  (PGN_EEC1):
				
				pengine->rpm = ((msg.data[4]<<8)|msg.data[3]);
				pengine->act_eng_torque = msg.data[2];
				pengine->dem_eng_torque = msg.data[7];
				empty_scan_count = 0;
			break;
			
			
			//##### Set Acc Pedal Position & Engine Load % ######
				
			case  (PGN_EEC2):
				pengine->acc_pedal_pos1 = msg.data[1];
				pengine->eng_percent_load = msg.data[2];
				pengine->max_torque_percent = msg.data[6];
				empty_scan_count = 0;
			
			break;
			
			
			//## Engine Coolant Temp & Engine Oil Temp ##########
			
			case (PGN_ET1):
				pengine->ect = msg.data[0];
				pengine->lot = ((msg.data[3]<<8)|msg.data[2]);
				pengine->fuel_temp = msg.data[1];
				empty_scan_count = 0;
			break;
			
			//##### Engine Oil Pressure & Oil Level #############
			
			case (PGN_EFL_P1) :
			
				pengine->lop = msg.data[3];
				pengine->oil_level = msg.data[2];
				pengine->coolant_level = msg.data[7];
				empty_scan_count = 0;
			break;
			
			//###### Boost Pressure & Boost Temperature #########
			
			case (PGN_IC1) :
				
				pengine->boost_pressure = msg.data[1];
				pengine->boost_temperature = msg.data[2];
				empty_scan_count = 0;
				
			break;
			
			//##### Engine Shutoff ##############################
		
			case (PGN_AUXIO1) :
			
				pengine->eng_protection_shutoff = msg.data[0] & 0x03;
				pengine->eng_shutoff_override = (msg.data[0]>>6) & 0x03;
				empty_scan_count = 0;
				
			break;
			
			//##### Hours of Operation, Total Revolutions ####### 
		
			case (PGN_HOURS) :
			
				*ptr       = msg.data[0];
				*(ptr + 1) = msg.data[1];
				*(ptr + 2) = msg.data[2];
				*(ptr + 3) = msg.data[3];
				pengine->hours = tmp ;
				empty_scan_count = 0;
				j1939_send_ack(0x18FEE500,0);
				/**ptr       = msg.data[4];
				*(ptr + 1) = msg.data[5];
				*(ptr + 2) = msg.data[6];
				*(ptr + 3) = msg.data[7];
				
				pengine->rev = 0;*/
			break;
			
			//###### Software Version ###########################
			
			case (PGN_SOFT) :
				
				pengine->soft_version = msg.data[2]<<8|msg.data[1];
				pengine->soft_sub_version = msg.data[4]<<8|msg.data[3];
				empty_scan_count = 0;
			break;
			
			
			//####### DM1 #######################
			
			case (PGN_DM1) :
				
				tmp = msg.data[4]>>5;
				DM1[0].spn = ( (msg.data[3]<<11)|(msg.data[2]<<5)|(tmp) );
				DM1[0].fmi = (msg.data[4] & 0x1F);
				DM1[0].occ = (msg.data[5] & 0x7F);
				if(0 != DM1[0].spn)
				{
					dtc_received = 1;
					DG.DM_number = 1;
				}
				empty_scan_count = 0;
					
			break;
			//###### DM2 #######################################
			
			case (PGN_DM2) :
				
				tmp = msg.data[4]>>5;
				DM2[0].spn = ( (msg.data[3]<<11)|(msg.data[2]<<5)|(tmp) );
				DM2[0].fmi = (msg.data[4] & 0x1F);
				DM2[0].occ = (msg.data[5] & 0x7F);
				if(0 != DM2[0].spn)
				{
					DG2.DM_number = 1;
				}
				empty_scan_count = 0;
				
			break;
			//###### BAM Packet #################################
			
			case (PGN_BAM) :
			
				if (0 == rx_src)
				{
					BAM.control = msg.data[0];
					if (32 == BAM.control)
					{
						BAM.PGN = msg.data[6]<<8 | msg.data[5];
						
						if (BAM.PGN == PGN_DM1)
						{
							BAM.bam_received = 1;
							BAM.message_size = msg.data[2]<<8 | msg.data[1];
							BAM.npackets = msg.data[3];
							DT.process_buffer = 0;
							dtc_received = 1;
						}
						else if (BAM.PGN == PGN_DM2)
						{
							BAM.bam_received = 1;
							BAM.message_size = msg.data[2]<<8 | msg.data[1];
							BAM.npackets = msg.data[3];
							DT.process_buffer = 0;
						}
					}
				}
				empty_scan_count = 0;
			break;
			
			//###### TP Data Packet #############################
			
			case (PGN_DT) :
				if (1 == BAM.bam_received)
				{
					if(BAM.npackets <= DT.count)
						DT.process_buffer = 1;
					else
					{
						if(old_sequence != msg.data[0])
							DT.count++;
						old_sequence = msg.data[0];
						j=1;
						for (i = ((msg.data[0]-1)*7); i <= ((msg.data[0]-1)*7)+6 ; i++)
						{
							DT.buffer[i] = msg.data[j++];
						}
					}
						
				}
				empty_scan_count = 0;
			
			break;
			
			//###### Acknowledge Message ########################
			
			case (PGN_ACK) :
				//empty_scan_count = 0;
			break;
			
			//###################################################
		}
	Leave_Critical_Section
	
}

void can_failure()
{
	KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
	display_can_failure();
	for(;;)
	{
		j1939_scan(&engine_data);
		if(empty_scan_count >= 16000)
			empty_scan_count = 18000;
		else
			break;
	}
}

void process_data(Engine_Data *pengine)
{
	double tmp_n;
	uint16_t coolantLevel = 0;
	Enter_Critical_Section
		
		ect_sensor.trip = 0;
		lop_sensor.trip = 0;
		rpm_sensor.trip = 0;
		coolant_level_sensor.trip = 0;
		
		if((pengine->ect - 40) > WT_TRIP_DF && pengine->ect <= 250 )//wt_trip.prm )
		{
			ect_sensor.tmr.en = 1;
		}else
		{
			ect_sensor.tmr.en = 0;
			ect_sensor.tmr.oneshot = 0;
		}
		if((pengine->lop) < /*(lop_trip.prm)*/LOP_TRIP_DF/4 && lop_sensor.en && pengine->lop <= 250 )
		{
			lop_sensor.tmr.en = 1;
		}else
		{
			lop_sensor.tmr.en = 0;
			lop_sensor.tmr.oneshot = 0;
		}
		
		if((pengine->rpm)/8 >= RPM_RUN_ON)
			RUN_RELAY_ON;
		else
			RUN_RELAY_OFF;
		
		if((pengine->rpm)/8 >= RPM_TRIP_DF && pengine->rpm <= 64255 )//wt_trip.prm )
		{
			rpm_sensor.tmr.en = 1;
		}else
		{
			rpm_sensor.tmr.en = 0;
			rpm_sensor.tmr.oneshot = 0;
		}
		
		coolantLevel = getCoolantlevel(ADC_COOLANT_LEVEL_CHANNEL);
		
		if(coolantLevel > ADC_COOLANT_LEVEL_LOW)
		{
			coolant_level_sensor.tmr.en = 1;
		}	
		else       
		{                             
			pengine->coolant_level = 1;
			coolant_level_sensor.tmr.en = 0;
			coolant_level_sensor.tmr.oneshot = 0;
		}
		
		
		if(ect_sensor.tmr.done || ect_sensor.tmr.oneshot)
		{
			ect_sensor.trip = 1;
		}
		
		if(lop_sensor.tmr.done || lop_sensor.tmr.oneshot)
		{
			lop_sensor.trip = 1;
		}
		
		if(rpm_sensor.tmr.done || rpm_sensor.tmr.oneshot)
		{
			rpm_sensor.trip = 1;
		}
		
		if(coolant_level_sensor.tmr.done || coolant_level_sensor.tmr.oneshot)
		{
			coolant_level_sensor.trip = 1;
			pengine->coolant_level = 2;
		}
	Leave_Critical_Section
		
		ect_sensor.tripC1 = (ect_sensor.trip && ect_sensor.en) || (ect_sensor.tripC1 && !(pengine->reset));
		ect_sensor.tripC2 = ect_sensor.tripC1  && ((pengine->mute) || (ect_sensor.tripC2 ));
		ect_sensor.buzzer = ect_sensor.tripC1 && !(ect_sensor.tripC2) ;
		
		lop_sensor.tripC1 = (lop_sensor.trip && lop_sensor.en ) || (lop_sensor.tripC1 && !(pengine->reset));
		lop_sensor.tripC2 = lop_sensor.tripC1 && ((pengine->mute) || (lop_sensor.tripC2 ));
		lop_sensor.buzzer = lop_sensor.tripC1 && !(lop_sensor.tripC2) ;
		
		rpm_sensor.tripC1 = (rpm_sensor.trip && rpm_sensor.en) || (rpm_sensor.tripC1 && !(pengine->reset));
		rpm_sensor.tripC2 = rpm_sensor.tripC1  && ((pengine->mute) || (rpm_sensor.tripC2));
		rpm_sensor.buzzer = rpm_sensor.tripC1 && !(rpm_sensor.tripC2) ;
		
		coolant_level_sensor.tripC1 = (coolant_level_sensor.trip /*&& coolant_level_sensor.en*/) || (coolant_level_sensor.tripC1 && !(pengine->reset));
		coolant_level_sensor.tripC2 = coolant_level_sensor.tripC1  && ((pengine->mute) || (coolant_level_sensor.tripC2));
		coolant_level_sensor.buzzer = coolant_level_sensor.tripC1 && !(coolant_level_sensor.tripC2) ;
		
		pengine->mute = 0;
		pengine->reset = 0;
		
			if(ect_sensor.buzzer || lop_sensor.buzzer || rpm_sensor.buzzer || coolant_level_sensor.buzzer)
			{
				RELAY_REG_PORT |= (1<<RELAY2);
				//KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
			}else
			{
				RELAY_REG_PORT &= ~(1<<RELAY2);
				//KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
			}
			
			if(ect_sensor.tripC1 || lop_sensor.tripC1 || rpm_sensor.tripC1 || coolant_level_sensor.tripC1 )
			{
				
				if(ect_sensor.tripC1)
				{
					pengine->alarm_msg = ECT_HIGH;
				}
				if(lop_sensor.tripC1)
				{
					pengine->alarm_msg = LOP_LOW;
				} 
				if(rpm_sensor.tripC1)
				{
					pengine->alarm_msg = OVERSPEED_ALARM;
				}
				if(coolant_level_sensor.tripC1)
				{
					pengine->alarm_msg = COOLANT_LEVEL_LOW;
					RELAY_REG_PORT |= (1<<RELAY1);
					//KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
				}else
				{
					RELAY_REG_PORT &= ~(1<<RELAY1);
				}
				//lop_sensor.en = 0; // only applicable for lop as physical value depends on rpm
			}else
			{ 
				RELAY_REG_PORT &= ~(1<<RELAY1);
				//KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
				
				Enter_Critical_Section
				if((pengine->rpm)/8 > FAULT_ENABLE_DF)//flt_enable.prm)
				{
					pengine->fault_enable = 1;
					pengine->fault_disable = 0;
					
				}
				if(pengine->fault.done)
				{
					ect_sensor.en = 1;
					lop_sensor.en = 1;
					rpm_sensor.en = 1;
					coolant_level_sensor.en = 1;
					pengine->alarm_msg = SAFETY_ENABLED;
					pengine->status = RUNNING;
				}
				
				if((pengine->rpm)/8 <  FAULT_DISABLE_DF )//flt_disable.prm)
				{
					pengine->fault_enable = 0;
					pengine->fault_disable = 1;
					pengine->fault.oneshot = 0;
					pengine->alarm_msg = SAFETY_DISABLED;
					pengine->status = READY; 
				}
				Leave_Critical_Section
				if(pengine->fault_disable)
				{
					ect_sensor.en = 0;
					lop_sensor.en = 0;
					rpm_sensor.en = 0;
					coolant_level_sensor.en = 0;
				/*	switch(pengine->status)
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
						
						case CAN_TIMEOUT:
						break;
						
						case CAN_ERROR :
						break;
						
						case VALUE_SAVED:
						break;
						
						case DEFAULT_STATUS:
						break;
						
						default:
						break; 
					}
					 */
				}
				if(pengine->status == RUNNING && coolant_level_sensor.tripC1 != 1)
				{
					
					pengine->alarm_msg = SAFETY_ENABLED;
				}
			}
			
		Enter_Critical_Section
		if(1 == DT.process_buffer)
		{
			uint16_t n;
			uint32_t tmp, tmp1, tmp2, spn;
			uint8_t i,j;
			
			tmp_n = ceil(((double)BAM.message_size/8));
			
			if (BAM.PGN == PGN_DM1)
			{
				DG.DM_number = (uint8_t)tmp_n;
				for(i = 0,j=0; i < DG.DM_number; i++)
				{
					tmp = (DT.buffer[(i*8)+4]>>5) & 0x07;
					tmp1 = DT.buffer[(i*8)+3] & 0xFF;
					tmp2 = DT.buffer[(i*8)+2] & 0xFF;
					spn = ( (tmp1<<11)|(tmp2<<3)|(tmp) );
					if (spn < 4810 || (spn < 520263 && spn > 520192) || (spn < 1400 && spn > 1600) )
					{
						//DM1[i].spn = ( (tmp<<16)|(tmp1<<8)|(tmp2) );  //SPN Conversion Method 3
						DM1[j].spn = spn;	//SPN Conversion Method 2
						DM1[j].fmi = (DT.buffer[(i*8)+4] & 0x1F);
						DM1[j].occ = (DT.buffer[(i*8)+5] & 0x7F);
						j++;
					}
					
				}
			}
			else if (BAM.PGN == PGN_DM2)
			{
				DG2.DM_number = (uint8_t)tmp_n;
				for(i = 0,j=0; i < DG2.DM_number; i++)
				{
					tmp = (DT.buffer[(i*8)+4]>>5) & 0x07;
					tmp1 = DT.buffer[(i*8)+3] & 0xFF;
					tmp2 = DT.buffer[(i*8)+2] & 0xFF;
					spn = ( (tmp1<<11)|(tmp2<<3)|(tmp) );
					if (spn < 4810 || (spn < 520263 && spn > 520192) || (spn < 1400 && spn > 1600) )
					{
						//DM1[i].spn = ( (tmp<<16)|(tmp1<<8)|(tmp2) );  //SPN Conversion Method 3
						DM2[j].spn = spn;								//SPN Conversion Method 2
						DM2[j].fmi = (DT.buffer[(i*8)+4] & 0x1F);
						DM2[j].occ = (DT.buffer[(i*8)+5] & 0x7F);
						j++;
					}
					
				}
			}
			BAM.bam_received = 0;
			DT.process_buffer = 0;
			DT.count = 0;
		}
		
		Leave_Critical_Section
} 

void diagnostics_configuration()
{	
	for (;;)
	{
			if(SW_DOWN)
			{
				_delay_ms(200);
				if(PAGE_CONFIG_MIN == page_num)
				{
					Enter_Critical_Section
					DG.page_increment = 1;
					Leave_Critical_Section
				}
			}
			else if(SW_UP)
			{
				_delay_ms(200);
				if(PAGE_CONFIG_MIN == page_num)
				{
					Enter_Critical_Section
					DG.page_decrement = 1;
					Leave_Critical_Section
				}
			}
			else if (SW_RIGHT)
			{
				_delay_ms(200);
				page_num = (PAGE_CONFIG_MAX <= page_num) ? PAGE_CONFIG_MIN: page_num+1;
				DG.refresh = 1;
				DG2.refresh = 1;
				display_page_select(page_num);
			}
			else if (SW_LEFT)
			{
				_delay_ms(200);
				page_num = (PAGE_CONFIG_MIN >= page_num) ? PAGE_CONFIG_MAX: page_num-1;
				DG.refresh = 1;
				DG2.refresh = 1;
				display_page_select(page_num);
			}
			if(18000 > empty_scan_count)
			empty_scan_count += 1;
			process_data(&engine_data);
			j1939_scan(&engine_data);
			checkStatus();
			if(PAGE_CONFIG_MIN > page_num)
				return;
		}
}

void init() 
{

	DEBUG_REG_DDR |= ((1<<DEBUG_LED1)|(1<<DEBUG_LED2)|(1<<DEBUG_LED_POWER));
	RELAY_REG_DDR |= ((1<<RELAY1)|(1<<RELAY2));
	KEYPAD_B_REG_DDR |= (1<<DEBUG_LED_FAULT);
	
	KEYPAD_A_REG_DDR &= ~((1<<SW_ARROW_LEFT)|(1<<SW_ARROW_UP)|(1<<SW_BRIGHTNESS_MINUS)|(1<<SW_ENTER));
	KEYPAD_B_REG_DDR &= ~((1<<SW_BRIGHTNESS_PLUS)|(1<<SW_ARROW_DOWN)|(1<<SW_ARROW_RIGHT)|(1<<SW_BACK)|(1<<SW_MUTE));
	
	// 2017-03-16: On MTU request, use PF5 (pin 56/ADC5/TMS) as general purpose output to drive a relay
	DDRF |= (1<<RUN_RELAY);
	RUN_RELAY_OFF;
	
	Ext_Int_init();
	ADC_init();
	config_init();
	display_init(0xFF);
	
	display_loading_text();
	display_loading_bar();
	display_function_text(0);
	_delay_ms(200);
	display_update_load(20,0,24);
	
	DEBUG_REG_PORT |= (1<<DEBUG_LED_POWER);
	
	DDRD |= (1<<CAN_STBY);
	PORTD &= ~(1<<CAN_STBY);
	
	can_init(BITRATE_250_KBPS);
	
	display_function_text(1);
	_delay_ms(200);
	display_update_load(20,24,74);
	
	j1939_set_filter(0,PGN_EEC2,PGN_MASK);
	j1939_set_filter(1,PGN_EEC1,PGN_MASK);
	j1939_set_filter(2,PGN_DM1,PGN_MASK);
	j1939_set_filter(3,PGN_SOFT,PGN_MASK);
	j1939_set_filter(4,PGN_HOURS,PGN_MASK);
	
	j1939_set_filter(5,PGN_CI,PGN_MASK);
	j1939_set_filter(6,PGN_ET1,PGN_MASK);
	j1939_set_filter(7,PGN_EFL_P1,PGN_MASK);
	//j1939_set_filter(8,PGN_AUXIO1,PGN_MASK);  WARNING: DO NOT USE FILTER 8
	j1939_set_filter(8,PGN_DM2,PGN_MASK);
	j1939_set_filter(9,PGN_IC1,PGN_MASK); 
	
	//j1939_set_filter(10,PGN_PTO,PGN_MASK);
	//j1939_set_filter(12,PGN_AMB,PGN_MASK);
	//j1939_set_filter(13,PGN_LFC,PGN_MASK);
	j1939_set_filter(10,PGN_AUXIO1,PGN_MASK);
	j1939_set_filter(11,PGN_ACK,PGN_MASK);
	j1939_set_filter(12,PGN_BAM,0xFF00);
	j1939_set_filter(13,PGN_DT,0xFF00);
	//j1939_set_filter(14,PGN_TCO1,PGN_MASK);
	
	display_function_text(2);
	_delay_ms(200);
	display_update_load(20,74,87);
	_delay_ms(200);
	
	display_update_enable(UPDATE_RATE);
	page_num = PAGE_MIN;
	display_page_select(page_num);
	
}

int main()
{
	 
	init();	
	sei();
	uint8_t x;
	BAM.bam_received = 0;
	DT.count = 0;
	reset_page_update = 0;
	
	for(;;)
	{
		if((0 == empty_run) && (16000 <= empty_scan_count))
		{
			empty_run = 1;
		}
		
		if (empty_run == 1)
		{
			can_failure();
			KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
			reset_page_update = 1;
			process_data(&engine_data);
			empty_run = 0;
			page_num = PAGE_MIN;
			display_page_select(page_num);
			update_status = 1;
			process_data(&engine_data);
		}
		
		static uint8_t old_page_num;
		if(current_status != checkStatus())
		{
			update_status = 1;
			current_status = checkStatus();
		}
			
			
		if(PAGE_MAX >= page_num)
		{
			x = checkStatus();
			if(1 == update_status)
			{
				setStatus(x);
				update_status = 0;
			}
		}
	
		
		if(PAGE_CONFIG_MIN <= page_num)
		{
			diagnostics_configuration();
		}
		else if(SW_RIGHT)
		{
			_delay_ms(250);
			page_num = (page_num >= PAGE_MAX ) ? PAGE_MIN: page_num + 1;
			if(page_num == 7)
			{
				page_num++;
			} 
				
			if(page_num != old_page_num)
			{
				update_status = 1;
				display_page_select(page_num);
				old_page_num = page_num;
			}
					
		}
		else if(SW_LEFT)
		{
			_delay_ms(250);
			page_num = (page_num <= PAGE_MIN) ? PAGE_MAX : page_num - 1;
			if(page_num == 7)
			{
				page_num--;
			}	
			if(page_num != old_page_num)
			{
				update_status = 1;
				display_page_select(page_num);
				old_page_num = page_num;
			}
		}
		else if(SW_ENTER_P)
		{
			_delay_ms(250);
			DG.refresh = 1;
			old_page_num = page_num;
			page_num = PAGE_CONFIG_MIN;
			display_update_disable();
			display_function_text(3);
			display_fancy_effect();
			_delay_ms(500);
			display_update_enable(UPDATE_RATE);
			display_page_select(page_num);
		}
		else
		{
			if(16000 > empty_scan_count)
				empty_scan_count += 1;
			j1939_scan(&engine_data);
			process_data(&engine_data);
					
		}
	
		 
		
	}
	
}


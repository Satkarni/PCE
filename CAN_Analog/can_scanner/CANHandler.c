/*
 * CANHandler.c
 *
 *  Created on: 17-Aug-2016
 *      Author: piyush
 */

#include "CANHandler.h"

void CANHandler_init()
{
	CANHandler_initSensors();

	can_init(BITRATE_250_KBPS);
	CANHandler_setFilter(0,PGN_EEC1,PGN_MASK);
	CANHandler_setFilter(1,PGN_ET1,PGN_MASK);
	CANHandler_setFilter(2,PGN_EFL_P1,PGN_MASK);
	CANHandler_setFilter(3,PGN_DM1,PGN_MASK);
	CANHandler_setFilter(4,PGN_BAM,PGN_MASK);
	CANHandler_setFilter(5,PGN_VEP1,PGN_MASK);
	CANHandler_setFilter(6,PGN_HOURS,PGN_MASK);
	CANHandler_setFilter(7,PGN_ACK,PGN_MASK);
	CANHandler_setFilter(8,PGN_REQ,PGN_MASK);
}

void CANHandler_initSensors()
{
	addSensor("RPM");
	revokeAlarm("RPM");
	allowTrip("RPM");
	setSensorMinMax("RPM",0,64255);
	enableTripAtHigh("RPM",1);
	setAlarmThresholdHigh("RPM",RPM_ALARM_LEVEL*8);
	setTripThresholdHigh("RPM",RPM_TRIP_LEVEL*8);
	setAlarmTimer("RPM",ALARM_DELAY);
	setTripTimer("RPM",TRIP_DELAY);
	setFailureTimer("RPM",10);
	enableSensorAlarm("RPM",0);
	setSensorValue("RPM",0);


	addSensor("LOP");
	setSensorMinMax("LOP",0,250);
	setFailureTimer("LOP",20);
	enableTripAtHigh("LOP",0);

	setAlarmThresholdLow("LOP",LOP_ALARM_LEVEL * 25);
	setTripThresholdLow("LOP",LOP_TRIP_LEVEL * 25);

	setAlarmTimer("LOP",ALARM_DELAY);
	setTripTimer("LOP",TRIP_DELAY);
	setSensorValue("LOP",0);
	setFailureTimer("LOP",10);

	addSensor("LOT");
	allowAlarm("LOT");
	allowTrip("LOT");
	setFailureTimer("LOT",10);
	enableTripAtHigh("LOT",1);
	setAlarmThresholdHigh("LOT",(LOT_ALARM_LEVEL + 273) * 32);
	setTripThresholdHigh("LOT",(LOT_TRIP_LEVEL + 273) * 32);
	setAlarmTimer("LOT",ALARM_DELAY);
	setTripTimer("LOT",TRIP_DELAY);
	setSensorMinMax("LOT",0,64255);
	setSensorValue("LOT",0);
	enableSensorAlarm("LOT",0);

	addSensor("ECT");
	allowAlarm("ECT");
	allowTrip("ECT");
	setSensorMinMax("ECT",0,250);
	setFailureTimer("ECT",10);
	enableTripAtHigh("ECT",1);
	setAlarmThresholdHigh("ECT",ECT_ALARM_LEVEL+40);
	setTripThresholdHigh("ECT",ECT_TRIP_LEVEL+40);
	setAlarmTimer("ECT",ALARM_DELAY);
	setTripTimer("ECT",TRIP_DELAY);

	addSensor("BattVGE");
	revokeAlarm("BattVGE");
	revokeTrip("BattVGE");
	setSensorMinMax("BattVGE",0,64255);
	setAlarmThresholdHigh("BattVGE",64255);
	setTripThresholdHigh("BattVGE",64255);
	setFailureTimer("BattVGE",10);

	addSensor("Hours");
	revokeAlarm("Hours");
	revokeTrip("Hours");
	setSensorMinMax("Hours",0,4211081215);
	setAlarmThresholdHigh("Hours",4211081215);
	setTripThresholdHigh("Hours",4211081215);
	setFailureTimer("Hours",10);
	setSensorValue("Hours",0);
}

void CANHandler_updateReading(char* sensor_name, uint32_t value)
{
	Enter_Critical_Section
	uint32_t currentValue;
	currentValue = getSensorValue(sensor_name);
	if(value != currentValue)
	{
		if(strcmp(sensor_name,"RPM") == 0)
		{
			if(abs(value - currentValue) < (750*8))
			{
				setSensorValue(sensor_name,value);
				page_updateValue(sensor_name);
			}
		}
		else if(strcmp(sensor_name,"LOT") == 0)
		{
			if(abs(value - currentValue) < (5*32))
			{
				setSensorValue(sensor_name,value);
				page_updateValue(sensor_name);
			}
		}
		else
		{
			setSensorValue(sensor_name,value);
			page_updateValue(sensor_name);
		}
	}
	Leave_Critical_Section
}

void CANHandler_setFilter(uint8_t mob_num,uint32_t pgn,uint32_t pgn_mask)
{
	Enter_Critical_Section
	can_filter_t param_filter;

	param_filter.id = pgn<<8;
	param_filter.mask = pgn_mask<<8;
	param_filter.flags.extended = 0x3;

	can_set_filter(mob_num,&param_filter);
	Leave_Critical_Section
}

void CANHandler_sendACK(uint32_t PGN, uint8_t control_byte)
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

void CANHandler_scan()
{
	can_t msg;
	can_get_message(&msg);

	Enter_Critical_Section

	uint16_t rx_pdus,rx_pduf;//,rx_src;
	uint16_t pgn = 0;
	uint32_t spn_tmp = 0;

	union uint8_to_uint32
	{
		uint32_t value32;
		uint8_t value8[4];
	}converter32;

	union uint8_to_uint16
	{
		uint16_t value16;
		uint8_t	value8[2];
	}converter16;

	converter32.value32 = 0;
	converter16.value16 = 0;

	rx_pduf = (msg.id>>16) & 0xFF;
	rx_pdus = (msg.id>>8) & 0xFF;
	//rx_src = msg.id & 0xFF;
	if(240 > rx_pduf)
		pgn = rx_pduf<<8 & 0xFF00;
	else
		pgn = rx_pduf<<8 | rx_pdus;


	switch (pgn)
	{
	case PGN_EEC1 :
		converter16.value8[1] = msg.data[4];
		converter16.value8[0] = msg.data[3];
		CANHandler_updateReading("RPM",converter16.value16);
		break;

	case PGN_ET1 :
		CANHandler_updateReading("ECT",msg.data[0]);

		converter16.value8[1] = msg.data[3];
		converter16.value8[0] = msg.data[2];
		CANHandler_updateReading("LOT",converter16.value16);
		break;

	case PGN_EFL_P1 :
		CANHandler_updateReading("LOP",(msg.data[3]));
		break;

	case PGN_DM1	:
		spn_tmp = msg.data[4]>>5;
		dtcpage[0].spn = ( (msg.data[3]<<11)|(msg.data[2]<<5)|(spn_tmp) );
		dtcpage[0].fmi = (msg.data[4] & 0x1F);
		dtcpage[0].occ = (msg.data[5] & 0x7F);
		if(0 != dtcpage[0].spn)
		{
			dtc_received = 1;
			max_dtc = 1;
		}
		break;

	case PGN_BAM:
		break;

	case PGN_VEP1 :
		converter16.value8[1] = msg.data[5];
		converter16.value8[0] = msg.data[4];
		CANHandler_updateReading("BattVGE",converter16.value16);
		break;

	case PGN_HOURS	:
		converter32.value8[3] = msg.data[3];
		converter32.value8[2] = msg.data[2];
		converter32.value8[1] = msg.data[1];
		converter32.value8[0] = msg.data[0];
		CANHandler_updateReading("Hours",converter32.value32);
//		CANHandler_sendACK(0x18FEE500,0);
		break;

	default:
		break;

	}
	Leave_Critical_Section
}

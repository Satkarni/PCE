/*
 * PCE_CAN.c
 *
 * Created: 8/2/2016 10:31:05 AM
 *  Author: Piyush
 */ 


#include "PCE_CAN.h"

uint8_t safetyCounter;


void init()
{
	ioHandler_initKeypad();
	Ext_Int_init();

	ioHandler_enablePowerLED();
	ioHandler_disableRelayStop();
	ioHandler_disableRelayBuzzer();

	DDRD |= (1<<CAN_STBY);
	PORTD &= ~(1<<CAN_STBY);

	initHardwareTimer1();
	for(uint8_t i=0; i<ALARM_LIST_LENGTH; i++)
	{
		alarm_events[i] = 0;
		trip_events[i] = 0;
	}

	CANHandler_init();
	analogScanner_init();
	initialiseTimerQueue();
	display_init();
	enableHardwareTimer1(TIMER1_RATE);
}

void timerTick()
{
	Enter_Critical_Section
//	recheckTimerQueue();			//Not required in this case because sensors are not dynamically enabled/disabled
	clockTimerQueue();
	processSensors();
	Leave_Critical_Section
}

void requestHours()
{
//	if(display_HEAD.value != 3)
//		return;

	DEBUG_LED1_ON;

		can_t msg_tx;

		Enter_Critical_Section

		uint32_t tmp = 0x18;
		uint32_t pgn = ((tmp << 24) | (PGN_HOURS<<8) | 0xFF);

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


void checkSafety()
{
	if(getSensorValue("RPM") > 500*8 && safetyCounter >= 5)
	{
		safetyCounter = 6;
		allowAlarm("LOP");
		allowTrip("LOP");
		allowAlarm("GrBxPr");
		allowTrip("GrBxPr");
		if(page_current_safety != STATUS_SAFETY_ENABLED)
		{
			page_current_safety = STATUS_SAFETY_ENABLED;
			display_drawDynamicPage(0);
		}
	}
	else if(getSensorValue("RPM") < 200*8)
	{
		safetyCounter = 0;
		revokeAlarm("LOP");
		revokeTrip("LOP");
		revokeAlarm("GrBxPr");
		revokeTrip("GrBxPr");
		if(page_current_safety != STATUS_SAFETY_DISABLED)
		{
			page_current_safety = STATUS_SAFETY_DISABLED;
			display_drawDynamicPage(0);
		}
	}
	else if(getSensorValue("RPM") > 200*8 && getSensorValue("RPM") < 500*8 )
	{
		safetyCounter = 0;
	}
}

void checkMute()
{
	if(isMuteActive())
	{
		ioHandler_disableRelayBuzzer();
	}
}
void checkSensors()
{
	if(isAlarmEnabled("LOP") == 1)
	{
		if(alarms[LOP_ALARM].enabled == 0)
			setAlarmEvent(LOP_ALARM);
	}
	if(isAlarmEnabled("LOT") == 1)
	{
		if(alarms[LOT_ALARM].enabled == 0)
			setAlarmEvent(LOT_ALARM);
	}
	if(isAlarmEnabled("ECT") == 1)
	{
		if(alarms[ECT_ALARM].enabled == 0)
			setAlarmEvent(ECT_ALARM);
	}
	if(isAlarmEnabled("GrBxPr") == 1)
	{
		if(alarms[GBP_ALARM].enabled == 0)
			setAlarmEvent(GBP_ALARM);
	}
	if(isAlarmEnabled("GrBxTmp") == 1)
	{
		if(alarms[GBT_ALARM].enabled == 0)
			setAlarmEvent(GBT_ALARM);
	}
	if(isAlarmEnabled("ExTmp") == 1)
	{
		if(alarms[EXT_ALARM].enabled == 0)
			setAlarmEvent(EXT_ALARM);
	}


	if(isTripEnabled("RPM") == 1)
	{
		if(trips[RPM_TRIP].enabled == 0)
			setTripEvent(RPM_TRIP);
	}
	if(isTripEnabled("LOP") == 1)
	{
		if(trips[LOP_TRIP].enabled == 0)
			setTripEvent(LOP_TRIP);
	}
	if(isTripEnabled("LOT") == 1)
	{
		if(trips[LOT_TRIP].enabled == 0)
			setTripEvent(LOT_TRIP);
	}
	if(isTripEnabled("ECT") == 1)
	{
		if(trips[ECT_TRIP].enabled == 0)
			setTripEvent(ECT_TRIP);
	}
	if(isTripEnabled("GrBxPr") == 1)
	{
		if(trips[GBP_TRIP].enabled == 0)
			setTripEvent(GBP_TRIP);
	}
	if(isTripEnabled("GrBxTmp") == 1)
	{
		if(trips[GBT_TRIP].enabled == 0)
			setTripEvent(GBT_TRIP);
	}
	if(isTripEnabled("ExTmp") == 1)
	{
		if(trips[EXT_TRIP].enabled == 0)
			setTripEvent(EXT_TRIP);
	}
}

int main(void)
{
	init();
	sei();
	wdt_enable(WDTO_2S);
	while(1)
	{
		if(timer1sFlag == 1)
		{
			timerTick();
			timer1sFlag = 0;
			safetyCounter += 1;

		}
		if(timerIntFlag == 1)
		{
			analogScanner_scan();
			timerIntFlag = 0;
		}
		CANHandler_scan();
		checkSafety();
		selectEvent();
		checkSensors();
		checkMute();
		if(SW_RIGHT)
		{
			_delay_ms(250);
			setKeypadEvent(RIGHT_ARROW);
		}
		else if(SW_LEFT)
		{
			_delay_ms(250);
			setKeypadEvent(LEFT_ARROW);
		}
		else if(SW_UP)
		{
			_delay_ms(250);
			setKeypadEvent(UP_ARROW);
		}
		else if(SW_DOWN)
		{
			_delay_ms(250);
			setKeypadEvent(DOWN_ARROW);
		}
		else if(SW_ENTER_P)
		{
			_delay_ms(250);
			setKeypadEvent(ENTER);
		}
	}

}

/*
 * ioHandler.c
 *
 * Created: 8/4/2016 11:33:35 AM
 *  Author: Piyush
 */ 

#include "inputHandler.h"

void keypadInit()
{
	EICRB |= ((0<<ISC40) |(1<<ISC41) | (1<<ISC50) | (1<<ISC51));
	EIMSK |= (1<<INT4) | (1<<INT5);
	interrupt_received = 0;
}

uint8_t keypadDebounce(enum button keypress)
{
	static uint8_t keyMap[7];
	uint8_t i;

	for(i=0; i < 7; i++)
	{
		if (keypress == i)
			keyMap[i] += 1;
		else
			keyMap[i] = 0;
	}
	if (keyMap[keypress] >= 2)
	{
		keyMap[keypress] = 0;
		return 1;
	}
	else
		return 0;
}

void scanKeypad()
{
	if(interrupt_received == 0)
		return;

	uint8_t register_data, tmp;
	register_data = MCP23S17_read(INTCAPA);
	tmp = ~register_data;
	interrupt_received = 0;
	MCP23S17_write(GPINTENA,DISABLE);
	MCP23S17_write(GPINTENA,ENABLE);
	switch(tmp)
	{
	case (1<<UP_ARROW)	:				//Data = 0000 0010
		if(keypadDebounce(EN_UP_ARROW))
			triggerUpArrow();
		break;

	case (1<<LEFT_ARROW):				//Data = 0000 0100
		if(keypadDebounce(EN_DOWN_ARROW))
			triggerLeftArrow();
		break;

	case (1<<ENTER):					//Data = 0000 1000
		if(keypadDebounce(EN_ENTER))
			triggerEnter();
		break;

	case (1<<RIGHT_ARROW):			//Data = 0001 0000
		if(keypadDebounce(EN_RIGHT_ARROW))
			triggerRightArrow();
		break;

	case (1<<DOWN_ARROW):				//Data = 0010 0000
		if(keypadDebounce(EN_DOWN_ARROW))
			triggerDownArrow();
		break;

	case (1<<RESET_CLEAR):			//Data = 0100 0000
		if(keypadDebounce(EN_RESET_CLEAR))
			triggerResetClear();
		break;

	case (1<<MUTE):					//Data = 1000 0000
		if(keypadDebounce(EN_MUTE))
			triggerMute();
		break;
	}
}

void triggerUpArrow()
{
	uint8_t tmp;
	switch(current_form)
	{
	case ID_MAINFORM :
		display_brightnessIncrement();
		break;

	case ID_PINFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		switch(display_pinCursor)
		{
		case CUR_PINBOX1 :
			tmp = display_getObjectValue(ID_PINBOX1);
			if(tmp >= 9)
				display_setGauge(0,ID_PINBOX1);
			else
				display_setGauge((tmp+1),ID_PINBOX1);
			break;

		case CUR_PINBOX2 :
			tmp = display_getObjectValue(ID_PINBOX2);
			if(tmp >= 9)
				display_setGauge(0,ID_PINBOX2);
			else
				display_setGauge((tmp+1),ID_PINBOX2);
			break;

		case CUR_PINBOX3 :
			tmp = display_getObjectValue(ID_PINBOX3);
			if(tmp >= 9)
				display_setGauge(0,ID_PINBOX3);
			else
				display_setGauge((tmp+1),ID_PINBOX3);
			break;

		case CUR_PINBOX4 :
			tmp = display_getObjectValue(ID_PINBOX4);
			if(tmp >= 9)
				display_setGauge(0,ID_PINBOX4);
			else
				display_setGauge((tmp+1),ID_PINBOX4);
			break;
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_SETTINGSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		if(display_cursorValue > CUR_ACTDIAG && display_cursorValue <= CUR_RESET)
		{
			display_setCursor(display_cursorValue, 0);
			display_setCursor(display_cursorValue-1, 1);
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_SETPOINTSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		if(display_cursorValue > CUR_OVSSETPOINT && display_cursorValue <= CUR_AMMSETPOINT)
		{
			display_setCursor(display_cursorValue, 0);
			display_setCursor(display_cursorValue-1, 1);
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_RATIOFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		ratioFlag = 1;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_ALARMSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		if(display_cursorValue > CUR_OVSENABLED && display_cursorValue <= CUR_AMMENABLED)
		{
			display_setCursor(display_cursorValue,0);
			display_setCursor(display_cursorValue-1,1);
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;
	}
}

void triggerDownArrow()
{
	uint8_t tmp;
	switch(current_form)
	{
	case ID_MAINFORM :
		display_brightnessDecrement();
		break;

	case ID_PINFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		switch(display_pinCursor)
		{
		case CUR_PINBOX1 :
			tmp = display_getObjectValue(ID_PINBOX1);
			if(tmp <= 0)
				display_setGauge(9,ID_PINBOX1);
			else
				display_setGauge((tmp-1),ID_PINBOX1);
			break;
		case CUR_PINBOX2 :
			tmp = display_getObjectValue(ID_PINBOX2);
			if(tmp <= 0)
				display_setGauge(9,ID_PINBOX2);
			else
				display_setGauge((tmp-1),ID_PINBOX2);
			break;
		case CUR_PINBOX3 :
			tmp = display_getObjectValue(ID_PINBOX3);
			if(tmp <= 0)
				display_setGauge(9,ID_PINBOX3);
			else
				display_setGauge((tmp-1),ID_PINBOX3);
			break;
		case CUR_PINBOX4 :
			tmp = display_getObjectValue(ID_PINBOX4);
			if(tmp <= 0)
				display_setGauge(9,ID_PINBOX4);
			else
				display_setGauge((tmp-1),ID_PINBOX4);
			break;
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_SETTINGSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		if(display_cursorValue >= CUR_ACTDIAG && display_cursorValue < CUR_RESET)
		{
			display_setCursor(display_cursorValue, 0);
			display_setCursor(display_cursorValue+1, 1);
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_SETPOINTSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		if(display_cursorValue >= CUR_OVSSETPOINT && display_cursorValue < CUR_AMMSETPOINT)
		{
			display_setCursor(display_cursorValue, 0);
			display_setCursor(display_cursorValue+1, 1);
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_RATIOFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		ratioFlag = 2;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_ALARMSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		if(display_cursorValue >= CUR_OVSENABLED && display_cursorValue < CUR_AMMENABLED)
		{
			display_setCursor(display_cursorValue,0);
			display_setCursor(display_cursorValue+1,1);
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	}
}

void triggerLeftArrow()
{
	switch(current_form)
	{
	case ID_SETPOINTSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		setPointFlag = 2;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_PINFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		switch(display_pinCursor)
		{
		case CUR_PINBOX2 :
			display_setPINCursor(CUR_PINBOX2,0);
			display_setPINCursor(CUR_PINBOX2D,0);
			display_setPINCursor(CUR_PINBOX1D,1);
			display_setPINCursor(CUR_PINBOX1,1);
			break;
		case CUR_PINBOX3 :
			display_setPINCursor(CUR_PINBOX3,0);
			display_setPINCursor(CUR_PINBOX3D,0);
			display_setPINCursor(CUR_PINBOX2D,1);
			display_setPINCursor(CUR_PINBOX2,1);
			break;
		case CUR_PINBOX4 :
			display_setPINCursor(CUR_PINBOX4,0);
			display_setPINCursor(CUR_PINBOX4D,0);
			display_setPINCursor(CUR_PINBOX3D,1);
			display_setPINCursor(CUR_PINBOX3,1);
			break;
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;
	}

}

void triggerRightArrow()
{
	switch(current_form)
	{
	case ID_SETPOINTSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		setPointFlag = 1;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_PINFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		switch(display_pinCursor)
		{
		case CUR_PINBOX1 :
			display_setPINCursor(CUR_PINBOX1,0);
			display_setPINCursor(CUR_PINBOX1D,0);
			display_setPINCursor(CUR_PINBOX2D,1);
			display_setPINCursor(CUR_PINBOX2,1);
			break;
		case CUR_PINBOX2 :
			display_setPINCursor(CUR_PINBOX2,0);
			display_setPINCursor(CUR_PINBOX2D,0);
			display_setPINCursor(CUR_PINBOX3D,1);
			display_setPINCursor(CUR_PINBOX3,1);
			break;
		case CUR_PINBOX3 :
			display_setPINCursor(CUR_PINBOX3,0);
			display_setPINCursor(CUR_PINBOX3D,0);
			display_setPINCursor(CUR_PINBOX4D,1);
			display_setPINCursor(CUR_PINBOX4,1);
			break;
		}
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;
	}
}

void triggerEnter()
{
	switch (current_form)
	{
	case ID_MAINFORM :
		display_setForm(ID_PINFORM);
		display_setPINCursor(CUR_PINBOX1,1);
		break;

	case ID_PINFORM :
		display_checkPIN();
		break;

	case ID_THRCALFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		enterPressCount += 1;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_RATIOFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		ratioFlag = 3;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_ALARMSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		alarmToggleFlag = 1;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;

	case ID_SETPOINTSFORM :
		MCP23S17_write(GPINTENA,DISABLE);
		setPointFlag = 3;
		_delay_ms(200);
		MCP23S17_write(GPINTENA,ENABLE);
		break;


	case ID_SETTINGSFORM :
		switch(display_cursorValue)
		{
		case CUR_ACTDIAG :
			display_setCursor(CUR_ACTDIAG,0);
			display_setForm(ID_ACTTESTFORM);
			break;

		case CUR_ADCDIAG :
			display_setCursor(CUR_ADCDIAG,0);
			display_setForm(ID_DIAGFORM);
			break;

		case CUR_THRCAL :
			if(settingsAccess == 2)
			{
				display_setCursor(CUR_THRCAL,1);
				display_setForm(ID_THRCALFORM);
			}
			break;

		case CUR_ACTCAL :
			if(settingsAccess == 2)
			{
				display_setCursor(CUR_ACTCAL,0);
				display_setForm(ID_ACTCALFORM);
			}
			break;

		case CUR_RATIO :
			if(settingsAccess == 2)
			{
				display_setCursor(CUR_RATIO,0);
				display_setForm(ID_RATIOFORM);
			}
			break;

		case CUR_SETPOINTS :
			if(settingsAccess == 2)
			{
				display_setCursor(CUR_SETPOINTS,0);
				display_setForm(ID_SETPOINTSFORM);
			}
			break;

		case CUR_ALARMS :
			if(settingsAccess == 2)
			{
				display_setCursor(CUR_ALARMS,0);
				display_setForm(ID_ALARMSFORM);
			}
			break;

		case CUR_RESET :
			if(settingsAccess == 2)
			{
				factoryResetFlag = 1;
			}
			break;
		}
		break;
	}
}

void triggerResetClear()
{
	if(resetSensors() == 0)
	{
		RELAY1_WARNING_LAMP_OFF;
		RELAY2_BUZZER_OFF;
	}
	switch(current_form)
	{
	case ID_PINFORM :
		display_setForm(ID_MAINFORM);
		settingsAccess = 0;
		break;

	case ID_MAINFORM :
		break;

	case ID_SETTINGSFORM :
		display_setForm(ID_MAINFORM);
		settingsAccess = 0;
		break;

	case ID_THRCALFORM :
		enterPressCount = 0;
		display_setForm(ID_SETTINGSFORM);
		break;

	default :
		display_setForm(ID_SETTINGSFORM);
		break;
	}
}

void triggerMute()
{
	muteSensors();
	if(isMuteActive())
		RELAY2_BUZZER_OFF;
}

ISR (INT4_vect)
{
	interrupt_received = 1;
	//	_delay_ms(200);
}

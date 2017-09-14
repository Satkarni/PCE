/*
 * DisplayInterface.c
 *
 * Created: 9/6/2016 11:45:27 AM
 *  Author: Piyush
 */ 

#include "DisplayInterface.h"

void display_init()
{
	current_contrast = MAX_CONTRAST;
	current_form = ID_PCEFORM;
	display_objectValue = 0;
	display_cursorValue = 0;
	display_pinCursor = 0;
	ack_received = 0;
	uint8_t i;
	for(i=0; i <= 100; i++)
	{
		display_setGauge(i,ID_LOADGAUGE);
	}
	display_setForm(ID_MAINFORM);
}

void display_POST(void)
{
	uint8_t i;
	for(i=0; i<3; i++)
	{
		display_setFaultLED(1,ID_LOPFAULT);
		display_setFaultLED(1,ID_ECTFAULT);
		display_setFaultLED(1,ID_OVSFAULT);
		display_setFaultLED(1,ID_CHARGEFAULT);
		enableFaultLED();
		_delay_ms(150);

		display_setFaultLED(0,ID_LOPFAULT);
		display_setFaultLED(0,ID_ECTFAULT);
		display_setFaultLED(0,ID_OVSFAULT);
		display_setFaultLED(0,ID_CHARGEFAULT);
		disableFaultLED();
		_delay_ms(150);
	}

	display_setFaultLED(1,ID_LOPFAULT);
	display_setFaultLED(1,ID_ECTFAULT);
	display_setFaultLED(1,ID_OVSFAULT);
	display_setFaultLED(1,ID_CHARGEFAULT);
	enableFaultLED();

	for (i=0;i<=15;i++)
	{
		display_setGauge(i*24,ID_RPMGAUGE);
		display_setGauge(i*8,ID_ECTGAUGE);
		display_setGauge(i*8,ID_LOPGAUGE);
		display_setGauge(i*8,ID_AMMGAUGE);
	}

	display_setFaultLED(0,ID_LOPFAULT);
	display_setFaultLED(0,ID_ECTFAULT);
	display_setFaultLED(0,ID_OVSFAULT);
	display_setFaultLED(0,ID_CHARGEFAULT);
	display_setGauge(0,ID_RPMGAUGE);
	display_setGauge(0,ID_ECTGAUGE);
	display_setGauge(0,ID_LOPGAUGE);
	display_setGauge(0,ID_AMMGAUGE);
	disableFaultLED();
}

uint8_t display_getChecksum(uint8_t *packet, uint8_t packet_length)
{
	uint8_t i,chksum = 0;
	for (i=0;i<packet_length-1;i++)
	{
		chksum ^= *(packet+i);
	}
	return chksum;
}

void display_setGauge(uint16_t newValue, uint16_t gauge_id)
{
	uint8_t packet[6];
	uint16_t updateValue = 0, temp;

	packet[0] = COMMAND_WRITE;
	temp = (gauge_id>>8) & 0xFF;
	packet[1] = temp;
	packet[2] = gauge_id & 0xFF;

	switch(gauge_id)
	{
	case ID_RPMGAUGE:
		updateValue = (newValue >= 300)	?	300	:	newValue;
		break;

	case ID_ECTGAUGE:
		updateValue = (newValue >= 120)	?	120	:	newValue;
		break;

	case ID_LOPGAUGE:
		updateValue = (newValue	>= 100)	?	100	:	newValue;
		break;

	case ID_AMMGAUGE:
		updateValue = (newValue	>= 100)	?	100	:	newValue;
		break;

	default:
		updateValue = newValue;
		break;
	}

	temp = (updateValue>>8) & 0xFF;
	packet[3] = temp;
	packet[4] =  updateValue & 0xFF;
	packet[5] = display_getChecksum(packet,6);

	for(uint8_t i=0; i<6; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}
}
void display_setFaultLED(uint8_t ledState, uint16_t led_id)
{

	uint8_t packet[6];
	uint8_t updateValue;
	packet[0] = COMMAND_WRITE;
	packet[1] = (led_id>>8) & 0xFF;
	packet[2] = led_id & 0xFF;
	packet[3] = 0;

	if(ledState == 0 || ledState == 1)
	{
		updateValue = ledState;
	}
	else
	{
		updateValue = 0;
	}
	packet[4] = updateValue;
	packet[5] = display_getChecksum(packet,6);

	for(uint8_t i=0; i<6; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}
}

void display_setTime(uint8_t hrs, uint8_t mins)
{
	if(current_form != ID_MAINFORM)
		return;

	uint8_t packet[6];
	uint8_t updateValue; 
	static uint8_t am_pm;

	static uint8_t current_hrs = 0, current_mins = 0;

	if(mins != current_mins)
	{
		current_mins = mins;
		packet[0] = COMMAND_WRITE;
		packet[1] = (ID_MINUTES>>8) & 0xFF;
		packet[2] = ID_MINUTES & 0xFF;
		packet[3] = 0x00;

		updateValue = (mins <= 59) ? mins : 0;

		packet[4] = updateValue;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}

		packet[1] = (ID_AMPM>>8) & 0xFF;
		packet[2] = ID_AMPM & 0xFF;
		packet[4] = am_pm;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}
	}

	if(hrs != current_hrs)
	{
		current_hrs = hrs;
		packet[0] = COMMAND_WRITE;
		packet[1] = (ID_HOURS>>8) & 0xFF;
		packet[2] = ID_HOURS & 0xFF;
		packet[3] = 0x00;

		if(hrs >= 0 && hrs<=11)
		{
			updateValue = (hrs == 0) ? 12 : hrs;
			am_pm = 0x00;
		}
		else
		{
			updateValue = (hrs == 12) ? 12 : hrs-12;
			am_pm = 0x01;
		}
		packet[4] = updateValue;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}


		packet[1] = (ID_AMPM>>8) & 0xFF;
		packet[2] = ID_AMPM & 0xFF;
		packet[4] = am_pm;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}
	}


}

void display_setDate(uint8_t date, uint8_t month, uint8_t year)
{
	if(current_form != ID_MAINFORM)
		return;

	uint8_t packet [6];
	uint8_t updateValue;

	static uint8_t current_date = 0, current_month = 0, current_year = 0;

	if(date != current_date)
	{
		current_date = date;
		packet[0] = COMMAND_WRITE;
		packet[1] = (ID_DATE>>8) & 0xFF;
		packet[2] = ID_DATE & 0xFF;
		packet[3] = 0x00;

		updateValue = (date <= 31) ? date-1 : 0;

		packet[4] = updateValue;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}
	}

	if(month != current_month)
	{
		current_month = month;
		packet[0] = COMMAND_WRITE;
		packet[1] = (ID_MONTH>>8) & 0xFF;
		packet[2] = ID_MONTH & 0xFF;
		packet[3] = 0x00;

		updateValue = (month <= 12) ? month-1 : 0;

		packet[4] = updateValue;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}
	}

	if(year != current_year)
	{
		current_year = year;
		packet[0] = COMMAND_WRITE;
		packet[1] = (ID_YEAR>>8) & 0xFF;
		packet[2] = ID_YEAR & 0xFF;
		packet[3] = 0x00;

		updateValue = (year <= 99) ? year : 0;

		packet[4] = updateValue;
		packet[5] = display_getChecksum(packet,6);

		for(uint8_t i=0; i<6; i++)
		{
			RS232_writechar(packet[i]);
		}
		ack_received = 0;
		while(ack_received == 0){
		}
	}
}
void display_setForm(uint16_t form_id)
{
	uint16_t tmp;
	if(current_form == form_id)
		return;

	switch (form_id)
	{
	case ID_SETTINGSFORM :
		tmp = CUR_ADCDIAG;
		while(tmp <= CUR_RESET)
		{
			display_setCursor(tmp,0);
			tmp += 1;
		}
		display_setCursor(CUR_ACTDIAG,1);
		if(settingsAccess == 1)
		{
			display_setGauge(0,ID_LOGINMSG);
			display_setGauge(1,ID_THRDEV);
			display_setGauge(1,ID_ACTDEV);
			display_setGauge(1,ID_RATIODEV);
			display_setGauge(1,ID_SETPOINTDEV);
			display_setGauge(1,ID_ALARMSDEV);
			display_setGauge(1,ID_RESETDEV);
		}
		else if (settingsAccess == 2)
		{
			display_setGauge(1,ID_LOGINMSG);
			display_setGauge(0,ID_THRDEV);
			display_setGauge(0,ID_ACTDEV);
			display_setGauge(0,ID_RATIODEV);
			display_setGauge(0,ID_SETPOINTDEV);
			display_setGauge(0,ID_ALARMSDEV);
			display_setGauge(0,ID_RESETDEV);
		}
		break;

	case ID_PINFORM :
		tmp = CUR_PINBOX1;
		while (tmp <= CUR_PINBOX4D)
		{
			display_setPINCursor(tmp,0);
			tmp += 1;
		}
		display_setPINCursor(CUR_PINBOX1D,1);
		display_setPINCursor(CUR_PINBOX1,1);

		tmp = ID_PINBOX1;
		while(tmp <= ID_PINBOX4)
		{
			display_setGauge(0,tmp);
			tmp += 1;
		}
		display_setGauge(0,ID_PINMSG);
		break;

	case ID_ALARMSFORM :
		tmp = CUR_ECTENABLED;
		while(tmp <= CUR_AMMENABLED)
		{
			display_setCursor(tmp,0);
			tmp += 1;
		}
		display_setCursor(CUR_OVSENABLED,1);
		break;

	case ID_THRCALFORM:
		display_setGauge(0,ID_THRCALMSG);
		break;

	case ID_SETPOINTSFORM :
		tmp = CUR_RUNSETPOINT;
		while(tmp <= CUR_AMMSETPOINT)
		{
			display_setCursor(tmp,0);
			tmp += 1;
		}
		display_setCursor(CUR_OVSSETPOINT,1);
		display_setGauge(0,ID_SETPOINTCHECK);
		break;
	}

	uint8_t packet[6];
	current_form = form_id;
	packet[0] = COMMAND_WRITE;
	packet[1] = (form_id>>8) & 0xFF;
	packet[2] = form_id & 0xFF;
	packet[3] =	0; 
	packet[4] =	0; 
	packet[5] = display_getChecksum(packet,6);

	for(uint8_t i=0; i<6; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}
}

uint16_t display_getObjectValue(uint16_t object_id)
{
	uint8_t packet[4], i;

	packet[0] = COMMAND_READ;
	packet[1] = (object_id>>8) & 0xFF;
	packet[2] = object_id & 0xFF;
	packet[3] = display_getChecksum(packet,4);
	for(i=0; i<4; i++)
	{
		RS232_writechar(packet[i]);
	}

	ack_received = 0;
	while(ack_received == 0){
	}

	return display_objectValue;
}

void display_Reset()
{
	DDRD |= (1<<PD7);
	PORTD |=  (1<<PD7);
	_delay_us(5);
	PORTD &= ~(1<<PD7);
	_delay_ms(5000);
}

void display_brightnessIncrement()
{
	uint8_t packet[3];
	current_contrast = (current_contrast >= MAX_CONTRAST) ?  MAX_CONTRAST : current_contrast+2;
	packet[0] = COMMAND_CONTRAST;
	packet[1] = current_contrast;
	packet[2] = display_getChecksum(packet,3);

	for(uint8_t i=0; i<3; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}
}

void display_brightnessDecrement()
{
	uint8_t packet[3];
	current_contrast = (current_contrast <= MIN_CONTRAST) ? MIN_CONTRAST : current_contrast-2;
	packet[0] = COMMAND_CONTRAST;
	packet[1] = current_contrast;
	packet[2] = display_getChecksum(packet,3);
	for(uint8_t i=0; i<3; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}
}

void display_processRead()
{
	static uint8_t read_buffer[6], packet_index = 0;
	if (packet_index == 0)
	{
		read_buffer[0] = RS232_readchar();
		if(read_buffer[0] == 0x05 || read_buffer[0] == 0x07)
		{
			packet_index = 1;
		}
		else
			ack_received = 1;
	}
	else
	{
		read_buffer[packet_index] = RS232_readchar();
		packet_index += 1;

		if(packet_index >= 5)
		{
			display_objectValue = (read_buffer[3]<<8) | (read_buffer[4]);
			packet_index = 0;
			ack_received = 1;
		}
	}
}

void display_updateADCGauges(void)
{
	if(current_form != ID_DIAGFORM)
		return;

	uint16_t adcdata;
	double tmp;

	adcdata = ADC_filterRead(ADC_POT_CHANNEL);
	display_setGauge(adcdata,ID_ADC0RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC0VOLTAGE);

	adcdata = ADC_filterRead(ADC_THROTTLE_CHANNEL);
	display_setGauge(adcdata,ID_ADC1RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC1VOLTAGE);

	adcdata = ADC_filterRead(ADC_DRV_CHANNEL);
	display_setGauge(adcdata,ID_ADC2RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC2VOLTAGE);

	adcdata = ADC_filterRead(ADC_LOP_CHANNEL);
	display_setGauge(adcdata,ID_ADC3RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC3VOLTAGE);

	adcdata = ADC_filterRead(ADC_ECT_CHANNEL);
	display_setGauge(adcdata,ID_ADC4RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC4VOLTAGE);

	adcdata = ADC_filterRead(ADC_CS_CHANNEL);
	display_setGauge(adcdata,ID_ADC5RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC5VOLTAGE);

	adcdata = ADC_filterRead(ADC_SR_CHANNEL);
	display_setGauge(adcdata,ID_ADC6RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC6VOLTAGE);

	adcdata = ADC_filterRead(ADC_420_CHANNEL);
	display_setGauge(adcdata,ID_ADC7RAW);
	tmp = ((double)adcdata*5000)/1024;
	adcdata = (uint16_t)tmp;
	display_setGauge(adcdata,ID_ADC7VOLTAGE);

	adcdata = ADC_getThrottleMin();
	display_setGauge(adcdata,ID_THROTTLEMIN);

	adcdata = ADC_getThrottleMax();
	display_setGauge(adcdata,ID_THROTTLEMAX);
}

void display_updateMainGauges()
{
	if(current_form != ID_MAINFORM)
		return;

	uint16_t tmp, error;
	static uint16_t prevRPMValue = 0, prevECTValue, prevLOPValue, prevAmmValue, prevEngineCondition, prevHoursFloat, prevHoursInt;

	tmp = (uint16_t)getSensorValue("RPM");
	if(prevRPMValue != tmp)
	{
		display_setGauge((uint16_t)getSensorValue("RPM"),ID_RPMGAUGE);
		prevRPMValue = tmp;
	}

	tmp = (uint16_t)getSensorValue("ECT");
	if(prevECTValue != tmp)
	{
		display_setGauge(tmp,ID_ECTGAUGE);
		prevECTValue = tmp;
	}

	tmp = (uint16_t)getSensorValue("LOP");
	if(prevLOPValue != tmp)
	{
		display_setGauge(tmp,ID_LOPGAUGE);
		prevLOPValue = tmp;
	}

	tmp = (uint16_t)getSensorValue("AMM");
	error = abs(prevAmmValue - tmp);
	if(error > 2)
	{
		display_setGauge(tmp,ID_AMMGAUGE);
		prevAmmValue = tmp;
	}

	if(engineCondition != prevEngineCondition)
	{
	    display_setGauge(engineCondition,ID_ENGINECON);
	    prevEngineCondition = engineCondition;
	}

	if(isAlarmEnabled("RPM"))
	{
		display_setFaultLED(1,ID_OVSFAULT);
		enableFaultLED();
		RELAY1_WARNING_LAMP_ON;
		if(isMuteActive() == 0)
			RELAY2_BUZZER_ON;
	}
	else
	{
		display_setFaultLED(0,ID_OVSFAULT);
	}

	if(isAlarmEnabled("ECT"))
	{
		display_setFaultLED(1,ID_ECTFAULT);
		enableFaultLED();
		RELAY1_WARNING_LAMP_ON;
		if(isMuteActive() == 0)
			RELAY2_BUZZER_ON;
	}
	else
	{
		display_setFaultLED(0,ID_ECTFAULT);
	}

	if(isAlarmEnabled("LOP"))
	{
		display_setFaultLED(1,ID_LOPFAULT);
		enableFaultLED();
		RELAY1_WARNING_LAMP_ON;
		if(isMuteActive() == 0)
			RELAY2_BUZZER_ON;
	}
	else
	{
		display_setFaultLED(0,ID_LOPFAULT);
	}

	if(isAlarmEnabled("AMM"))
	{
		display_setFaultLED(1,ID_CHARGEFAULT);
		enableFaultLED();
		RELAY1_WARNING_LAMP_ON;
		if(isMuteActive() == 0)
			RELAY2_BUZZER_ON;
	}
	else
	{
		display_setFaultLED(0,ID_CHARGEFAULT);
	}

	if(isAlarmEnabled("RPM") == 0 && isAlarmEnabled("ECT") == 0 && isAlarmEnabled("LOP") == 0 && isAlarmEnabled("AMM") == 0)
	{
		disableFaultLED();
	}


	tmp = getEngineHours();
	if(prevHoursInt != tmp)
	{
	    if(tmp < 100)
	    {
	        display_setGauge(0,ID_ENGHOURS1);
	        display_setGauge(tmp,ID_ENGHOURS2);
	    }
	    else if (tmp >= 100)
	    {
	        uint16_t value;
	        value = tmp/100;
	        display_setGauge(value,ID_ENGHOURS1);
	        value = tmp%100;
	        display_setGauge(value,ID_ENGHOURS2);
	    }
	    prevHoursInt = tmp;
	}

	tmp = (getEngineSeconds() * 10) / 3600;
	if(prevHoursFloat != tmp)
	{
	    display_setGauge(tmp,ID_ENGHOURS3);
	    prevHoursFloat = tmp;
	}

}

void display_updateACTGauges()
{
	if(current_form != ID_ACTTESTFORM)
		return;

	display_setGauge(ADC_getThrottleMax(),ID_THROTTLEMAX);
	display_setGauge(ADC_getThrottleMin(),ID_THROTTLEMIN);

	display_setGauge((100-actuator_data.throttle_percentage),ID_THROTTLEPERCENT);

	display_setGauge(actuator_data.speed_percentage,ID_SPEEDPERCENT);

	display_setGauge(actuator_data.command_percentage,ID_COMMANDPOS);

	display_setGauge(ADC_getActuatorMax(),ID_ACTUATORMAX);
	display_setGauge(actuator_data.actuator_percentage,ID_ACTUALPOS);
	display_setGauge(actuator_data.actuator_percentage,ID_ACTUALPERCENT);

	display_setGauge((uint16_t)(duty_cycle*100.0/255.0),ID_DUTYCYCLE);
}

void display_setCursor(uint16_t cursor_id, uint8_t enable)
{
	uint8_t packet[6];

	if (enable == 1)
	{
		packet[4] = 1;
		display_cursorValue = cursor_id;
	}
	else if (enable == 0)
	{
		packet[4] = 0;
	}
	else
	{
		return;
	}
	packet[0] = COMMAND_WRITE;
	packet[1] = (cursor_id>>8) & 0xFF;
	packet[2] = cursor_id & 0xFF;
	packet[3] = 0;
	packet[5] = display_getChecksum(packet,6);

	for(uint8_t i=0; i<6; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}
}

void display_setPINCursor(uint16_t cursor_id, uint8_t enable)
{
	uint8_t packet[6];

	if (enable == 1)
	{
		packet[4] = 1;
		display_pinCursor = cursor_id;
	}
	else if (enable == 0)
	{
		packet[4] = 0;
	}
	else
	{
		return;
	}
	packet[0] = COMMAND_WRITE;
	packet[1] = (cursor_id>>8) & 0xFF;
	packet[2] = cursor_id & 0xFF;
	packet[3] = 0;
	packet[5] = display_getChecksum(packet,6);

	for(uint8_t i=0; i<6; i++)
	{
		RS232_writechar(packet[i]);
	}
	ack_received = 0;
	while(ack_received == 0){
	}

}

void display_checkPIN()
{
	uint16_t pin = 0;
	pin += display_getObjectValue(ID_PINBOX1);
	pin *= 10;
	pin += display_getObjectValue(ID_PINBOX2);
	pin *= 10;
	pin += display_getObjectValue(ID_PINBOX3);
	pin *= 10;
	pin += display_getObjectValue(ID_PINBOX4);

	if (pin == OPERATOR_PASSWORD)
	{
		display_setGauge(1,ID_PINMSG);
		settingsAccess = 1;
		_delay_ms(500);
		display_setForm(ID_SETTINGSFORM);
	}
	else if (pin == DEVELOPER_PASSWORD)
	{
		display_setGauge(1,ID_PINMSG);
		settingsAccess = 2;
		_delay_ms(500);
		display_setForm(ID_SETTINGSFORM);
	}
	else
	{
		display_setGauge(2,ID_PINMSG);
		_delay_ms(500);
		display_setForm(ID_MAINFORM);
		settingsAccess = 0;
	}
}

ISR(USART1_RX_vect)
{
	display_processRead();
}

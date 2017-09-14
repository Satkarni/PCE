/*
 * main.c
 *
 * Created: 8/4/2016 11:15:32 AM
 *  Author: Piyush
 */ 

#include "KTA.h"
#include "ECTLookup.h"
#include "LOPLookup.h"


ISR(TIMER2_COMP_vect)
{
	if(counter%125 == 0)			//250ms counter
	{
		DEBUG_LED1_LD8_TOGGLE;
		wdt_reset();
		updateRPMSensor();
	}
	if(counter == 500)			//1sec counter
	{
	    if(engineCondition == 2)
	    {
	        incrementEngineSeconds();
	    }
		oneSecondFlag = 1;
		counter = 1;
	}
	counter++;
}

void initSensors()
{
	addSensor("RPM");
	setSensorMinMax("RPM",0,300);
	allowAlarm("RPM");
	revokeTrip("RPM");
	enableTripAtHigh("RPM",1);
	setAlarmThresholdHigh("RPM",getOVSThreshold());
	setAlarmTimer("RPM",5);
	setFailureTimer("RPM",120);

	addSensor("ECT");
	setSensorMinMax("ECT",0,120);
	revokeAlarm("ECT");
	revokeTrip("ECT");
	enableTripAtHigh("ECT",1);
	setAlarmThresholdHigh("ECT",getECTThreshold());
	setAlarmTimer("ECT",5);
	setFailureTimer("ECT",120);

	addSensor("LOP");
	setSensorMinMax("LOP",0,100);
	revokeAlarm("LOP");
	revokeTrip("LOP");
	enableTripAtHigh("LOP",0);
	setAlarmThresholdLow("LOP",getLOPThreshold());
	setAlarmTimer("LOP",5);
	setFailureTimer("LOP",120);

	addSensor("AMM");
	setSensorMinMax("AMM",0,100);
	revokeAlarm("AMM");
	revokeTrip("AMM");
	enableTripAtHigh("AMM",0);
	setAlarmThresholdLow("AMM",getAMMThreshold());
	setAlarmTimer("AMM",5);
	setFailureTimer("AMM",120);

	initialiseTimerQueue();
}

void clockSensors()
{
	if(current_form != ID_MAINFORM)
		return;

	clockTimerQueue();
	processSensors();
}

void updateDateTime()
{
	if(current_form != ID_MAINFORM)
		return;

	struct rtc currentTime;
	ds1307Getdate(&currentTime);
	display_setTime(currentTime.hours,currentTime.minutes);
	display_setDate(currentTime.date,currentTime.month,currentTime.year);
}

void updateRPMSensor()
{

		Timers_RPMTimerDisable();

		if(current_form != ID_MAINFORM)
			return;

		double rpm=0, ratio;
		uint16_t tmp;
		ratio = (double)getPulleyRatio()/100.0;
		Enter_Critical_Section
		rpm = TCNT3*4;
		rpm = (rpm <= RPM_FREQ_MIN) ? 0 :
				((rpm >= RPM_FREQ_MAX)? 0 : rpm) ;	// limit freq between 60< freq < freq_max

		rpm = (rpm/RPM_NO_OF_TEETH) * RPM_SECONDS * (7.5 / ratio);			// convert to rpm
		tmp = (uint16_t)(rpm/10);
		setSensorValue("RPM",tmp);

		if(tmp >= getRunThreshold())
		{
			if(isEnabledECTAlarm() == 1)
				allowAlarm("ECT");

			if(isEnabledLOPAlarm() == 1)
				allowAlarm("LOP");

			if(isEnabledAMMAlarm() == 1)
				allowAlarm("AMM");

			if (engineCondition != 2)
			    engineCondition = 2;


			RELAY4_RUNNING_ON;
			RELAY3_LOCKOUT_OFF;
			RELAY5_SPARE_ON;
		}
		else if(tmp < 20)
		{
			if(isEnabledECTAlarm() == 1)
			    revokeAlarm("ECT");

			if(isEnabledLOPAlarm() == 1)
			    revokeAlarm("LOP");

			if(isEnabledAMMAlarm() == 1)
			    revokeAlarm("AMM");


			RELAY4_RUNNING_OFF;

			if(DI_IDLE_ON || DI_IDLE_OFF || DI_RUN_OFF)
			{
			    RELAY3_LOCKOUT_ON;
			    if (engineCondition != 0)
			        engineCondition = 0;
			}
			else
			{
			    RELAY3_LOCKOUT_OFF;
			    if (engineCondition != 1)
			        engineCondition = 1;
			}

			RELAY5_SPARE_OFF;
		}
		Leave_Critical_Section
		Timers_RPMTimerEnable();
}

void updateSensors()
{
	uint16_t reading, adcdata;
	 adcdata = ADC_filterRead(ADC_ECT_CHANNEL);
	 if(adcdata < ECT_MIN)
		 reading = 120;
	 else if(adcdata > ECT_MAX)
		 reading = 0;
	 else
		 reading = pgm_read_byte_near(ECT_ADCLookup + adcdata);
	 setSensorValue("ECT",reading);

	 adcdata = ADC_filterRead(ADC_LOP_CHANNEL);
	 if(adcdata < LOP_MIN)
		 reading = 0;
	 else if(adcdata > LOP_MAX)
		 reading = 7.1;
	 else
		 reading = pgm_read_byte_near(LOP_ADCLookup + adcdata);
	 setSensorValue("LOP",reading);

	 adcdata = ADC_filterRead(ADC_CS_CHANNEL);
	 if(adcdata < AMM_MIN)
		 reading = 0;
	 else if(adcdata > AMM_MAX)
		 reading = 100;
	 else
	 {
	     double offset, tmp;
	     offset = (double)ammeterOffset*5000/1024;
	     tmp = ((((double)adcdata * 5000)/1024.0)-offset) / 5; //5 = 15/3 where 3 is number of wires and 15mV/A is sensitivity
	     if(tmp > 100)
	         reading = 100;
	     else if (tmp < 0)
	         reading = 0;
	     else
	         reading = (uint16_t)tmp;
		 //reading = (adcdata*100)/512;
	 }
	 setSensorValue("AMM",reading);
}

void checkActuator()
{
	if(current_form == ID_THRCALFORM)
	{
		if(ADC_read(ADC_DRV_CHANNEL) > 0)
		{
			ACT_DIR_IN;
			Timers_setDutyCycle(100);
		}
		else
		{
			Timers_setDutyCycle(0);
		}
		return;
	}

	double throttle_position, throttle_current, throttle_max, throttle_min;
	double speed_position;
	double actuator_current, actuator_max;
	uint16_t command_position, actuator_position;
	int8_t error;
	uint8_t cycle;

	actuator_current = (double)ADC_read(ADC_DRV_CHANNEL);
	actuator_max = (double)ADC_getActuatorMax();
	actuator_position = (actuator_current >= actuator_max) ? 100 : (actuator_current/actuator_max)*100;
	actuator_data.actuator_percentage = actuator_position;

	throttle_current = (double)ADC_read(ADC_THROTTLE_CHANNEL);
	throttle_max = (double)ADC_getThrottleMax();
	throttle_min = (double)ADC_getThrottleMin();

	if(throttle_current >= throttle_max)
	{
		throttle_position = 0;
	}
	else if(throttle_current <= throttle_min)
	{
		throttle_position = 100;
	}
	else
	{
		throttle_position = 100 - ((100*(throttle_current - throttle_min))/(throttle_max-throttle_min));
	}

	actuator_data.throttle_percentage = (uint16_t)throttle_position;

	speed_position = ((double)ADC_read(ADC_POT_CHANNEL)/1022)*100;
	actuator_data.speed_percentage = (uint16_t)speed_position;

	command_position = (uint16_t)(throttle_position * speed_position/100);
	actuator_data.command_percentage = command_position;

	if(DI_IDLE_ON || DI_IDLE_OFF || DI_RUN_OFF)
	{
		if(actuator_current > 4)
		{
			ACT_DIR_IN;
			Timers_setDutyCycle(100);
		}
		else
		{
			Timers_setDutyCycle(0);
		}
		return;
	}

	error = command_position - actuator_position;
	if(error > 0 && command_position == 100)
	{
		ACT_DIR_OUT;
		Timers_setDutyCycle(100);
	}
	else if (error < 0 && command_position == 0)
	{
		ACT_DIR_IN;
		Timers_setDutyCycle(100);
	}
	else if (error > 2)
	{
		ACT_DIR_OUT;
		cycle = (abs(error+ACTOUT_MIN_PWM) > 100) ? 100 : abs(error+ACTOUT_MIN_PWM);
		Timers_setDutyCycle(cycle);
	}
	else if(error < -2)
	{
		ACT_DIR_IN;
		cycle = (abs(error-ACTIN_MIN_PWM) > 100) ? 100 : abs(error-ACTIN_MIN_PWM);
		Timers_setDutyCycle(cycle);
	}
	else
		Timers_setDutyCycle(0);
}

void RelayInit(void)
{
	RELAY_REG_DDR |= ( (1<<RELAY1_WARNING_LAMP)|(1<<RELAY2_BUZZER)|(1<<RELAY3_LOCKOUT)| \
			(1<<RELAY4_RUNNING)|(1<<RELAY5_SPARE) );
	RELAY_REG_PORT &= ~( (1<<RELAY1_WARNING_LAMP)|(1<<RELAY2_BUZZER)|(1<<RELAY3_LOCKOUT)| \
			(1<<RELAY4_RUNNING)|(1<<RELAY5_SPARE) );
}

void DIinit(void)
{
	DI_REG_DDR &= ~( (1<<DI_START_KEYSW)|(1<<DI_STOP_KEYSW)|				\
			(1<<DI_RUN)|(1<<DI_IDLE)|							\
			(1<<DI_SPARE_5V_1)|(1<<DI_SPARE_5V_2)|				\
			(1<<DI_SPARE_24V_1)|(1<<DI_SPARE_24V_2) );
	DI_REG_PORT |= ( (1<<DI_START_KEYSW)|(1<<DI_STOP_KEYSW)|				\
			(1<<DI_RUN)|(1<<DI_IDLE)|							\
			(1<<DI_SPARE_5V_1)|(1<<DI_SPARE_5V_2)|				\
			(1<<DI_SPARE_24V_1)|(1<<DI_SPARE_24V_2) );
}

void init()
{
	spi_init();
	ds1307Init();
	RS232_init(BAUDRATE_232);
	RS232_enable(1,1);
	keypadInit();
	RelayInit();
	DIinit();
	MCP23S17_init();
	enablePowerLED();
	display_Reset();
	calibrationsInit();

	sei();
	display_init();
	updateDateTime();
	Timers_PWMInit();
	Timers_PWMEnable();
	Timers_setDutyCycle(0);
	DDRC |= (1<<DRV_DIRECTION);
	ACT_DIR_IN;
	Timers_setDutyCycle(100);
	display_POST();
	Timers_setDutyCycle(0);
	ADC_init();
	ammeterOffset = ADC_read(ADC_CS_CHANNEL);
	Enter_Critical_Section
	initSensors();
	Leave_Critical_Section
	initTimers();
	resetSensors();
}

int main(void)
{
	init();
	wdt_enable(WDTO_500MS);
	while(1)
	{
		checkActuator();
		if(oneSecondFlag == 1)
		{
			clockSensors();
			updateDateTime();
			oneSecondFlag = 0;
		}
		scanKeypad();
		updateSensors();
		display_updateMainGauges();
		display_updateADCGauges();
		display_updateACTGauges();
		enablePowerLED();
		calibrateActuator();
		calibrateThrottle();
		calibratePulleyRatio();
		toggleAlarms();
		adjustSetPoints();
		testMain();
		if(factoryResetFlag == 1)
		{
			factoryReset();
			factoryResetFlag = 0;
		}
	}
}

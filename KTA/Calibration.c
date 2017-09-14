/*
 * Calibration.c
 *
 *  Created on: 26-Oct-2016
 *      Author: piyush
 */

#include "Calibration.h"

uint16_t EEMEM pulleyRatio;
uint16_t EEMEM ovsThreshold;
uint16_t EEMEM runThreshold;
uint16_t EEMEM ectThreshold;
uint16_t EEMEM lopThreshold;
uint16_t EEMEM ammThreshold;
uint16_t EEMEM engineSeconds[1000];
uint16_t EEMEM engineHours = 0;

uint8_t EEMEM ovsEnabled;
uint8_t EEMEM ectEnabled;
uint8_t EEMEM lopEnabled;
uint8_t EEMEM ammEnabled;


void calibrationsInit()
{
	if(eeprom_read_word(&pulleyRatio) == 0xFFFF)
		eeprom_update_word(&pulleyRatio, PULLEY_RATIO * 100);

	if(eeprom_read_word(&ovsThreshold) == 0xFFFF)
		eeprom_update_word(&ovsThreshold, OVERSPEED_THRESHOLD/10);

	if(eeprom_read_word(&runThreshold) == 0xFFFF)
		eeprom_update_word(&runThreshold, RUN_THRESHOLD/10);

	if(eeprom_read_word(&ectThreshold) == 0xFFFF)
		eeprom_update_word(&ectThreshold, ECT_THRESHOLD);

	if(eeprom_read_word(&lopThreshold) == 0xFFFF)
		eeprom_update_word(&lopThreshold, LOP_THRESHOLD*10);

	if(eeprom_read_word(&ammThreshold) == 0xFFFF)
		eeprom_update_word(&ammThreshold, AMM_THRESHOLD);

	if(eeprom_read_byte(&ovsEnabled) == 0xFF)
		eeprom_update_byte(&ovsEnabled,1);

	if(eeprom_read_byte(&ectEnabled) == 0xFF)
		eeprom_update_byte(&ectEnabled,1);

	if(eeprom_read_byte(&lopEnabled) == 0xFF)
		eeprom_update_byte(&lopEnabled,1);

	if(eeprom_read_byte(&ammEnabled) == 0xFF)
		eeprom_update_byte(&ammEnabled,1);

	if(eeprom_read_word(&engineHours) == 0xFFFF)
	    eeprom_update_word(&engineHours,0);

//	for (uint16_t i=0; i < 1000; i++)
//	{
//	    eeprom_update_word(&engineSeconds[i],0xFFFF);
//	}
//	eeprom_update_word(&engineHours,0);

}

void calibratePulleyRatio (void)
{
	static uint8_t firstRun = 0;

	if(current_form != ID_RATIOFORM)
	{
		firstRun = 0;
		return;
	}

	uint16_t currentRatio;
	static uint16_t newRatio = 0;

	if(firstRun == 0)
	{
		currentRatio = eeprom_read_word(&pulleyRatio);
		display_setGauge(currentRatio,ID_ACTIVERATIO);
		display_setGauge(currentRatio,ID_NEWRATIO);
		newRatio = currentRatio;
		firstRun = 1;
	}

	switch(ratioFlag)
	{
	case  1:
		newRatio = (newRatio >= 500) ? 500 : newRatio+5;
		display_setGauge(newRatio,ID_NEWRATIO);
		ratioFlag = 0;
		break;

	case  2 	:
		newRatio = (newRatio <= 100) ? 100 : newRatio-5;
		display_setGauge(newRatio,ID_NEWRATIO);
		ratioFlag = 0;
		break;

	case  3 :
		eeprom_update_word(&pulleyRatio,newRatio);
		display_setGauge(newRatio,ID_ACTIVERATIO);
		ratioFlag = 0;
		firstRun = 0;
		break;

	default:
		break;
	}
}

void calibrateThrottle()
{
	if(current_form != ID_THRCALFORM)
		return;

	double thrMax, thrMin, thrFeedback, thrCurrent;
	static uint8_t setFlag = 0;

	thrCurrent = ADC_filterRead(ADC_THROTTLE_CHANNEL);
	display_setGauge(thrCurrent, ID_THRADC);
	thrMax = ADC_getThrottleMax();
	thrMin = ADC_getThrottleMin();

	thrCurrent = (thrCurrent < thrMin) ? thrMin : ((thrCurrent > thrMax) ? thrMax : thrCurrent);
	thrFeedback = 100.0*(thrCurrent - thrMin)/(thrMax-thrMin);
	display_setGauge((uint16_t)thrFeedback, ID_THRFEEDBACK);
	if(enterPressCount == 1 && setFlag == 0)
	{
		display_setGauge(1,ID_THRCALMSG);
		ADC_updateThrottleMax();
		setFlag = 1;
	}
	if(enterPressCount == 2 && setFlag == 1)
	{
		display_setGauge(2,ID_THRCALMSG);
		ADC_updateThrottleMin();
		setFlag = 0;
		enterPressCount = 0;
		_delay_ms(400);
		display_setForm(ID_SETTINGSFORM);
	}
}

void calibrateActuator()
{
	if(current_form != ID_ACTCALFORM)
		return;

	uint16_t prev_value = 0, current_value, timeout_counter = 0;

	ACT_DIR_IN;
	Timers_setDutyCycle(100);
	display_setGauge(100,ID_ACTPWM);
	display_setGauge(ADC_filterRead(ADC_DRV_CHANNEL),ID_ACTFEEDBACK);
	_delay_ms(250);

	while (ADC_filterRead(ADC_DRV_CHANNEL) > 5)
	{
		display_setGauge(ADC_filterRead(ADC_DRV_CHANNEL),ID_ACTFEEDBACK);
		_delay_ms(250);

		timeout_counter += 1;
		if(timeout_counter >= 20)
		{
			display_setGauge(2,ID_ACTCALMSG);
			_delay_ms(750);
			display_setForm(ID_SETTINGSFORM);
			display_setGauge(0,ID_ACTCALMSG);
			return;
		}
	}

	Timers_setDutyCycle(60);
	display_setGauge(60,ID_ACTPWM);
	ACT_DIR_OUT;
	_delay_ms(250);

	while(1)
	{
		current_value = ADC_filterRead(ADC_DRV_CHANNEL);
		display_setGauge(current_value, ID_ACTFEEDBACK);
		if(prev_value == current_value)
		{
			Timers_setDutyCycle(0);
			display_setGauge(0,ID_ACTPWM);
			if(current_value < 600)
			{
				display_setGauge(2,ID_ACTCALMSG);
				_delay_ms(750);
				display_setForm(ID_SETTINGSFORM);
				display_setGauge(0,ID_ACTCALMSG);
				return;
			}
			ADC_updateActuatorMax();
			display_setGauge(1,ID_ACTCALMSG);
			_delay_ms(750);
			display_setForm(ID_SETTINGSFORM);
			display_setGauge(0,ID_ACTCALMSG);
			return;
		}
		prev_value = current_value;
		_delay_ms(200);
	}
}

void toggleAlarms()
{
	if(current_form != ID_ALARMSFORM)
		return;

	uint8_t ovsState, ectState, lopState, ammState;

	ovsState = (eeprom_read_byte(&ovsEnabled) == 1) ? 1 : 0;
	ectState = (eeprom_read_byte(&ectEnabled) == 1) ? 1 : 0;
	lopState = (eeprom_read_byte(&lopEnabled) == 1) ? 1 : 0;
	ammState = (eeprom_read_byte(&ammEnabled) == 1) ? 1 : 0;
	display_setGauge(ovsState,ID_OVSENABLED);
	display_setGauge(ectState,ID_ECTENABLED);
	display_setGauge(lopState,ID_LOPENABLED);
	display_setGauge(ammState,ID_AMMENABLED);

	if(alarmToggleFlag == 1)
	{
		switch(display_cursorValue)
		{
		case CUR_OVSENABLED	:
			if(ovsState == 1)
			{
				revokeAlarm("RPM");
				eeprom_update_byte(&ovsEnabled,0);
			}
			else
			{
				allowAlarm("RPM");
				eeprom_update_byte(&ovsEnabled,1);
			}
			break;

		case CUR_ECTENABLED :
			if(ectState == 1)
			{
				revokeAlarm("ECT");
				eeprom_update_byte(&ectEnabled,0);
			}
			else
			{
				allowAlarm("ECT");
				eeprom_update_byte(&ectEnabled,1);
			}
			break;

		case CUR_LOPENABLED :
			if(lopState == 1)
			{
				revokeAlarm("LOP");
				eeprom_update_byte(&lopEnabled,0);
			}
			else
			{
				allowAlarm("LOP");
				eeprom_update_byte(&lopEnabled,1);
			}
			break;

		case CUR_AMMENABLED :
			if(ammState == 1)
			{
				revokeAlarm("AMM");
				eeprom_update_byte(&ammEnabled,0);
			}
			else
			{
				allowAlarm("AMM");
				eeprom_update_byte(&ammEnabled,1);
			}
			break;
		}
		alarmToggleFlag = 0;
	}
}

void adjustSetPoints()
{
	if(current_form != ID_SETPOINTSFORM)
		return;

	static uint16_t newOVS, newRUN, newECT, newLOP, newAMM, firstRun = 0;

	if(firstRun == 0)
	{
		newOVS = eeprom_read_word(&ovsThreshold)*10;
		newRUN = eeprom_read_word(&runThreshold)*10;
		newECT = eeprom_read_word(&ectThreshold);
		newLOP = eeprom_read_word(&lopThreshold);
		newAMM = eeprom_read_word(&ammThreshold);

		display_setGauge(newOVS,ID_OVSSETPOINT);
		display_setGauge(newRUN,ID_RUNSETPOINT);
		display_setGauge(newECT,ID_ECTSETPOINT);
		display_setGauge(newLOP,ID_LOPSETPOINT);
		display_setGauge(newAMM,ID_AMMSETPOINT);

		firstRun = 1;
	}

	if(setPointFlag == 1 || setPointFlag == 2)
	{
		switch(display_cursorValue)
		{
		case CUR_OVSSETPOINT :
			if(setPointFlag == 1)
			{
				newOVS = (newOVS >= 3000) ? 3000 : newOVS+50;
			}
			else
			{
				newOVS = (newOVS <= 800) ? 800 : newOVS-50;
			}
			display_setGauge(newOVS,ID_OVSSETPOINT);
			break;

		case CUR_RUNSETPOINT :
			if(setPointFlag == 1)
			{
				newRUN = (newRUN >= 1500) ? 1500 : newRUN+50;
			}
			else
			{
				newRUN = (newRUN <= 200) ? 200 : newRUN-50;
			}
			display_setGauge(newRUN,ID_RUNSETPOINT);
			break;

		case CUR_ECTSETPOINT :
			if(setPointFlag == 1)
			{
				newECT = (newECT >= 120) ? 120 : newECT+1;
			}
			else
			{
				newECT = (newECT <= 25) ? 25 : newECT-1;
			}
			display_setGauge(newECT,ID_ECTSETPOINT);
			break;

		case CUR_LOPSETPOINT :
			if(setPointFlag == 1)
			{
				newLOP = (newLOP >= 60) ? 60 : newLOP+1;
			}
			else
			{
				newLOP = (newLOP <= 5) ? 5 : newLOP-1;
			}
			display_setGauge(newLOP,ID_LOPSETPOINT);
			break;

		case CUR_AMMSETPOINT :
			if(setPointFlag == 1)
			{
				newAMM = (newAMM >= 50) ? 50 : newAMM+1;
			}
			else
			{
				newAMM = (newAMM <= 1) ? 1 : newAMM-1;
			}
			display_setGauge(newAMM,ID_AMMSETPOINT);
			break;

		default :
			break;
		}
	}
	else if(setPointFlag == 3)
	{
		eeprom_update_word(&ovsThreshold,newOVS/10);
		eeprom_update_word(&runThreshold,newRUN/10);
		eeprom_update_word(&ectThreshold,newECT);
		eeprom_update_word(&lopThreshold,newLOP);
		eeprom_update_word(&ammThreshold,newAMM);

		setAlarmThresholdHigh("RPM",newOVS);
		setAlarmThresholdHigh("ECT",newECT);
		setAlarmThresholdLow("LOP",newLOP);
		setAlarmThresholdLow("AMM",newAMM);

		display_setGauge(1,ID_SETPOINTCHECK);
		_delay_ms(750);
		display_setForm(ID_SETTINGSFORM);
		display_setGauge(0,ID_SETPOINTCHECK);
		firstRun = 0;
	}
	setPointFlag = 0;
}

void incrementEngineSeconds()
{
    static uint8_t firstRun = 1, counter = 1;
    static uint16_t currentSeconds;
    uint16_t currentHours;
    if(firstRun == 1)
    {
        for(bufferAddress=0; bufferAddress < 1000 ; bufferAddress++)
        {
            currentSeconds = eeprom_read_word(&engineSeconds[bufferAddress]);
            if (currentSeconds != 0xFFFF)
                break;
        }
        if (currentSeconds == 0xFFFF)
        {
            bufferAddress = 0;
        }
        firstRun = 0;
    }

    if (counter == 10)
    {
        if(currentSeconds == 0xFFFF)
        {
            currentSeconds = 10;
            eeprom_update_word(&engineSeconds[bufferAddress],currentSeconds);
        }
        else
        {
            eeprom_update_word(&engineSeconds[bufferAddress],0xFFFF);

            if(currentSeconds == 3590)
            {
                currentSeconds = 0;
                currentHours = eeprom_read_word(&engineHours);
                currentHours = (currentHours == 0xFFFF) ? 1 : currentHours + 1;
                eeprom_update_word(&engineHours, currentHours);
            }
            else
            {
                currentSeconds += 10;
            }

            bufferAddress = (bufferAddress == 999) ? 0 : bufferAddress + 1;
            eeprom_update_word(&engineSeconds[bufferAddress],currentSeconds);
        }
        counter = 1;
    }
    else
    {
        counter += 1;
    }

}

uint16_t getPulleyRatio()
{
	return eeprom_read_word(&pulleyRatio);
}

uint16_t getOVSThreshold()
{
	return eeprom_read_word(&ovsThreshold);
}

uint16_t getRunThreshold()
{
	return eeprom_read_word(&runThreshold);
}

uint16_t getECTThreshold()
{
	return eeprom_read_word(&ectThreshold);
}

uint16_t getLOPThreshold()
{
	return eeprom_read_word(&lopThreshold);
}

uint16_t getAMMThreshold()
{
	return eeprom_read_word(&ammThreshold);
}

uint16_t getEngineHours()
{
    return eeprom_read_word(&engineHours);
}

uint16_t getEngineSeconds()
{
    uint16_t tmp, i;
    tmp = eeprom_read_word(&engineSeconds[bufferAddress]);

    if (tmp == 0xFFFF || tmp == 0xFF)
    {
        for(i=0; i < 1000 ; i++)
        {
            tmp = eeprom_read_word(&engineSeconds[i]);
            if (tmp != 0xFFFF && tmp != 0xFF)
                break;
        }
        if  (tmp == 0xFFFF || tmp == 0xFF)
            tmp = 0;
    }

    return tmp;
}

uint8_t isEnabledOVSAlarm()
{
	return eeprom_read_byte(&ovsEnabled);
}

uint8_t isEnabledECTAlarm()
{
	return eeprom_read_byte(&ectEnabled);
}

uint8_t isEnabledLOPAlarm()
{
	return eeprom_read_byte(&lopEnabled);
}

uint8_t isEnabledAMMAlarm()
{
	return eeprom_read_byte(&ammEnabled);
}


void factoryReset()
{
	ADC_resetDefaults();
	eeprom_update_word(&pulleyRatio,100);

	eeprom_update_word(&ovsThreshold, OVERSPEED_THRESHOLD/10);
	eeprom_update_word(&runThreshold,RUN_THRESHOLD/10);
	eeprom_update_word(&ectThreshold,ECT_THRESHOLD);
	eeprom_update_word(&lopThreshold,LOP_THRESHOLD*10);
	eeprom_update_word(&ammThreshold,AMM_THRESHOLD);

	eeprom_update_byte(&ovsEnabled, 1);
	eeprom_update_byte(&ectEnabled, 1);
	eeprom_update_byte(&lopEnabled, 1);
	eeprom_update_byte(&ammEnabled, 1);
	display_setForm(ID_RESETFORM);
	for(uint8_t i=0; i<5; i++)
	{
		display_setGauge(i,ID_RESETTIMER);
		_delay_ms(1000);
	}
	cli();
}

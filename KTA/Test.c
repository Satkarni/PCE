/*
 * Test.c
 *
 * Created: 31/08/2016 13:12:48
 *  Author: ROHANP
 */ 

#include <util/delay.h>
#include "Hardware.h"
#include "RS485.h"
#include "DisplayInterface.h"
#include "expander/inputHandler.h"
#include "expander/outputHandler.h"
#include "ADC.h"
#include "Timers.h"

void debugLedInit(void)
{
	DEBUG_REG_DDR |= ( (1<<DEBUG_LED1_LD8)|(1<<DEBUG_LED2_LD9) );
	DEBUG_REG_PORT &= ~( (1<<DEBUG_LED1_LD8)|(1<<DEBUG_LED2_LD9) );
}
void debugLed1LD8blink(uint8_t count)
{
	uint8_t loop = count<<1;
	while(loop-->0)
	{
		DEBUG_LED1_LD8_TOGGLE;
		_delay_ms(250);
	}
}
void debugLed2LD9blink(uint8_t count)
{
	uint8_t loop = count<<1;
	while(loop-->0)
	{
		DEBUG_LED2_LD9_TOGGLE;
		_delay_ms(250);
	}
}
void debugLedAltblink(uint8_t count)
{
	uint8_t loop = count<<1;
	while(loop-->0)
	{
		DEBUG_LED1_LD8_TOGGLE;
		_delay_ms(250);
		DEBUG_LED2_LD9_TOGGLE;
		_delay_ms(250);
	}
}
void debugLedSimulblink(uint8_t count)
{
	uint8_t loop = count<<1;
	while(loop-->0)
	{
		DEBUG_LED1_LD8_TOGGLE;
		DEBUG_LED2_LD9_TOGGLE;
		_delay_ms(250);
	}
}
uint8_t getdebugLed1LD8state(void)
{
	return ( DEBUG_REG_PIN & (1<<DEBUG_LED1_LD8) );
}
uint8_t getdebugLed2LD9state(void)
{
	return ( DEBUG_REG_PIN & (1<<DEBUG_LED2_LD9) );
}

void debugRelaySimulcycle(uint8_t count)
{

	while (count-->0)
	{
		RELAY1_WARNING_LAMP_ON;
		RELAY2_BUZZER_ON;
		RELAY3_LOCKOUT_ON;
		RELAY4_RUNNING_ON;
		RELAY5_SPARE_ON;
		_delay_ms(500);

		RELAY1_WARNING_LAMP_OFF;
		RELAY2_BUZZER_OFF;
		RELAY3_LOCKOUT_OFF;
		RELAY4_RUNNING_OFF;
		RELAY5_SPARE_OFF;
		_delay_ms(500);
	}
}

uint8_t debugGetDI(void)
{
	return DI_REG_PIN;
}


void testDebugLED(void)
{
	debugLedInit();

	DEBUG_LED1_LD8_ON;
	_delay_ms(500);
	DEBUG_LED1_LD8_OFF;
	_delay_ms(500);

	DEBUG_LED2_LD9_ON;
	_delay_ms(500);
	DEBUG_LED2_LD9_OFF;
	_delay_ms(500);

	DEBUG_LED1_LD8_TOGGLE;
	DEBUG_LED2_LD9_TOGGLE;
	_delay_ms(1000);

	if( getdebugLed1LD8state() )
	{
		DEBUG_LED1_LD8_OFF;
	}
	if( getdebugLed2LD9state() )
	{
		DEBUG_LED2_LD9_OFF;
	}
	_delay_ms(500);

	debugLed1LD8blink(5);
	debugLed2LD9blink(5);
	debugLedAltblink(5);
	debugLedSimulblink(5);

}
void testRelay(void)
{

	RELAY1_WARNING_LAMP_ON;
	_delay_ms(500);
	RELAY1_WARNING_LAMP_OFF;
	_delay_ms(500);

	RELAY2_BUZZER_ON;
	_delay_ms(500);
	RELAY2_BUZZER_OFF;
	_delay_ms(500);

	RELAY3_LOCKOUT_ON;
	_delay_ms(500);
	RELAY3_LOCKOUT_OFF;
	_delay_ms(500);

	RELAY4_RUNNING_ON;
	_delay_ms(500);
	RELAY4_RUNNING_OFF;
	_delay_ms(500);

	RELAY5_SPARE_ON;
	_delay_ms(500);
	RELAY5_SPARE_OFF;
	_delay_ms(500);

	debugRelaySimulcycle(3);

}
void testDI(void) 
{
	unsigned long count = 600;
	while(count-->0)
	{
		if(DI_START_KEYSW_ON)	RELAY1_WARNING_LAMP_ON;
		if(DI_START_KEYSW_OFF)	RELAY1_WARNING_LAMP_OFF;

		if(DI_STOP_KEYSW_ON)	RELAY2_BUZZER_ON;
		if(DI_STOP_KEYSW_OFF)	RELAY2_BUZZER_OFF;

		if(DI_RUN_ON)			DEBUG_LED1_LD8_ON;
		if(DI_RUN_OFF)			DEBUG_LED1_LD8_OFF;

		if(DI_IDLE_ON)			DEBUG_LED2_LD9_ON;
		if(DI_IDLE_OFF)			DEBUG_LED2_LD9_OFF;

		if(DI_SPARE_5V_1_ON)	RELAY5_SPARE_ON;
		if(DI_SPARE_5V_1_OFF)	RELAY5_SPARE_OFF;

		if(DI_SPARE_5V_2_ON)	RELAY1_WARNING_LAMP_ON;
		if(DI_SPARE_5V_2_OFF)	RELAY1_WARNING_LAMP_OFF;

		if(DI_SPARE_24V_1_ON)	RELAY3_LOCKOUT_ON;
		if(DI_SPARE_24V_1_OFF)	RELAY3_LOCKOUT_OFF;

		if(DI_SPARE_24V_2_ON)	RELAY4_RUNNING_ON;
		if(DI_SPARE_24V_2_OFF)	RELAY4_RUNNING_OFF;
		_delay_ms(100);
	}
}
void testRS485(void)
{
	RS485_init(BAUDRATE_485);
	RS485_enable(0,1);

	while(1)
	{
		RS485_writeString("Rohan");
		DEBUG_LED1_LD8_TOGGLE;
		_delay_ms(250);
	}
}


void testspiAD420(void)
{
	debugLedAltblink(3);
	_delay_ms(2000);

	while(DI_SPARE_24V_1_ON)
	{
		AD420_Calibrate();
	}

}

void testActuator()
{
	double throttle_position, throttle_current, throttle_max, throttle_min;
	double speed_position;
	double actuator_current, actuator_max;
	uint16_t command_position, actuator_position;
	int8_t error;
	uint8_t cycle;

	throttle_current = (double)ADC_read(ADC_THROTTLE_CHANNEL);
	throttle_max = (double)ADC_getThrottleMax();
	throttle_min = (double)ADC_getThrottleMin();

	throttle_position = 100 - ((100*throttle_current)/(throttle_max-throttle_min));
	speed_position = ((double)ADC_read(ADC_POT_CHANNEL)/1022)*100;
	command_position = (uint16_t)(throttle_position * speed_position/100);

	actuator_current = (double)ADC_read(ADC_DRV_CHANNEL);
	actuator_max = (double)ADC_getActuatorMax();
	actuator_position = (actuator_current/actuator_max)*100;

	error = command_position - actuator_position;
	if (error > 2)
	{
		cycle = (abs(error+16) > 100) ? 100 : abs(error+16);
		Timers_setDutyCycle(cycle);
		PORTC &= ~(1<<DRV_DIRECTION);
	}
	else if(error < -2)
	{
		cycle = (abs(error-25) > 100) ? 100 : abs(error-25);
		Timers_setDutyCycle(cycle);
		PORTC |= (1<<DRV_DIRECTION);
	}
	else
		Timers_setDutyCycle(0);
}


void testMain(void)
{
	static uint8_t first_run = 1;
	if(first_run == 1)
	{
		first_run = 0;
		debugLedInit();
	}
	//testActuator();
	//testDebugLED();
	//debugLed2LD9blink(3);
	//testRS485();
	//testRelay();
	//testDI();
	//testspiAD420();
}

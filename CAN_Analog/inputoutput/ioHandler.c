/*
 * ioHandler.c
 *
 *  Created on: 13-Aug-2016
 *      Author: piyush
 */

#include "ioHandler.h"

void ioHandler_initKeypad()
{
	DEBUG_REG_DDR |= ((1<<DEBUG_LED1)|(1<<DEBUG_LED2)|(1<<DEBUG_LED_POWER));
	RELAY_REG_DDR |= ((1<<RELAY1)|(1<<RELAY2));
	KEYPAD_B_REG_DDR |= (1<<DEBUG_LED_FAULT);

	KEYPAD_A_REG_DDR &= ~((1<<SW_ARROW_LEFT)|(1<<SW_ARROW_UP)|(1<<SW_BRIGHTNESS_MINUS)|(1<<SW_ENTER));
	KEYPAD_B_REG_DDR &= ~((1<<SW_BRIGHTNESS_PLUS)|(1<<SW_ARROW_DOWN)|(1<<SW_ARROW_RIGHT)|(1<<SW_BACK)|(1<<SW_MUTE));
}

void ioHandler_enablePowerLED()
{
	DEBUG_REG_PORT |= (1<<DEBUG_LED_POWER);
}

void ioHandler_disablePowerLED()
{
	DEBUG_REG_PORT &= ~(1<<DEBUG_LED_POWER);
}

void ioHandler_togglePowerLED()
{
	DEBUG_REG_PORT ^= (1<<DEBUG_LED_POWER);
}

void ioHandler_enableFaultLED()
{
	KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
}

void ioHandler_disableFaultLED()
{
	KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
}

void ioHandler_toggleFaultLED()
{
	KEYPAD_B_REG_PORT ^= (1<<DEBUG_LED_FAULT);
}

void ioHandler_blinkFaultLED(uint8_t count)
{
	uint8_t i;
	i = count;
	while(i-- > 0)
	{
		ioHandler_enableFaultLED();
		_delay_ms(200);
		ioHandler_disableFaultLED();
		_delay_ms(200);
	}
}

void ioHandler_enableRelayStop()
{
#if defined(ENERGISE_TO_STOP) && !defined(ENERGISE_TO_RUN)
	RELAY_REG_PORT |= (1<<RELAY1);
#elif defined(ENERGISE_TO_RUN) && !defined(ENERGISE_TO_STOP)
	RELAY_REG_PORT &= ~(1<<RELAY1);
#else
	RELAY_REG_PORT |= (1<<RELAY1);
#endif
}

void ioHandler_disableRelayStop()
{
#if defined(ENERGISE_TO_STOP) && !defined(ENERGISE_TO_RUN)
	RELAY_REG_PORT &= ~(1<<RELAY1);
#elif defined(ENERGISE_TO_RUN) && !defined(ENERGISE_TO_STOP)
	RELAY_REG_PORT |= (1<<RELAY1);
#else
	RELAY_REG_PORT &= ~(1<<RELAY1);
#endif
}

void ioHandler_enableRelayBuzzer()
{
	RELAY_REG_PORT |= (1<<RELAY2);
}

void ioHandler_disableRelayBuzzer()
{
	RELAY_REG_PORT &= ~(1<<RELAY2);
}


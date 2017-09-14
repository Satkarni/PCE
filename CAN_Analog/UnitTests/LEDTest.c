/*
 * LEDTest.c
 *
 *  Created on: 14-Mar-2017
 *      Author: rohan
 */

#include "Test.h"

void simulBlink(uint8_t count)
{
	while (count-->0)
	{
		DEBUG_REG_PORT |= (1<<DEBUG_LED1);
		DEBUG_REG_PORT |= (1<<DEBUG_LED2);
		DEBUG_REG_PORT |= (1<<DEBUG_LED_POWER);
		KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
		_delay_ms(200);
		DEBUG_REG_PORT &= ~(1<<DEBUG_LED1);
		DEBUG_REG_PORT &= ~(1<<DEBUG_LED2);
		DEBUG_REG_PORT &= ~(1<<DEBUG_LED_POWER);
		KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
		_delay_ms(200);
	}
}

void altBlink(uint8_t count)
{
	while (count-->0)
	{
		DEBUG_REG_PORT |= (1<<DEBUG_LED1);
		DEBUG_REG_PORT &= ~(1<<DEBUG_LED2);
		DEBUG_REG_PORT |= (1<<DEBUG_LED_POWER);
		KEYPAD_B_REG_PORT &= ~(1<<DEBUG_LED_FAULT);
		_delay_ms(200);
		DEBUG_REG_PORT &= ~(1<<DEBUG_LED1);
		DEBUG_REG_PORT |= (1<<DEBUG_LED2);
		DEBUG_REG_PORT &= ~(1<<DEBUG_LED_POWER);
		KEYPAD_B_REG_PORT |= (1<<DEBUG_LED_FAULT);
		_delay_ms(200);
	}
}

void LEDTestSetup()
{
	simulBlink(5);
	altBlink(5);
}

void LEDTestSetdown()
{

}

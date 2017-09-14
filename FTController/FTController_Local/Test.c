/*
 * Test.c
 *
 *  Created on: 13-Jan-2017
 *      Author: piyush
 */
#include "Test.h"
/*! \fn void selfTest(void)
    \brief Display test routine

    To enable this please uncomment TEST_MODE in Local_controller.c
    When enabled program will enter an endless testing loop.
    This will display 8888 on all the displays and enable all LEDs.
    After that it will enable and disable all relays and debug LEDs on board.

 */
void selfTest(void)
{
    uint16_t i=0;
    DDRA |= (1<<DEBUG_LED1)|(1<<DEBUG_LED2);

    i=8888;
    MAX7219_display7segment(i,RPM);
    MAX7219_display7segment(i,WATER_TEMP);
    MAX7219_display7segment(i,LUBE_OIL_PRESSURE);
    MAX7219_display7segment(i,LUBE_OIL_TEMP);
    MAX7219_display7segment(i,BATTERY_VGE);
    MAX7219_display7segment(i,RUN_TIME);
    MAX7219_displayLED(0xFFFF);

    for (i=0; i < 3; i++)
    {
        PORTA |= (1<<START_OUTPUT);
        PORTA |= (1<<STOP_OUTPUT);
        PORTA |= (1<<SPARE_OUTPUT);
        PORTD |= (1<<LOCKOUT_RELAY_OUTPUT);

        PORTA |= (1<<DEBUG_LED1);
        PORTA |= (1<<DEBUG_LED2);

        _delay_ms(500);

        PORTA &= ~(1<<START_OUTPUT);
        PORTA &= ~(1<<STOP_OUTPUT);
        PORTA &= ~(1<<SPARE_OUTPUT);
        PORTD &= ~(1<<LOCKOUT_RELAY_OUTPUT);

        PORTA &= ~(1<<DEBUG_LED1);
        PORTA &= ~(1<<DEBUG_LED2);
    }

    MAX7219_clearDisplay(CHIP0);
    MAX7219_clearDisplay(CHIP1);
    MAX7219_clearDisplay(CHIP2);
    MAX7219_clearDisplay(CHIP3);
    MAX7219_displayLED(0x0000);
    _delay_ms(1000);
}

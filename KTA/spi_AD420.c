/*
 * spi_AD420.c
 *
 * Created: 09/09/2016 18:01:19
 *  Author: ROHANP
 */ 


#include "spi.h"
#include "Hardware.h"

void AD420_Setcurrent(uint16_t current)
{	
	AD420_CS_LOW;
	spi_tranceive(current>>8);
	spi_tranceive(current);
	AD420_CS_HIGH;
}

void AD420_Calibrate(void)
{
	uint16_t current=0x8000;
	AD420_Setcurrent(current);
	do
	{
		current = 0;
		AD420_Setcurrent(current);
		DEBUG_LED1_LD8_TOGGLE;
		_delay_ms(100);
	}while(DI_SPARE_5V_1_ON);
	
	
	do
	{
		current = 0xFFFF;
		AD420_Setcurrent(current);
		DEBUG_LED2_LD9_TOGGLE;
		_delay_ms(200);
	}while(DI_SPARE_5V_2_ON);

}


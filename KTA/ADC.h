/*
 * ADC.h
 *
 * Created: 08-08-2016 14:01:12
 *  Author: ADMIN
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/eeprom.h>

#define ADC_POT_CHANNEL	( 0 )
#define ADC_THROTTLE_CHANNEL	( 1 )
#define ADC_DRV_CHANNEL	( 2 )
#define ADC_LOP_CHANNEL	( 3 )
#define ADC_ECT_CHANNEL	( 4 )
#define ADC_CS_CHANNEL		( 5 )
#define ADC_SR_CHANNEL		( 6 )
#define ADC_420_CHANNEL	( 7 )
#define ADC_COOLANT_LEVEL_LOW		( 500 )	
#define FILTER_ALPHA 0.5


void ADC_init();
void ADC_enable();
void ADC_disable();
uint16_t ADC_read(uint8_t channel);
uint16_t ADC_filterRead(uint8_t channel);
void ADC_updateThrottleMin ();
void ADC_updateThrottleMax ();
void ADC_updateActuatorMax ();
uint16_t ADC_getThrottleMin();
uint16_t ADC_getThrottleMax();
uint16_t ADC_getActuatorMax();
void ADC_resetDefaults(void);





#endif /* ADC_H_ */

/*
 * ADC.h
 *
 * Created: 30-04-2015 14:01:12
 *  Author: ADMIN
 */ 

/**
*	@file ADC.h
*	@author Rohan
*	@brief ABSTRACT: Handles hardware ADC
*/


#ifndef ADC_H_
#define ADC_H_

/**
 *	\defgroup adc ADC Functions [ABSTRACT]
 *	@{
 */
void ADC_init();
void ADC_enable();
void ADC_disable();
uint16_t ADC_read(uint8_t channel);

///@}



#endif /* ADC_H_ */

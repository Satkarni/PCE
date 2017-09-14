/*
 * ADC.h
 *
 * Created: 30-04-2015 14:01:12
 *  Author: ADMIN
 */ 

/**
*	@file ADC.h
*	@author Rohan, Piyush
*	@brief ABSTRACT: Handles hardware ADC
*/

#define FILTER_ALPHA	( 0.5 )							///<Alpha factor for EMA (IIR) filter used in ADC_filterRead.

#define GEARBOX_TEMP_CHANNEL				( 0 )
#define GEARBOX_PRESSURE_CHANNEL		( 1 )
#define EXHAUST_TEMP_CHANNEL				( 2 )
#define SPARE_TEMP_CHANNEL					( 3 )


#ifndef ADC_H_
#define ADC_H_

/**
 *	\defgroup adc ADC Functions [ABSTRACT]
 *	@{
 */

/**
 * @brief Initialises the ADC
 */
void ADC_init();

/**
 * @brief Enables the ADC
 */
void ADC_enable();

/**
 * @brief Disables the ADC
 */
void ADC_disable();

/**
 * @brief Reads value from ADC
 *
 * Read value from ADC at the specified channel.
 *
 * @param Channel from which to get reading.
 *
 * @return Value received from ADC.
 */
uint16_t ADC_read(uint8_t channel);

/**
 * @brief Reads EMA filtered value from ADC.
 *
 * Read value from the ADC at the specified channel.
 * This function will apply an exponential moving
 * average filter to the value before returning it. The
 * alpha factor of the EMA filter is taken from the macro
 * FILTER_ALPHA.
 *
 * @param Channel from which to get reading.
 *
 * @return Filtered value received from ADC.
 */
uint16_t ADC_filterRead(uint8_t channel);

///@}



#endif /* ADC_H_ */

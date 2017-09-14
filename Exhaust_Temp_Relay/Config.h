/*
 * Config.h
 *
 * Created: 16/12/2016 10:04:53
 *  Author: ROHANP
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#define IN1_TEMP_SETPOINT	( 575 )
#define IN2_TEMP_SETPOINT	( 575 )
#define TEMP_HYS			( 50 )

#define TEMP_MIN			( 0 )
#define TEMP_MAX			( 800 )

#define VGE_mV_TEMP_MIN			( 960 )
#define VGE_mV_TEMP_MAX			( 4800 )
#define VGE_mV_TEMP_SETPOINT	( 3720 ) 
#define VGE_mV_TEMP_HYS			( 3480 )

#define ADC_TEMP1_MIN		( 196 )
#define ADC_TEMP1_MAX		( 983 )
#define ADC_TEMP1_SETPOINT	( 762 )
#define ADC_TEMP1_HYS		( 713 )

#define ADC_TEMP2_MIN		( 196 )
#define ADC_TEMP2_MAX		( 983 )
#define ADC_TEMP2_SETPOINT	( 762 )
#define ADC_TEMP2_HYS		( 713 )

#define TEMP1_TRIP_DELAY		( 5 )
#define TEMP2_TRIP_DELAY		( 5 )

#endif /* CONFIG_H_ */
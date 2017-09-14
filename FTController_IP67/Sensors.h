/*
 * Sensors.h
 *
 * Created: 20-08-2015 15:36:28
 *  Author: ADMIN
 *
 * This header file contains sensor parameters. Modify this file to change alarm, trip values. 
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

#define SAMPLE 5
#define SENSOR_DELAY	6		// 3 sec delay for temp sensors

#define ADC_REF 5
#define ADC_MAX_VAL 1024
#define LUBE_OIL_PRESSURE_ADC_CHANNEL 0
#define LUBE_OIL_TEMP_ADC_CHANNEL	  1
#define WATER_TEMP_ADC_CHANNEL		  2
#define BATTERY_VGE_ADC_CHANNEL		  3

#define LOP_MAX_ADC 922
#define LOP_MIN_ADC 102
#define LOP_SLOPE_ADC  0.122		//100/(LOP_MAX_ADC - LOP_MIN_ADC)
#define LOP_TRIP_ADC  225
#define LOP_ALARM_ADC 307
#define LOP_TOL_ADC   30
#define LOP_STARTER_ENGAGE 140

#define PT100_CURRENT   1
#define PT100_GAIN	    21
#define PT100_ADC_SCALE 0.385
#define PT100_CURRENT_MA 1000
#define PT100_RES_AT_0_DEGREE 100

#define LOT_MAX_ADC 492
#define LOT_MIN_ADC 307
//#define LOT_TRIP_ADC  400
#define LOT_ALARM_ADC 445
#define LOT_TOL_ADC   5
#define LOT_WIRE_RES  30

#define WT_MAX_ADC 492
#define WT_MIN_ADC 307
//#define WT_TRIP_ADC  400
#define WT_ALARM_ADC 411
#define WT_TOL_ADC   5
#define WT_WIRE_RES	 4

#define NUMBER_OF_TEETH 0.882		// 136 - rpm = (number of teeth)*freq/120
#define RPM_MIN 10
#define RPM_TRIP  2500
#define RPM_ALARM 1900
#define RPM_TOL  10
#define RPM_IDLE 650
#define RPM_LOCKOUT_ENABLE	100
#define RPM_LOCKOUT_DISABLE 500

#define BV_MAX_TO_DISPLAY 158
#define BV_MAX_ADC 50
#define BV_MIN_ADC 655
#define BV_SLOPE_ADC  0.215 	//(BATTERY_VGE_MAX_TO_DISPLAY - BATTERY_VGE_MIN_TO_DISPLAY)/(BATTERY_VGE_MAX_ADC - BATTERY_VGE_MIN_ADC)
//#define BV_TRIP_ADC  225
#define BV_ALARM_ADC 100
#define BV_TOL_ADC   10
#define BV_SENSOR_FAIL_ADC 820


#endif /* SENSORS_H_ */
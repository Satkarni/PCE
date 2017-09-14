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

/*!
  \def SAMPLE

  Only to be used when simple moving average is used for ADC readings.
  This will set the number of samples for calculating the moving average.
  Please note that update rate will reduce as you increase this parameter,
  hence set a reasonable value for this parameter.
*/
#define SAMPLE			30

/*!
  \def SENSOR_DELAY

  This parameter determines the frequency at which sensor readings are taken.
  The base rate is a sensor reading every 500ms.\n
  If you wish to take a reading every y seconds, please set value to 2y.\n
  E.g. For 2 second delay between sensor readings\n
  #define SENSOR_DELAY 4
*/
#define SENSOR_DELAY	6

#define ADC_REF 5
#define ADC_MAX_VAL 1024
#define LUBE_OIL_PRESSURE_ADC_CHANNEL 0
#define LUBE_OIL_TEMP_ADC_CHANNEL	  1
#define WATER_TEMP_ADC_CHANNEL		  2
#define BATTERY_VGE_ADC_CHANNEL	  3

/*!
  \def FILTER_ALPHA

  Only to be used when exponential moving average is used for ADC readings.
  This sets the weight of past readings relative to the current reading. \n
  Value can range from 0 - 1. A value close to 0 means that the output will
  change very slowly as past readings are weighted more, whereas value close to
  1 will reduce filtering drastically.\n
*/
#define FILTER_ALPHA    0.5

/*!
  \defgroup LOP_Sensor
  \brief Setpoints for LOP sensor \n

  For measuring Lube oil pressure(LOP) we use a Danfoss sensor with voltage
  output. \n
  Sensor input: 0 to 10 bar \n
  Sensor output: 0.5 to 4.5V \n

  0 bar - 0.5V - 102 ADC \n
  5 bar - 2.5V - 512 ADC \n
  10 bar - 4.5V - 922 ADC \n

  To calculate ADC values for alarm and trip: \n
  ADC = (pressure*10/(LOP_SLOPE_ADC)) + LOP_MIN_ADC \n
  ADC = (pressure*10/(0.122)) + 102 \n\n
  ADC = pressure*81.967 + 102 \n\n

  To set alarm at 2.5 bar \n
  LOP_ALARM_ADC = 2.5*81.967 + 102 = 306.91 ~ 307 \n

  To set trip at 1.5 bar \n
  LOP_TRIP_ADC = 1.5*81.967 + 102 = 224.95 ~ 225 \n

  @{
*/
#define LOP_MAX_ADC 922
#define LOP_MIN_ADC 102
#define LOP_SLOPE_ADC  0.122		//100/(LOP_MAX_ADC - LOP_MIN_ADC)
#define LOP_TRIP_ADC  225           //SL90/DV12
//#define LOP_ALARM_ADC 266        //SL90
#define LOP_ALARM_ADC 307         //DV12
#define LOP_TOL_ADC   30
#define LOP_LOCKOUT_ENABLE	143
/*!
  @}
*/

/*!
  \defgroup PT100_SENSOR
  \brief Setpoints for all parameters which use RTD(PT100) sensor

  For measuring water temperature and lube oil temperature we use a PT100
  sensor. Constant 15 mA current is passed through the sensor and voltage
  across the sensor is measured. As resistance varies with temperature voltage
  across the sensor varies. \n\n

  Sensor input: 0 to 150 degC \n
  Sensor resistance: 100 to 157.32 ohm \n
  Sensor output: 1.5V(1500 mV) to 2.3598V(2359.8 mV) \n\n

  0 degC  - 100 ohm  -  1500 mV  -  307 ADC \n
  75 degC - 128.98 ohm - 1934.7 mV - 396 ADC \n
  150 degC - 157.32 ohm - 2359.8 mV - 484 ADC \n\n

  To calculate ADC values for alarm and trip: \n
  tempr = resistance of PT100 at required temperature from the PT100
  table \n

  ADC = (tempr*PT100_CURRENT*1024/5000) \n
  ADC = (tempr*15*1024/5000) \n\n
  ADC = tempr*3.072 \n\n

  To set alarm at 97 degC \n
  tempr = 137.3619 ohm at 97 degC from PT100 table \n
  LOT_ALARM_ADC = 137.36*3.072 = 421.96 ~ 422 \n\n

  To set trip at 105 degC \n
  tempr = 140.39 ohm at 105 degC from PT100 table \n
  LOT_TRIP_ADC = 140.39*3.072 = 431.27 ~ 431 \n
  @{
*/

#define PT100_CURRENT 15
#define PT100_ADC_SCALE 0.385
#define PT100_CURRENT_MA 1000
#define PT100_RES_AT_0_DEGREE 100

#define LOT_MAX_ADC 484
#define LOT_MIN_ADC 299
#define LOT_TRIP_ADC  461           //SL90/DV12
//#define LOT_ALARM_ADC 437         //SL90
#define LOT_ALARM_ADC 443         //DV12
#define LOT_TOL_ADC   5
#define LOT_WIRE_RES  4

#define WT_MAX_ADC 484
#define WT_MIN_ADC 299
//#define WT_TRIP_ADC  421        //SL90
//#define WT_ALARM_ADC 416     //SL90
#define WT_TRIP_ADC  430        //DV12
#define WT_ALARM_ADC 425     //DV12
#define WT_TOL_ADC   5
#define WT_WIRE_RES	 4
/*!
  @}
*/

/*!
  \def NUMBER_OF_TEETH

  NUMBER_OF_TEETH = 120/(number of teeth)

  For e.g. \n
  To set number of teeth to 150 \n
  NUMBER_OF_TEETH = 120/150 = 0.8 \n\n

  To set number of teeth to 142 \n
  NUMBER_OF_TEETH = 120/142 = 0.845 \n
*/
#define NUMBER_OF_TEETH 0.612		//DV12 // 196 - rpm = (number of teeth)*freq/120
//#define NUMBER_OF_TEETH 0.8		//SL90 // 150 - rpm = (number of teeth)*freq/120
#define RPM_MIN 10
#define RPM_TRIP  1980              //SL90/DV12
#define RPM_ALARM 1980          //SL90/DV12
#define RPM_TOL  10
#define RPM_LOCKOUT_ENABLE	100
#define RPM_LOCKOUT_DISABLE 500

#define BV_MAX_TO_DISPLAY 0
#define BV_MAX_ADC 1024
#define BV_MIN_ADC 0
#define BV_SLOPE_ADC  0.3906
//#define BV_TRIP_ADC  225
#define BV_ALARM_ADC 461
#define BV_TOL_ADC   10
#define BV_SENSOR_FAIL_ADC 1024


#endif /* SENSORS_H_ */

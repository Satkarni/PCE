

#ifndef GOA_POLICE_BOAT_H
#define GOA_POLICE_BOAT_H


#define F_CPU 16000000UL
#define REVISION "1.0"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Hardware.h"
#include "Config.h"
#include "ADC.h"

#include "can_bus_lib/can.h"
#include "can_bus_lib/at90can_private.h"
#include "J1939.h"

#include "Display.h"
#include "ks0108.h"

typedef struct
{
	unsigned int buzzer: 1;
	unsigned int trip  : 1;
	unsigned int tripC1 : 1;
	unsigned int tripC2 : 1;
	unsigned int en		: 1;
	timer tmr;
}Sensor;

Sensor lop_sensor;
Sensor ect_sensor;



/*	Reserved for future use 


#define POLLING_RATE_IN_10MS	50							//50*10 = 500 ms
#define POLLING_RATE			POLLING_RATE_IN_10MS*144

#define LED_HEARTBEAT	PB0

// Analog inputs
#define LUBE_OIL_PRESSURE		0
#define SEA_WATER_PRESSURE		1
#define WATER_TEMPERATURE		2
#define LUBE_OIL_TEMPERATURE	3
#define EXHAUST_TEMPERATURE		4			//fixed, cannot be interchanged
#define GEARBOX_OIL_PRESSURE	5			//fixed, cannot be interchanged

// Digital inputs
#define RESET_BUTTON	PB2
#define ACK_BUTTON		PB3
#define ENTER_BUTTON	PB4
#define FUEL_LEAK		PB5

#define MPU				PB1

// Sensor range ( all assume constant current from LM317 is 15mA )
#define LUBE_OIL_PRESSURE_TOL_MIN		10			// ~0.04 V tolerance when pressure is 0
#define SEA_WATER_PRESSURE_TOL_MIN		10
#define WATER_TEMPERATURE_TOL_MIN		10
#define LUBE_OIL_TEMPERATURE_TOL_MIN    10
#define GEARBOX_OIL_PRESSURE_TOL_MIN	10

#define PRESSURE_SCALE1					5.736		// ADC = 30(1+AP) ... A = 0.1743 .... pressure_scale = 1/A
#define PRESSURE_SCALE2					0.1912
#define LUBE_OIL_PRESSURE_MIN			30			// 10-180 ohm 0 -> 0.15 - 2.70 V -> 30 - 553
#define LUBE_OIL_PRESSURE_MAX			553
#define SEA_WATER_PRESSURE_MIN			30			// 10-180 ohm 0 -> 0.15 - 2.70 V -> 30 - 553
#define SEA_WATER_PRESSURE_MAX			553


#define TEMPERATURE_SCALE1				255.86		// R = Ro(1+BT)  ... B = 3.9038e-3...  temperature_scale = 1/B
#define TEMPERATURE_SCALE2				0.833
#define WATER_TEMPERATURE_MIN			307			// 100-180 ohm  -> 1.50 - 2.70 V -> 307 - 553
#define WATER_TEMPERATURE_MAX			553
#define LUBE_OIL_TEMPERATURE_MIN		307			// 100-180 ohm  -> 1.50 - 2.70 V -> 307 - 553
#define LUBE_OIL_TEMPERATURE_MAX		553

#define GEARBOX_OIL_PRESSURE_SCALE1  	10.024		// ADC = 205(1+BP)....C = 0.0998 ...
#define GEARBOX_OIL_PRESSURE_SCALE2		0.0488	
#define GEARBOX_OIL_PRESSURE_MIN		205			// 4-20 mA -> 1.00 - 5.00 V -> 205 - 1023
#define GEARBOX_OIL_PRESSURE_MAX		1023

#define EXHAUST_TEMPERATURE_MIN			0			// 0.00 - 4.80 V -> 0 - 1000 
#define EXHAUST_TEMPERATURE_MAX			1000		// ADC value is equal to real temperature 
#define EXHAUST_TEMP_SENSOR_FAIL		1001		// 5V when thermocouple is open

#define RPM_MIN							0
#define RPM_MAX							9999
#define RPM_PRESSURE_SAFETY_EN			250


typedef enum{
	
	INACTIVE,
	PREACTIVE,
	RUNNING,
	OVERSPEED
	
}Engine_State;
*/
#endif 
/*
 * EEPROM.h
 *
 *  Created on: 26-Aug-2016
 *      Author: piyush
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/eeprom.h>

#define		EEP_OVS_ALARM			( 2 )
#define		EEP_OVS_TRIP				( 6 )
#define		EEP_OVS_TRIP_DELAY		( 10 )
#define		EEP_OVS_ALARM_DELAY	( 12 )

#define		EEP_LOP_ALARM				( 14 )
#define		EEP_LOP_TRIP				( 20 )
#define		EEP_LOP_TRIP_DELAY		( 24 )
#define		EEP_LOP_ALARM_DELAY	( 26 )

#define		EEP_LOT_ALARM				( 28 )
#define		EEP_LOT_TRIP				( 32 )
#define		EEP_LOT_TRIP_DELAY		( 36 )
#define		EEP_LOT_ALARM_DELAY	( 38 )

#define		EEP_ECT_ALARM				( 40 )
#define		EEP_ECT_TRIP				( 44 )
#define		EEP_ECT_TRIP_DELAY		( 48 )
#define		EEP_ECT_ALARM_DELAY	( 50 )



#endif /* EEPROM_H_ */

/*
 * CANHandler.h
 *
 *  Created on: 17-Aug-2016
 *      Author: piyush
 */

/**
 * @file CANHandler.h
 * @author Piyush
 * @brief Contains logic for handling CAN based sensors
 **/
#ifndef CAN_SCANNER_CANHANDLER_H_
#define CAN_SCANNER_CANHANDLER_H_

#include "../can_bus_lib/can.h"
#include "../can_bus_lib/at90can_private.h"
#include "J1939.h"
#include "../display/Display.h"
#include "../display/DTCPage.h"
#include "../EEPROM.h"

#include "../sensor_lib/SensorsDAO.h"
#include "../sensor_lib/SensorHandler.h"
#include "../sensor_lib/TimerQueue.h"
#include "../SetPoints.h"



typedef struct
{
	uint8_t bam_received : 1;
	uint8_t control;
	uint16_t message_size;
	uint8_t npackets;
	uint16_t PGN;
}tpcm;

typedef struct
{
	uint8_t buffer[700];
	uint8_t count;
	uint8_t process_buffer : 1;
}DataPacket;

DataPacket DT;

tpcm BAM;

void CANHandler_init();
void CANHandler_initSensors();
void CANHandler_updateReading(char* sensor_name, uint32_t value);
void CANHandler_setFilter(uint8_t mob_num,uint32_t pgn,uint32_t pgn_mask);
void CANHandler_scan();
void CANHandler_sendACK(uint32_t PGN, uint8_t control_byte);

#endif /* CAN_SCANNER_CANHANDLER_H_ */

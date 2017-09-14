/*
 * Config.h
 *
 * Created: 01/08/2016 15:26:29
 *  Author: ROHANP
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#define	Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define	Leave_Critical_Section		SREG = sreg_; } while (0);

#define FREQ_MIN		( 60 )
#define FREQ_MAX		( 10000 )
#define RPM_MIN			( 0 )
#define RPM_SECONDS		( 60.0 )
#define RPM_RESET		( 60 )

#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)    // set baudrate value for UBRR

#define RCV_BUFFER_MAX		( 16 )
#define FRAME_NORMAL		( 16 )
#define FRAME_CURRENT_RPM	( 5 )

#define FRAME_WRITE			( 1 )
#define FRAME_READ			( 2 )
#define FRAME_TEST			( 3 )
#define FRAME_LOG			( 4 )
#define FRAME_CURRENTRPM	( 5 )
#define FRAME_ACK			( 6 )

#define EEP_MAX_PACKET		( 63 )
#define EEP_MAX_LOG_PACKET	( 5 )

#define EEPADDR_PACKETCNT		(0x00)
#define EEPADDR_RATEDSPEED		(0x01)
#define EEPADDR_NUMBEROFTEETH	(0x03)
#define EEPADDR_OVERSPEEDPCT	(0x06)
#define EEPADDR_TRIPDELAY		(0x08)
#define EEPADDR_DATEYEAR		(0x0A)
#define EEPADDR_DATEMONTH		(0x0B)
#define EEPADDR_DATEDAY			(0x0C)
#define EEPADDR_DATEHOURS		(0x0D)
#define EEPADDR_DATEMINUTES		(0x0E)

 typedef struct settings_t
 {
	 uint16_t ratedSpeed;
	 uint16_t rpmOverspeed;
	 uint16_t numberofTeeth;
	 uint8_t overspeedPct;
	 uint8_t tripDelay;
 }settings_t;
 settings_t currentSettings;
 volatile uint16_t RPM;
 
void USART_enable(uint8_t rxenable,uint8_t txenable);
void USART_init(uint16_t ubrr_value);
uint8_t USART_readchar();
void USART_writechar(uint8_t data);
void USART_writeString(char *data);

//Getters
uint8_t getFunction(uint8_t offset);
uint16_t getRatedspeed(uint8_t offset);
uint16_t getCurrentspeed(void);
uint8_t getNumberofteeth(uint8_t offset);
uint8_t getOverspeedpct(uint8_t offset);
uint8_t getTripdelay(uint8_t offset);
uint8_t getDate(uint8_t offset);
uint16_t getCRC(uint8_t *crc_p, uint8_t length);
uint8_t getPacketCnt(void);

//Setters
void setFunction(uint8_t *buffer,uint8_t offset);
void setRatedspeed(uint8_t *buffer,uint8_t offset);
void setCurrentspeed(uint16_t rpm);
void setNumberofteeth(uint8_t *buffer,uint8_t offset);
void setOverspeedpct(uint8_t *buffer,uint8_t offset);
void setTripdelay(uint8_t *buffer,uint8_t offset);
void setDate(uint8_t *buffer,uint8_t offset);
void setPacketCnt(uint8_t packetCnt);
void setcurrentSettings(settings_t *settings);

uint8_t processCRC(uint8_t *buffer,uint8_t length);
void processSendCurrentRPM(void);
void config_main(void);

#endif /* CONFIG_H_ */
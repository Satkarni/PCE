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

#define FREQ_MIN		( 0 )
#define FREQ_MAX		( 10000 )
#define RPM_MIN			( 0 )
#define RPM_SECONDS		( 60.0 )
#define RPM_RESET		( 60 )

#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)    // set baudrate value for UBRR

#define RCV_BUFFER_MAX		( 24 )
#define FRAME_NORMAL		( 24 )

#define FRAME_WRITE			( 0 )
#define FRAME_READ			( 1 )
#define FRAME_ACK			( 6 )

#define EEP_MAX_PACKET		( 63 )
#define EEP_MAX_LOG_PACKET	( 5 )

#define EEPADDR_PACKETCNT		( 0x00 )
#define EEPADDR_NUMBEROFTEETH	( 0x01 )
#define EEPADDR_MINRPM			( 0x03 )
#define EEPADDR_MAXRPM			( 0x05 )
#define EEPADDR_OUTPUTTYPE		( 0x07 )
#define EEPADDR_CURRENTRANGE	( 0x08 )
#define EEPADDR_VOLTAGERANGE	( 0x09 )

#define EEPADDR_RLY1_EN			( 0x0A )
#define EEPADDR_RLY1_RST		( 0x0B )
#define EEPADDR_RLY1_ONRPM		( 0x0C )
#define EEPADDR_RLY1_OFFRPM		( 0x0E )
#define EEPADDR_RLY1_DELAY		( 0x10 )

#define EEPADDR_RLY2_EN			( 0x11 )
#define EEPADDR_RLY2_RST		( 0x12 )
#define EEPADDR_RLY2_ONRPM		( 0x13 )
#define EEPADDR_RLY2_OFFRPM		( 0x15 )
#define EEPADDR_RLY2_DELAY		( 0x17 )

 volatile uint16_t RPM;
 
 typedef struct settings_t
 {
	 uint16_t numberofTeeth;
	 uint16_t minRpm;
	 uint16_t maxRpm;
	 uint8_t outputType;
	 uint8_t currentRange;
	 uint8_t voltageRange;
	 
	 uint8_t rly1Enable;
	 uint8_t rly1Resettype;
	 uint16_t rly1onRpm;
	 uint16_t rly1offRpm;
	 uint8_t rly1Resetdelay;
	 
	 uint8_t rly2Enable;
	 uint8_t rly2Resettype;
	 uint16_t rly2onRpm;
	 uint16_t rly2offRpm;
	 uint8_t rly2Resetdelay;
	 
 }settings_t;
 settings_t currentSettings;
 
void USART_enable(uint8_t rxenable,uint8_t txenable);
void USART_init(uint16_t ubrr_value);
uint8_t USART_readchar();
void USART_writechar(uint8_t data);
void USART_writeString(char *data);

//Getters
uint8_t getPacketCnt(void);
uint16_t getCRC(uint8_t *crc_p, uint8_t length);
uint8_t getFunction(uint8_t offset);
uint16_t getNumberofteeth(uint8_t offset);
uint16_t getMinRPM(uint8_t offset);
uint16_t getMaxRPM(uint8_t offset);
uint8_t getOutputType(uint8_t offset);
uint8_t getCurrentRange(uint8_t offset);
uint8_t getVoltageRange(uint8_t offset);

uint8_t getRly1Enable(uint8_t offset);
uint8_t getRly1Resettype(uint8_t offset);
uint16_t getRly1OnRPM(uint8_t offset);
uint16_t getRly1OffRPM(uint8_t offset);
uint8_t getRly1ResetDelay(uint8_t offset);

uint8_t getRly2Enable(uint8_t offset);
uint8_t getRly2Resettype(uint8_t offset);
uint16_t getRly2OnRPM(uint8_t offset);
uint16_t getRly2OffRPM(uint8_t offset);
uint8_t getRly2ResetDelay(uint8_t offset);

//Setters
void setPacketCnt(uint8_t packetCnt);
void setFunction(uint8_t *buffer,uint8_t offset);
void setNumberofteeth(uint8_t *buffer,uint8_t offset);
void setMinRPM(uint8_t *buffer,uint8_t offset);
void setMaxRPM(uint8_t *buffer,uint8_t offset);
void setOutputType(uint8_t *buffer,uint8_t offset);
void setCurrentRange(uint8_t *buffer,uint8_t offset);
void setVoltageRange(uint8_t *buffer,uint8_t offset);

void setRly1Enable(uint8_t *buffer,uint8_t offset);
void setRly1Resettype(uint8_t *buffer,uint8_t offset);
void setRly1OnRPM(uint8_t *buffer,uint8_t offset);
void setRly1OffRPM(uint8_t *buffer,uint8_t offset);
void setRly1ResetDelay(uint8_t *buffer,uint8_t offset);

void setRly2Enable(uint8_t *buffer,uint8_t offset);
void setRly2Resettype(uint8_t *buffer,uint8_t offset);
void setRly2OnRPM(uint8_t *buffer,uint8_t offset);
void setRly2OffRPM(uint8_t *buffer,uint8_t offset);
void setRly2ResetDelay(uint8_t *buffer,uint8_t offset);

void setcurrentSettings(settings_t *settings);
uint8_t processCRC(uint8_t *buffer,uint8_t length);

void config_main(void);

#endif /* CONFIG_H_ */
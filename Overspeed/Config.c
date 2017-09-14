/*
 * Config.c
 *
 * Created: 01/08/2016 15:26:17
 *  Author: ROHANP
 */ 

#include "Config.h"

typedef enum rcvState_
{
	STATE_RX_IDLE,              /*!< Receiver is in idle state. */
	STATE_RX_RCV,               /*!< Frame is beeing received. */
	STATE_RX_ERROR				// Error
} rcvState_;

static volatile rcvState_ rcvState;
static volatile uint8_t rcvPos;

volatile uint8_t rcvBuffer[RCV_BUFFER_MAX];

ISR(USART_RX_vect)
{
	
	uint8_t temp = 0;
	switch(rcvState)
	{
		
		case STATE_RX_IDLE :
			temp = USART_readchar();
			
			if((temp == FRAME_READ) || (temp == FRAME_WRITE) || (temp == FRAME_TEST) || (temp == FRAME_LOG) || (temp == FRAME_ACK)	)	// start of new packet
			{
				rcvPos = 1;
				rcvBuffer[rcvPos-1] = temp; 
				rcvState = STATE_RX_RCV;
			}
			
		break;
		
		case STATE_RX_RCV :
			if(rcvPos < RCV_BUFFER_MAX)
			{
				rcvBuffer[rcvPos++] = USART_readchar();
				
				if(rcvPos == FRAME_NORMAL - 1) 
				{
					config_main();
				}
					
			}else
			{
				rcvState = STATE_RX_ERROR;
			}
		break;
		
		
		case STATE_RX_ERROR :
			
			for(uint8_t i=0;i<RCV_BUFFER_MAX;i++)
			{
				rcvBuffer[i] = 0;
			}
			rcvState = STATE_RX_IDLE; 
		break;
		
	}
}
void USART_enable(uint8_t rxenable,uint8_t txenable)
{
	
	if(rxenable)
	{	
		UCSR0B |= (1<<RXEN0) | (1<<RXCIE0 );	
	}
	else
	{	
		UCSR0B &= ~((1<<RXEN0) | (1<<RXCIE0 ));
	}
	
	if(txenable)
	{		
		UCSR0B |= (1<<TXEN0); 
	}
	else
	{		
		UCSR0B &= ~(1<<TXEN0 );
	}
}
void USART_init(uint16_t ubrr_value)
{
   DDRD &= ~(1<<PORTD0);	// Configure RXD as input and enable pullup
   PORTD |= (1<<PORTD0);
   UBRR0L = ubrr_value;
   UBRR0H = (ubrr_value>>8);
   
   /*Set Frame Format
   >> Asynchronous mode
   >> NO Parity
   >> 1 StopBit
   >> char size 8     */
   UCSR0C= (1<<UCSZ01)|(1<<UCSZ00);
   
   rcvState = STATE_RX_IDLE;
   USART_enable(1,1);
}
uint8_t USART_readchar()
{
	
	
	while(!(UCSR0A & (1<<RXC0))){
	}
	return UDR0;
}
void USART_writechar(uint8_t data)
{
	
	
	while(!(UCSR0A & (1<<UDRE0))) {
	}
	UDR0=data;
	
	
}
void USART_writeString(char *data)
{
	while(*data != 0)
	{
		USART_writechar(*data);
		data++;
	}
}

//Getters
uint8_t getFunction(uint8_t offset)
{
	uint8_t function = 0;
	
	return function;
}
uint16_t getRatedspeed(uint8_t offset)
{
	uint16_t ratedSpeed = 0;
	Enter_Critical_Section
		ratedSpeed = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_RATEDSPEED +1)))<<8;
		ratedSpeed |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RATEDSPEED));
	Leave_Critical_Section
	return ratedSpeed;
}
uint16_t getCurrentspeed(void)
{
	return RPM;
}
uint8_t getNumberofteeth(uint8_t offset)
{
	uint16_t numberofTeeth = 0;
	Enter_Critical_Section
		numberofTeeth = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH + 1)))<<8;
		numberofTeeth |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH));
	Leave_Critical_Section
	return numberofTeeth;
}
uint8_t getOverspeedpct(uint8_t offset)
{
	uint8_t overspeedPct = 0;
	Enter_Critical_Section
		overspeedPct |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_OVERSPEEDPCT));
	Leave_Critical_Section
	return overspeedPct;
}
uint8_t getTripdelay(uint8_t offset)
{
	uint8_t tripDelay = 0;
	Enter_Critical_Section
		tripDelay |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_TRIPDELAY));
	Leave_Critical_Section
	return tripDelay;
}
uint8_t getDate(uint8_t offset)
{
	uint8_t date = 0;
	
	return date;
}
uint16_t getCRC(uint8_t *crc_p, uint8_t length){

	uint16_t crc;
	uint8_t thisbyte,shift,lastbit,l;
	
	crc=0xffff;
	for (l=0; l<length; l++)
	{
		thisbyte= *(crc_p+l);
		crc = crc^thisbyte;
		for (shift=1; shift<=8; shift++)
		{
			lastbit = crc & 1;
			crc = (crc >> 1) & 0x7fff;
			if (lastbit==1)
			{
				crc = crc^0xA001 ;
			}
		}
	}
	return crc;
}
uint8_t getPacketCnt(void)
{
	uint8_t packetCnt = 0;
	Enter_Critical_Section
		packetCnt = eeprom_read_byte((uint8_t*)EEPADDR_PACKETCNT);
	Leave_Critical_Section
	if(packetCnt >= EEP_MAX_PACKET) packetCnt = 0;	// Default value of eeprom is 0xFF, hence set to 0
	return packetCnt;	
}

//Setters
void setFunction(uint8_t *buffer,uint8_t offset)
{
	
}
void setRatedspeed(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t ratedSpeed = 0;
		ratedSpeed |= buffer[1]<<8;
		ratedSpeed |= buffer[2];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_RATEDSPEED),ratedSpeed);
	Leave_Critical_Section
}
void setCurrentspeed(uint16_t rpm)
{
	Enter_Critical_Section
		RPM = rpm;
	Leave_Critical_Section
}
void setNumberofteeth(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t numberofTeeth = 0;
		numberofTeeth |= buffer[5]<<8;
		numberofTeeth |= buffer[6];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_NUMBEROFTEETH),numberofTeeth);
	Leave_Critical_Section
}
void setOverspeedpct(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_OVERSPEEDPCT),buffer[7]);
	Leave_Critical_Section
}
void setTripdelay(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_TRIPDELAY),buffer[8]);
	Leave_Critical_Section
}
void setDate(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section	
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_DATEYEAR),buffer[9]);
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_DATEMONTH),buffer[10]);
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_DATEDAY),buffer[11]);
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_DATEHOURS),buffer[12]);
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_DATEMINUTES),buffer[13]);
	Leave_Critical_Section
}
void setPacketCnt(uint8_t packetCnt)
{
	Enter_Critical_Section
		eeprom_update_byte((uint8_t*)EEPADDR_PACKETCNT,packetCnt);
	Leave_Critical_Section
}
void setcurrentSettings(settings_t *settings)
{
	double rpmOverspeed_d = 0;
	uint8_t currentPacketOffset = getPacketCnt();
	if(currentPacketOffset != 0)
	{
		currentPacketOffset--;
	}
	currentPacketOffset *= 16;
	Enter_Critical_Section
		settings->ratedSpeed = getRatedspeed(currentPacketOffset);
		settings->numberofTeeth = getNumberofteeth(currentPacketOffset);
		settings->overspeedPct = getOverspeedpct(currentPacketOffset);
		settings->tripDelay = getTripdelay(currentPacketOffset);
		
		rpmOverspeed_d = (double)settings->ratedSpeed*(1.0 + ((double)(settings->overspeedPct)/100) );
		settings->rpmOverspeed = (uint16_t)rpmOverspeed_d;
	Leave_Critical_Section
}

uint8_t processCRC(uint8_t *buffer,uint8_t length)
{
	uint8_t  crcError = 0;
	uint16_t crcRx = 0;
	crcRx |= buffer[14]<<8;
	crcRx |= buffer[15];
	uint16_t crcCalc = getCRC(buffer,length);
	
	if(crcCalc != crcRx) crcError = 1;
	return crcError;
}
void processWritePacket(uint8_t *buffer)
{
	uint8_t sendBuffer[FRAME_NORMAL];
	uint8_t function = FRAME_ACK;
	uint8_t ackFunction = 0;
	uint8_t packetCnt = getPacketCnt();
	uint8_t offset = packetCnt*16; 

	setRatedspeed(buffer,offset);
	setNumberofteeth(buffer,offset);
	setOverspeedpct(buffer,offset);
	setTripdelay(buffer,offset);
	setDate(buffer,offset);
	
	packetCnt++;	
	setPacketCnt(packetCnt);
	
	sendBuffer[0] = function;
	sendBuffer[1] = ackFunction;
	for(uint8_t i = 2;i < FRAME_NORMAL; i++)
	{
		sendBuffer[i] = 0;
	}
	
	for(uint8_t i = 0; i < FRAME_NORMAL;i++)
	{
		USART_writechar(sendBuffer[i]);
	}
}
void processReadPacket(void)
{
	uint8_t sendBuffer[FRAME_NORMAL];
	
	uint8_t function = FRAME_READ;
	uint16_t currentSpeed = 0;
	uint16_t crc = 0;
	uint8_t packetCnt = getPacketCnt()-1;
	uint8_t offset = packetCnt*16;
	sendBuffer[0] = function;
	sendBuffer[4] = currentSpeed;
	sendBuffer[3] = currentSpeed>>8;
	
	Enter_Critical_Section
		sendBuffer[2] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_RATEDSPEED));
		sendBuffer[1] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_RATEDSPEED+1));
		sendBuffer[6] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH));
		sendBuffer[5] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH+1));
		sendBuffer[7] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_OVERSPEEDPCT));
		sendBuffer[8] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_TRIPDELAY));
	Leave_Critical_Section
	sendBuffer[9] = 0;
	sendBuffer[10] = 0;
	sendBuffer[11] = 0;
	sendBuffer[12] = 0;
	sendBuffer[13] = 0;
	
	crc = getCRC(sendBuffer,14);
	sendBuffer[15] = crc;
	sendBuffer[14] = crc>>8;
	
	for(uint8_t i = 0; i < FRAME_NORMAL;i++)
	{
		USART_writechar(sendBuffer[i]);
	}
}
void processTestPacket(void)
{
	
}
void processLogPacket(void)
{
	uint8_t sendBuffer[FRAME_NORMAL];
	uint8_t function = FRAME_LOG;
	uint16_t currentSpeed = 0;
	uint16_t crc = 0;
	uint8_t firstLogpacket = 0;
	uint8_t packetCnt = getPacketCnt();
	uint8_t offset = 0;
	
	if(packetCnt > EEP_MAX_LOG_PACKET) firstLogpacket = packetCnt - EEP_MAX_LOG_PACKET;
	else firstLogpacket = 0;
	for (uint8_t i = packetCnt; i > firstLogpacket; i--)
	{
		offset = 16*(i-1);
		sendBuffer[0] = function;
		sendBuffer[4] = currentSpeed;
		sendBuffer[3] = currentSpeed>>8;
		
		sendBuffer[2] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_RATEDSPEED));
		sendBuffer[1] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_RATEDSPEED+1));
		sendBuffer[6] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH));
		sendBuffer[5] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH+1));
		sendBuffer[7] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_OVERSPEEDPCT));
		sendBuffer[8] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_TRIPDELAY));
		
		sendBuffer[9] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_DATEYEAR));
		sendBuffer[10] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_DATEMONTH));
		sendBuffer[11] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_DATEDAY));
		sendBuffer[12] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_DATEHOURS));
		sendBuffer[13] = eeprom_read_byte((uint8_t*)(offset + EEPADDR_DATEMINUTES));
		
		crc = getCRC(sendBuffer,14);
		sendBuffer[15] = crc;
		sendBuffer[14] = crc>>8;
		
		for(uint8_t j = 0; j < FRAME_NORMAL;j++)
		{
			USART_writechar(sendBuffer[j]);
		}
	}
}
void processAckPacket(void)
{
	
}
void processSendCurrentRPM(void)
{
	uint8_t sendBuffer[FRAME_CURRENT_RPM];
	
	uint8_t function = FRAME_CURRENTRPM;
	uint8_t ratedSpeed = 0;
	uint16_t currentSpeed = getCurrentspeed();
	
	
	sendBuffer[0] = function;
	sendBuffer[2] = ratedSpeed;
	sendBuffer[1] = ratedSpeed>>8;
	sendBuffer[4] = currentSpeed;
	sendBuffer[3] = currentSpeed>>8;
	
	for(uint8_t i = 0;i<FRAME_CURRENTRPM;i++)
	{
		USART_writechar(sendBuffer[i]);
	} 

}
void config_main(void)
{
	
	uint8_t processBuffer[RCV_BUFFER_MAX];
	Enter_Critical_Section
	for(uint8_t i = 0; i<RCV_BUFFER_MAX;i++)
	{
		processBuffer[i] = rcvBuffer[i];
		rcvBuffer[i] = 0;
	}
	Leave_Critical_Section
	
	switch(processBuffer[0])
	{
		case FRAME_WRITE :
			processWritePacket(processBuffer);
			setcurrentSettings(&currentSettings);
		break;
		
		case FRAME_READ :
			processReadPacket();
		break;
		
		case FRAME_TEST :
			processTestPacket();
		break;
		
		case FRAME_LOG :
			processLogPacket();
		break;
		
		case FRAME_ACK :
			processAckPacket();
		break;
		
	}
	rcvState = STATE_RX_IDLE;
			
}


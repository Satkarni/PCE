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
	
	uint8_t temp = 0xFF;
	switch(rcvState)
	{
		
		case STATE_RX_IDLE :
			temp = USART_readchar();
				
			if((temp == FRAME_READ) || (temp == FRAME_WRITE) )	// start of new packet
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
				
				if(rcvPos == FRAME_NORMAL ) 
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
	while(!(UCSR0A & (1<<RXC0))){}
	return UDR0;
}
void USART_writechar(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0))) {}
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
uint8_t getFunction(uint8_t offset)
{
	return 0;
}
uint16_t getNumberofteeth(uint8_t offset)
{
	uint16_t numberofTeeth = 0;
	Enter_Critical_Section
		numberofTeeth = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH + 1)))<<8;
		numberofTeeth |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_NUMBEROFTEETH));
	Leave_Critical_Section
	return numberofTeeth;
}
uint16_t getMinRPM(uint8_t offset)
{
	uint16_t minRPM = 0;
	Enter_Critical_Section
		minRPM = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_MINRPM + 1)))<<8;
		minRPM |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_MINRPM));
	Leave_Critical_Section
	return minRPM;
}
uint16_t getMaxRPM(uint8_t offset)
{
	uint16_t maxRPM = 0;
	Enter_Critical_Section
		maxRPM = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_MAXRPM + 1)))<<8;
		maxRPM |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_MAXRPM));
		Leave_Critical_Section
	return maxRPM;
}
uint8_t getOutputType(uint8_t offset)
{
	uint8_t outputType = 0;
	Enter_Critical_Section
		outputType |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_OUTPUTTYPE));
	Leave_Critical_Section
	return outputType;
}
uint8_t getCurrentRange(uint8_t offset)
{
	uint8_t currentRange = 0;
	Enter_Critical_Section
		currentRange |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_CURRENTRANGE));
	Leave_Critical_Section
	return currentRange;
}
uint8_t getVoltageRange(uint8_t offset)
{
	uint8_t voltageRange = 0;
	Enter_Critical_Section
		voltageRange |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_VOLTAGERANGE));
	Leave_Critical_Section
	return voltageRange;
}

uint8_t getRly1Enable(uint8_t offset)
{
	uint8_t rly1Enable = 0;
	Enter_Critical_Section
		rly1Enable |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_EN));
	Leave_Critical_Section
	return rly1Enable;
}
uint8_t getRly1Resettype(uint8_t offset)
{
	uint8_t rly1ResetType = 0;
	Enter_Critical_Section
		rly1ResetType |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_RST));
	Leave_Critical_Section
	return rly1ResetType;
}
uint16_t getRly1OnRPM(uint8_t offset)
{
	uint16_t onRPM = 0;
	Enter_Critical_Section
		onRPM = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_ONRPM + 1)))<<8;
		onRPM |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_ONRPM));
	Leave_Critical_Section
	return onRPM;
}
uint16_t getRly1OffRPM(uint8_t offset)
{
	uint16_t offRPM = 0;
	Enter_Critical_Section
		offRPM = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_OFFRPM + 1)))<<8;
		offRPM |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_OFFRPM));
	Leave_Critical_Section
	return offRPM;
}
uint8_t getRly1ResetDelay(uint8_t offset)
{
	uint8_t rly1Delay = 0;
	Enter_Critical_Section
		rly1Delay |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY1_DELAY));
	Leave_Critical_Section
	return rly1Delay;
}

uint8_t getrly2Enable(uint8_t offset)
{
	uint8_t rly2Enable = 0;
	Enter_Critical_Section
		rly2Enable |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_EN));
	Leave_Critical_Section
	return rly2Enable;
}
uint8_t getrly2Resettype(uint8_t offset)
{
	uint8_t rly2ResetType = 0;
	Enter_Critical_Section
		rly2ResetType |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_RST));
	Leave_Critical_Section
	return rly2ResetType;
}
uint16_t getrly2OnRPM(uint8_t offset)
{
	uint16_t onRPM = 0;
	Enter_Critical_Section
		onRPM = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_ONRPM + 1)))<<8;
		onRPM |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_ONRPM));
	Leave_Critical_Section
	return onRPM;
}
uint16_t getrly2OffRPM(uint8_t offset)
{
	uint16_t offRPM = 0;
	Enter_Critical_Section
		offRPM = (eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_OFFRPM + 1)))<<8;
		offRPM |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_OFFRPM));
	Leave_Critical_Section
	return offRPM;
}
uint8_t getrly2ResetDelay(uint8_t offset)
{
	uint8_t rly2Delay = 0;
	Enter_Critical_Section
		rly2Delay |= eeprom_read_byte((uint8_t*)(offset + EEPADDR_RLY2_DELAY));
	Leave_Critical_Section
	return rly2Delay;
}


//Setters
void setPacketCnt(uint8_t packetCnt)
{
	Enter_Critical_Section
		eeprom_update_byte((uint8_t*)EEPADDR_PACKETCNT,packetCnt);
	Leave_Critical_Section
}
void setFunction(uint8_t *buffer,uint8_t offset)
{
	
}
void setNumberofteeth(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t numberofTeeth = 0;
		numberofTeeth |= buffer[1]<<8;
		numberofTeeth |= buffer[2];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_NUMBEROFTEETH),numberofTeeth);
	Leave_Critical_Section
}
void setMinRPM(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t minRPM = 0;
		minRPM |= buffer[3]<<8;
		minRPM |= buffer[4];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_MINRPM),minRPM);
	Leave_Critical_Section
}
void setMaxRPM(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t maxRPM = 0;
		maxRPM |= buffer[5]<<8;
		maxRPM |= buffer[6];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_MAXRPM),maxRPM);
	Leave_Critical_Section
}
void setOutputType(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t outputType = 0;
		outputType = buffer[7];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_OUTPUTTYPE),outputType);
	Leave_Critical_Section
}
void setCurrentRange(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t currentRange = 0;
		currentRange = buffer[8];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_CURRENTRANGE),currentRange);
	Leave_Critical_Section
}
void setVoltageRange(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t voltageRange = 0;
		voltageRange = buffer[9];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_VOLTAGERANGE),voltageRange);
	Leave_Critical_Section
}

void setRly1Enable(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t rly1Enable = 0;
		rly1Enable = buffer[10];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_RLY1_EN),rly1Enable);
	Leave_Critical_Section
}
void setRly1Resettype(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t rly1Resettype = 0;
		rly1Resettype = buffer[11];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_RLY1_RST),rly1Resettype);
	Leave_Critical_Section
}
void setRly1OnRPM(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t onRPM = 0;
		onRPM |= buffer[12]<<8;
		onRPM |= buffer[13];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_RLY1_ONRPM),onRPM);
	Leave_Critical_Section
}
void setRly1OffRPM(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t offRPM = 0;
		offRPM |= buffer[14]<<8;
		offRPM |= buffer[15];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_RLY1_OFFRPM),offRPM);
	Leave_Critical_Section
}
void setRly1ResetDelay(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t rly1Resetdelay = 0;
		rly1Resetdelay = buffer[16];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_RLY1_DELAY),rly1Resetdelay);
	Leave_Critical_Section
}

void setRly2Enable(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t Rly2Enable = 0;
		Rly2Enable = buffer[17];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_RLY2_EN),Rly2Enable);
	Leave_Critical_Section
}
void setRly2Resettype(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t Rly2Resettype = 0;
		Rly2Resettype = buffer[18];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_RLY2_RST),Rly2Resettype);
	Leave_Critical_Section
}
void setRly2OnRPM(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t onRPM = 0;
		onRPM |= buffer[19]<<8;
		onRPM |= buffer[20];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_RLY2_ONRPM),onRPM);
	Leave_Critical_Section
}
void setRly2OffRPM(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint16_t offRPM = 0;
		offRPM |= buffer[21]<<8;
		offRPM |= buffer[22];
		eeprom_update_word((uint16_t*)(offset + EEPADDR_RLY2_OFFRPM),offRPM);
	Leave_Critical_Section
}
void setRly2ResetDelay(uint8_t *buffer,uint8_t offset)
{
	Enter_Critical_Section
		uint8_t Rly2Resetdelay = 0;
		Rly2Resetdelay = buffer[23];
		eeprom_update_byte((uint8_t*)(offset + EEPADDR_RLY2_DELAY),Rly2Resetdelay);
	Leave_Critical_Section
}

void setcurrentSettings(settings_t *settings)
{
	uint8_t offset = 0;
	settings->numberofTeeth = getNumberofteeth(offset);
	settings->minRpm = getMinRPM(offset);
	settings->maxRpm = getMaxRPM(offset);
	settings->outputType = getOutputType(offset);
	settings->currentRange = getCurrentRange(offset);
	settings->voltageRange = getVoltageRange(offset);
	
	settings->rly1Enable = getRly1Enable(offset);
	settings->rly1Resettype = getRly1Resettype(offset);
	settings->rly1onRpm = getRly1OnRPM(offset);
	settings->rly1offRpm = getRly1OffRPM(offset);
	settings->rly1Resetdelay = getRly1ResetDelay(offset);
	
	settings->rly2Enable = getrly2Enable(offset);
	settings->rly2Resettype = getrly2Resettype(offset);
	settings->rly2onRpm = getrly2OnRPM(offset);
	settings->rly2offRpm = getrly2OffRPM(offset);
	settings->rly2Resetdelay = getrly2ResetDelay(offset);
	
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
	uint8_t packetCnt = getPacketCnt();
	uint8_t offset = packetCnt*16; 
	
	setNumberofteeth(buffer,offset);
	setMinRPM(buffer,offset);
	setMaxRPM(buffer,offset);
	setOutputType(buffer,offset);
	setCurrentRange(buffer,offset);
	setVoltageRange(buffer,offset);

	setRly1Enable(buffer,offset);
	setRly1Resettype(buffer,offset);
	setRly1OnRPM(buffer,offset);
	setRly1OffRPM(buffer,offset);
	setRly1ResetDelay(buffer,offset);

	setRly2Enable(buffer,offset);
	setRly2Resettype(buffer,offset);
	setRly2OnRPM(buffer,offset);
	setRly2OffRPM(buffer,offset);
	setRly2ResetDelay(buffer,offset);
	
	//packetCnt++;	
	//setPacketCnt(packetCnt);
	
	sendBuffer[0] = function;
	for(uint8_t i = 1;i < FRAME_NORMAL; i++)
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
	//uint16_t currentSpeed = 0;
	//uint16_t crc = 0;
	//uint8_t packetCnt = getPacketCnt()-1;
	//uint8_t offset = packetCnt*16;
	uint8_t offset = 0;
	sendBuffer[0] = function;
	
	uint16_t numberofTeeth = getNumberofteeth(offset);
	sendBuffer[2] = numberofTeeth;
	sendBuffer[1] = numberofTeeth>>8;
	
	uint16_t minRPM = getMinRPM(offset);
	sendBuffer[4] = minRPM;
	sendBuffer[3] = minRPM>>8;
	
	uint16_t maxRPM = getMaxRPM(offset);
	sendBuffer[6] = maxRPM;
	sendBuffer[5] = maxRPM>>8;
	
	uint8_t  outputType = getOutputType(offset);
	sendBuffer[7] = outputType;
	
	uint8_t currentRange = getCurrentRange(offset);
	sendBuffer[8] = currentRange;
	
	uint8_t voltageRange = getVoltageRange(offset);
	sendBuffer[9] = voltageRange;
	
	uint8_t rly1Enable = getRly1Enable(offset);
	sendBuffer[10] = rly1Enable;
	
	uint8_t rly1ResetType = getRly1Resettype(offset);
	sendBuffer[11] = rly1ResetType;
	
	uint16_t rly1OnRPM = getRly1OnRPM(offset);
	sendBuffer[13] = rly1OnRPM;
	sendBuffer[12] = rly1OnRPM>>8;
	
	uint16_t rly1OffRPM = getRly1OffRPM(offset);
	sendBuffer[15] = rly1OffRPM;
	sendBuffer[14] = rly1OffRPM>>8;
	
	uint16_t rly1Delay = getRly1ResetDelay(offset);
	sendBuffer[16] = rly1Delay;
	
	uint8_t rly2Enable = getrly2Enable(offset);
	sendBuffer[17] = rly2Enable;
	
	uint8_t rly2ResetType = getrly2Resettype(offset);
	sendBuffer[18] = rly2ResetType;
	
	uint16_t rly2OnRPM = getrly2OnRPM(offset);
	sendBuffer[20] = rly2OnRPM;
	sendBuffer[19] = rly2OnRPM>>8;
	
	uint16_t rly2OffRPM = getrly2OffRPM(offset);
	sendBuffer[22] = rly2OffRPM;
	sendBuffer[21] = rly2OffRPM>>8;
	
	uint16_t rly2Delay = getrly2ResetDelay(offset);
	sendBuffer[23] = rly2Delay;
	
	
	//crc = getCRC(sendBuffer,14);
	//sendBuffer[15] = crc;
	//sendBuffer[14] = crc>>8;
	
	for(uint8_t i = 0; i < FRAME_NORMAL;i++)
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
		break;
		
		case FRAME_READ :
			processReadPacket();
		break;
		
	}
	rcvState = STATE_RX_IDLE;
			
}


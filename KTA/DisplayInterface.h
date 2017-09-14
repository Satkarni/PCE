/*
 * DisplayInterface.h
 *
 * Created: 9/6/2016 11:45:13 AM
 *  Author: Piyush
 */ 


#ifndef DISPLAYINTERFACE_H_
#define DISPLAYINTERFACE_H_

#include "RS232.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Hardware.h"
#include "expander/outputHandler.h"
#include "sensor_lib/SensorsDAO.h"
#include "sensor_lib/SensorHandler.h"
#include "sensor_lib/TimerQueue.h"
#include "RTC/RTCHandler.h"
#include "ADC.h"
#include "Calibration.h"
#include "Timers.h"

#define	ID_RPMGAUGE		0x0800
#define ID_ECTGAUGE		0x0801
#define ID_LOPGAUGE		0x0802
#define ID_AMMGAUGE		0x1102
#define ID_ENGINECON      0x1112
#define ID_ENGHOURS3     0x1115
#define ID_ENGHOURS2     0x1114
#define ID_ENGHOURS1     0x1113

#define ID_LOADGAUGE		0x0B00

#define ID_ADC0RAW		0x0F00
#define ID_ADC1RAW		0x0F01
#define ID_ADC2RAW		0x0F02
#define ID_ADC3RAW		0x0F03
#define ID_ADC4RAW		0x0F04
#define ID_ADC5RAW		0x0F05
#define ID_ADC6RAW		0x0F06
#define ID_ADC7RAW		0x0F07

#define ID_ADC0VOLTAGE	0x0F08
#define ID_ADC1VOLTAGE	0x0F09
#define ID_ADC2VOLTAGE	0x0F0A
#define ID_ADC3VOLTAGE	0x0F0B
#define ID_ADC4VOLTAGE	0x0F0C
#define ID_ADC5VOLTAGE	0x0F0D
#define ID_ADC6VOLTAGE	0x0F0E
#define ID_ADC7VOLTAGE	0x0F0F

#define ID_DUTYCYCLE		0x1000
#define ID_THROTTLEPERCENT 0x0F11
#define ID_ACTUALPERCENT	0x0F10
#define ID_THROTTLEMIN	0x0F12
#define ID_THROTTLEMAX	0x0F13
#define ID_ACTUATORMAX	0x0F14
#define ID_SPEEDPERCENT 0x0F15
#define ID_COMMANDPOS	0x0B01
#define ID_ACTUALPOS		0x0B02

#define ID_OVSFAULT		0x2100
#define ID_ECTFAULT		0x2102
#define ID_LOPFAULT		0x2101
#define ID_CHARGEFAULT	0x2103

#define ID_HOURS		0x1100
#define ID_MINUTES	0x1101
#define ID_AMPM		0x1103
#define ID_DATE		0x1104
#define ID_MONTH		0x1105
#define ID_YEAR		0x1106

#define ID_PCEFORM				0x0A01
#define ID_MAINFORM			0x0A00
#define ID_DIAGFORM			0x0A02
#define ID_ACTTESTFORM		0x0A03
#define ID_SETTINGSFORM		0x0A04
#define ID_ALARMSFORM		0x0A05
#define ID_ACTCALFORM		0x0A06
#define ID_THRCALFORM		0x0A07
#define ID_PINFORM				0x0A08
#define ID_RATIOFORM			0x0A09
#define ID_SETPOINTSFORM	0x0A0A
#define ID_RESETFORM			0x0A0B

#define CUR_ACTDIAG		0x1B01
#define CUR_ADCDIAG		0x1B02
#define CUR_THRCAL			0x1B03
#define CUR_ACTCAL			0x1B04
#define CUR_RATIO			0x1B05
#define CUR_SETPOINTS		0x1B06
#define CUR_ALARMS			0x1B07
#define CUR_RESET			0x1B08
#define ID_LOGINMSG		0x110A
#define ID_THRDEV			0x110B
#define ID_ACTDEV			0x110C
#define ID_RATIODEV			0x110D
#define ID_SETPOINTDEV	0x110E
#define ID_ALARMSDEV		0x110F
#define ID_RESETDEV			0x1110

#define CUR_OVSENABLED		0x1B09
#define CUR_ECTENABLED		0x1B0A
#define CUR_LOPENABLED		0x1B0B
#define CUR_AMMENABLED		0x1B0C
#define ID_OVSENABLED		0x1300
#define ID_ECTENABLED			0x1301
#define ID_LOPENABLED			0x1302
#define ID_AMMENABLED		0x1303

#define ID_ACTPWM			0x1001
#define ID_ACTFEEDBACK	0x0F1B
#define ID_ACTCALMSG		0x1107

#define ID_THRCALMSG		0x1108
#define ID_THRFEEDBACK	0x0B03
#define ID_THRADC			0x0F1C

#define ID_PINBOX1			0x0F1D
#define ID_PINBOX2			0x0F1E
#define ID_PINBOX3			0x0F1F
#define ID_PINBOX4			0x0F20
#define CUR_PINBOX1		0x1B12
#define CUR_PINBOX2		0x1B13
#define CUR_PINBOX3		0x1B14
#define CUR_PINBOX4		0x1B15
#define CUR_PINBOX1D		0x1B16
#define CUR_PINBOX2D		0x1B17
#define CUR_PINBOX3D		0x1B18
#define CUR_PINBOX4D		0x1B19
#define ID_PINMSG				0x1109

#define ID_ACTIVERATIO		0x0F22
#define ID_NEWRATIO		0x0F23

#define CUR_OVSSETPOINT	0x1B0D
#define CUR_RUNSETPOINT	0x1B0E
#define CUR_ECTSETPOINT		0x1B0F
#define CUR_LOPSETPOINT 	0x1B10
#define CUR_AMMSETPOINT	0x1B11
#define ID_OVSSETPOINT		0x0F16
#define ID_RUNSETPOINT		0x0F1A
#define ID_ECTSETPOINT		0x0F17
#define ID_LOPSETPOINT		0x0F18
#define ID_AMMSETPOINT		0x0F19
#define ID_SETPOINTCHECK	0x1B1C

#define ID_RESETTIMER			0x1111

#define COMMAND_READ			0x00
#define COMMAND_WRITE			0x01
#define COMMAND_CONTRAST	0x04
#define COMMAND_REPORT		0x05

#define MIN_CONTRAST		0x01
#define MAX_CONTRAST	0x0F

#define OPERATOR_PASSWORD	0000
#define DEVELOPER_PASSWORD	347

uint8_t current_contrast, settingsAccess;
uint16_t current_form;

volatile uint16_t display_objectValue, display_cursorValue, display_pinCursor;
volatile uint8_t  ack_received, engineCondition;

struct actuator_struct
{
	uint16_t speed_percentage;
	uint16_t throttle_percentage;
	uint16_t command_percentage;
	uint16_t actuator_percentage;
};

struct actuator_struct actuator_data;

void display_init();
void display_POST(void);
uint8_t display_getChecksum(uint8_t *packet, uint8_t packet_length);
void display_setGauge(uint16_t newValue, uint16_t gauge_id);
void display_setFaultLED(uint8_t ledState, uint16_t led_id);
void display_setTime(uint8_t hrs, uint8_t mins);
void display_setDate(uint8_t date, uint8_t month, uint8_t year);
void display_setForm(uint16_t form_id);
uint16_t display_getObjectValue(uint16_t object_id);
void display_brightnessIncrement();
void display_brightnessDecrement();
void display_updateADCGauges(void);
void display_updateMainGauges();
void display_updateACTGauges();
void display_setCursor(uint16_t cursor_id, uint8_t enable);
void display_setPINCursor(uint16_t cursor_id, uint8_t enable);
void display_checkPIN();

void display_Reset();

#endif /* DISPLAYINTERFACE_H_ */

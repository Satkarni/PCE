/*
 * Config.h
 *
 * Created: 28/01/2017 14:54:56
 *  Author: ROHANP
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#define SYSTEM_TIMER_OCR_VAL	( 14400 )	// 1/1024 prescaler , 1 s cycle
#define ALARM_DELAY				( 5 )

struct DI_
{
	unsigned int alarm:1;
	unsigned int alarmC1:1;
	unsigned int alarmC2:1;
	unsigned int alarmC3:1;
	unsigned int buzzer:1;	 
	
	volatile uint8_t timer;
	volatile unsigned int timer_en:1;
	volatile unsigned int timer_done:1;
};

struct DI_ di1;
struct DI_ di2;


void init(void);
void test(void);

void system_timer_init();
void system_timer_enable();
void system_timer_disable();

void brightnessInit(void);
void brightnessPwmEnable(void);
void brightnessPwmdisable(void);
void brightnessSetduty(uint8_t duty);
void brightnessSet(void);

void lampCheck(void);

void process(void);


#endif /* CONFIG_H_ */
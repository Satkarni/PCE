/*
 * PTO_Remote_Panel.c
 *
 * Created: 28/01/2017 13:31:33
 *  Author: ROHANP
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Hardware.h"
#include "Config.h"

uint8_t brightness,lampcheck,MUTE,RESET;

void init(void)
{
	DDRB |= ((1<<POWER_LED)|(1<<LOW_OIL_LED)|(1<<FLT_FAIL_LED));
	DDRC |= (1<<DEBUG_LED);
	PORTB &= ~((1<<POWER_LED)|(1<<LOW_OIL_LED)|(1<<FLT_FAIL_LED));
	PORTC &= ~(1<<DEBUG_LED);
	
	DDRC &= ~((1<<SW_DIMMER)|(1<<SW_MUTE)|(1<<SW_RESET)|(1<<SW_LMP_CHECK));
	PORTC |= ((1<<SW_DIMMER)|(1<<SW_MUTE)|(1<<SW_RESET)|(1<<SW_LMP_CHECK));
	DDRD &= ~((1<<SW_DI1)|(1<<SW_DI2)|(1<<SW_DI3));
	PORTD |= ((1<<SW_DI1)|(1<<SW_DI2)|(1<<SW_DI3));
	
	DDRD |= ((1<<BUZZER)|(1<<BRIGHTNESS));
	BUZZER_OFF;
	BRIGHTNESS_ON;
	brightnessInit();
	
	system_timer_init();
	system_timer_enable();
}

void test(void)
{
	brightnessPwmdisable();
	BRIGHTNESS_ON;
	uint8_t count = 5;
	while(count-->0)
	{
		POWER_LED_TOGGLE;
		LOW_OIL_LED_TOGGLE;
		FLT_FAIL_LED_TOGGLE;
		DEBUG_LED_TOGGLE;
		_delay_ms(200);
	}
	
	BUZZER_ON;
	_delay_ms(1000);
	BUZZER_OFF;
	
	while (1)
	{
		if(SW_DIMMER_ON)	POWER_LED_ON;
		else                POWER_LED_OFF;
	
		if(SW_MUTE_ON || SW_DI1_ON)	LOW_OIL_LED_ON;
		else						LOW_OIL_LED_OFF;
		
		if(SW_RESET_ON || SW_DI2_ON) FLT_FAIL_LED_ON;
		else						 FLT_FAIL_LED_OFF;   
		
		if(SW_LMP_CHECK_ON)
		{
			POWER_LED_ON;
			LOW_OIL_LED_ON;
			FLT_FAIL_LED_ON;
			DEBUG_LED_ON;
			BUZZER_ON;
		}else
		{
			DEBUG_LED_OFF;
			BUZZER_OFF;
		}
	}
	BRIGHTNESS_OFF;
}

void system_timer_init()
{
	TCCR1B |= (1<<WGM12);		// CTC mode timer 1
	TCNT1 = 0x0000;
}

void system_timer_enable()
{
	TCNT1 = 0x0000;
	OCR1A  = SYSTEM_TIMER_OCR_VAL;
	TIMSK1 |= (1<<OCIE1A);
	TCCR1B |= (1<<CS12)|(1<<CS10);
}

void system_timer_disable()
{
	TCNT1 = 0x0000;
	TCCR1B &= ~((1<<CS12)|(1<<CS10));
}

ISR(TIMER1_COMPA_vect)
{
	DEBUG_LED_TOGGLE;
	if(di1.timer_en)
	{
		di1.timer++;
		if(di1.timer >= ALARM_DELAY)
		{
			di1.timer_done = 1;
			di1.timer = 0;
		}
		
	}
	
	if(di2.timer_en)
	{
		di2.timer++;
		if(di2.timer >= ALARM_DELAY)
		{
			di2.timer_done = 1;
			di2.timer = 0;
		}
	}
}

void brightnessInit(void)
{
	TCCR0A |= ((1<<WGM01)|(1<<WGM00)); // Fast PWM mode
	TCCR0B |= ((1<<CS02)|(0<<CS01)|(0<<CS00));	// 1/256 prescaler, PWM freq ~ 60 Khz
	brightness = 255;
	brightnessPwmEnable();
	brightnessSetduty(brightness);
}

void brightnessPwmEnable(void)
{
	TCCR0A |= ((1<<COM0A1)|(0<<COM0A0)); // Non inverted PWM
}

void brightnessPwmdisable(void)
{
	TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0)); // Non inverted PWM
}

void brightnessSetduty(uint8_t duty)
{
	OCR0A = duty;
}

void brightnessSet(void)
{
	brightnessPwmEnable();
	brightnessSetduty(brightness);
	if(SW_DIMMER_ON)
	{
		_delay_ms(200);
		brightness = (brightness == 0) ? 255 : (brightness - 51); 
	}
}

void brightnessTest(void)
{
	brightness = 255;
	brightnessPwmEnable();
	while(1)
	{
		brightness = (brightness == 0 ) ? 255 : (brightness - 51);
		brightnessSetduty(brightness);
		_delay_ms(1000);
	}
}

void lampCheck(void)
{
	
	if(SW_LMP_CHECK_ON)
	{
		brightnessPwmdisable();
		_delay_ms(1);
		BRIGHTNESS_ON;
		POWER_LED_ON;
		LOW_OIL_LED_ON;
		FLT_FAIL_LED_ON;
		DEBUG_LED_ON;
		BUZZER_ON;
		lampcheck = 1;
	}
	else
	{
		lampcheck = 0;
		brightnessSet();
	}
}

void process_lowOil(void)
{
	if(SW_DI1_ON)	
	{
		di1.timer_en = 1;
	}else
	{
		di1.timer = 0;
		di1.timer_done = 0;
		di1.timer_en = 0;
	}            
	
	if(di1.timer_done == 1)
	{
		di1.alarm = 1;
	}else
	{
		di1.alarm = 0;
	}
}

void process_filterFail(void)
{
	if(SW_DI2_ON)
	{
		di2.timer_en = 1;
	}else
	{
		di2.timer = 0;
		di2.timer_done = 0;
		di2.timer_en = 0;
	}
	
	if(di2.timer_done == 1)
	{
		di2.alarm = 1;
	}else
	{
		di2.alarm = 0;
	}
}

void process(void)
{
	POWER_LED_ON;
	lampCheck();
	
	if(SW_MUTE_ON) MUTE = 1;
	else MUTE = 0;
	
	if(SW_RESET_ON) RESET = 1;
	else RESET = 0;
	
	process_lowOil();
	process_filterFail();
	
	di1.alarmC1 = (di1.alarm) || (di1.alarmC1 && !RESET);
	if(di1.alarmC1 || lampcheck)	LOW_OIL_LED_ON;
	else							LOW_OIL_LED_OFF;
	
	di1.alarmC3 = di1.alarm && RESET;
	di1.alarmC2 = di1.alarmC1 && (MUTE || di1.alarmC2) && !di1.alarmC3;
	di1.buzzer = di1.alarmC1 && !di1.alarmC2;
	
	di2.alarmC1 = (di2.alarm) || (di2.alarmC1 && !RESET);
	if(di2.alarmC1 || lampcheck)	FLT_FAIL_LED_ON;
	else							FLT_FAIL_LED_OFF;
	
	di2.alarmC3 = di2.alarm && RESET;
	di2.alarmC2 = di2.alarmC1 && (MUTE || di2.alarmC2) && !di2.alarmC3;
	di2.buzzer = di2.alarmC1 && !di2.alarmC2;
	
	
	if(di1.buzzer || di2.buzzer || lampcheck)	BUZZER_ON;
	else										BUZZER_OFF;
	
}

int main(void)
{
    init();
	sei();
	while(1)
    {
		//test();
		//brightnessTest();
		process();
    }
}
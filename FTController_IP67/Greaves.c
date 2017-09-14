/*
 * Greaves.c
 *
 * Created: 05-10-2015 16:03:07
 *  Author: ROHANP
 */ 

/*
 * Local_controller_v1.c
 *
 * Created: 25-07-2015 14:00:03
 *  Author: ADMIN
 */ 

#include "Greaves.h"
#include <avr/io.h>

volatile uint8_t update_status = 0;
volatile uint8_t lop_sensor_enable_delay = 0;
volatile uint8_t lop_sensor_enable_timer=0;
volatile uint8_t starter_timer = 0;
volatile uint8_t start_timer_bit = 0;
volatile uint8_t stop_timer = 0;
volatile uint8_t stop_timer_bit = 0;
volatile uint8_t lockout = 0;

uint8_t count_runtime = 0;
uint8_t common_trip=0;
uint8_t lop_sensor_5 = 0;
uint8_t lop_sensor_2 = 0;

uint8_t rpm_sensor_delay = 0;
uint8_t battery_vge_alarm_delay = 0;
uint8_t lop_alarm_delay = 0;
uint8_t lop_trip_delay = 0;
uint8_t standstill = 0;

uint8_t START = 0;
uint8_t STOP = 0;
uint8_t MUTE = 0;
uint8_t RESET = 0;
uint16_t rpm100=0;
uint16_t rpm500=0;

volatile uint16_t rpm=0;
uint16_t run_time = 0;
uint8_t k = 0;
uint16_t lop_rawADC = 0;
uint8_t fuel_leak = 0;
uint8_t charge_fail = 0;

uint16_t lot_rawADC = 0;
uint16_t lot_rawADCarray[SAMPLE+3];
uint16_t wt_rawADC = 0;
uint16_t wt_rawADCarray[SAMPLE+3];
uint16_t bv_rawADC = 0;
uint16_t bv_rawADCarray[SAMPLE+3];

float lop = 0.0;
float lot = 0.0;
float wt = 0.0;
float bv = 0.0;

SENSOR lop_sensor;
SENSOR lot_sensor;
SENSOR wt_sensor;
SENSOR rpm_sensor;
SENSOR bv_sensor;
SENSOR fuel_leak_sensor;
SENSOR charge_fail_sensor;

DISPLAY				display;

SENSOR_ERROR_CODE	sensor_error_code;
uint16_t LED_state = 0;

uint8_t counter_routine=0;
uint8_t count_runtime;


 void system_timer_init(){
	
	TCCR1B |= (1<<WGM12);		// CTC mode timer 1
	TCNT1 = 0x0000;
}

 void system_timer_enable(){
	
	
	TCNT1 = 0x0000;
	OCR1A  = SYSTEM_TIMER_OCR_VAL;
	TIMSK |= (1<<OCIE1A);
	TCCR1B |= (1<<CS12)|(1<<CS10);
	
}

 void system_timer_disable(){
	
	TCNT1 = 0x0000;
	TCCR1B &= ~((1<<CS12)|(1<<CS10));
	
}

ISR(TIMER1_COMPA_vect){
	
	TCCR3B = OFF;
	rpm = TCNT3;
	rpm *= NUMBER_OF_TEETH;
	rpm_timer_enable();
	
	update_status = 1;
	
	if(start_timer_bit){
		starter_timer++;
		if(starter_timer > STARTER_TIMEOUT){
			start_timer_bit = 0;
			starter_timer=0;
		}	
	}
	
	if(stop_timer_bit){
		stop_timer++;
		if(stop_timer > STOP_TIMEOUT){
			stop_timer_bit = 0;
			stop_timer=0;
		}
	}
	
	
	if(!lockout){
		if(lop_sensor_enable_timer++ > LOP_SENSOR_ENABLE_DELAY){
		
			lop_sensor_enable_delay = 1;
			lop_sensor_enable_timer = 50;
		}
	}else{
		lop_sensor_enable_delay = 0;
		lop_sensor_enable_timer = 0;
	}
	
	
}

void rpm_timer_init(){
	
	DDRE &= ~(1<<MPU);
	PORTE |= (1<<MPU);
}
void rpm_timer_enable(){
	
	TCNT3 = 0x0000;
	TCCR3B |= (1<<CS32)|(1<<CS31) ;   //T3 external clock falling edge
}

 void ADC_enable(){
	
	ADCSRA |= (1<<ADEN);
}
void ADC_init(){
	
	
	ADMUX |= (1<<REFS0); // Aref=Vcc
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //F_CPU / 128  sampling rate = 115.2 ks per sec
	ADC_enable();
}
inline void ADC_disable() {
	
	ADCSRA &= ~(1<<ADEN);
}
uint16_t ADC_read(uint8_t channel){
	
	ADMUX = (ADMUX& 0xF8)|channel;
	
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC));
	return ADC;
}


void test_display_main(){
	
	uint16_t i=0;
	i=8888;
	MAX7219_display7segment(i,RPM);
	MAX7219_display7segment(i,WATER_TEMP);
	MAX7219_display7segment(i,LUBE_OIL_PRESSURE);
	MAX7219_display7segment(i,LUBE_OIL_TEMP);
	MAX7219_display7segment(i,BATTERY_VGE);
	MAX7219_display7segment(i,RUN_TIME);
	MAX7219_displayLED(0xFF,MAX7219_DIGIT1);
	MAX7219_displayLED(0xFF,MAX7219_DIGIT2);
	
	_delay_ms(1000);
	MAX7219_clearDisplay(CHIP0);
	MAX7219_clearDisplay(CHIP1);
	MAX7219_clearDisplay(CHIP2);
	MAX7219_clearDisplay(CHIP3);
	
	
	
}

void test_simple(){
	
	DDRA |= (1<<DEBUG_LED1)|(1<<DEBUG_LED2);
	
	while(1){
		
		PORTA ^= (1<<DEBUG_LED1);
		PORTA ^= (1<<DEBUG_LED2);
		_delay_ms(250);
	}
}

 void process_rpm(){
	
	rpm_sensor.alarm = 0;
	rpm_sensor.trip = 0;
	rpm_sensor.out_of_range = 0;
	
	if( rpm > RPM_TRIP){
		rpm_sensor.trip = 1;
	}
	if (rpm > RPM_ALARM){
		rpm_sensor.alarm = 1;
	}
	
	if((rpm < RPM_TOL) && (!lop_sensor.trip) )
	{
		rpm_sensor.out_of_range = 1;
		rpm_sensor_delay++;
		if(rpm_sensor_delay > (RPM_SENSOR_FAIL_DELAY + 10))
		{
			rpm_sensor_delay = RPM_SENSOR_FAIL_DELAY + 1;
		}
		
	}
	
	if(rpm_sensor_delay > RPM_SENSOR_FAIL_DELAY)
	{
		rpm_sensor.trip = 1;
		MAX7219_displayError(RPM_SENSOR_FAIL);
		
	}
	else{
		MAX7219_display7segment(rpm,RPM);
	}
	
	if(!rpm_sensor.out_of_range)
	{
		rpm_sensor_delay = 0;
	}
}

void process_lop(){
	
	lop_sensor.alarm = 0;
	lop_sensor.trip = 0;
	lop_sensor.out_of_range = 0;
	
	lop_rawADC = ADC_read(LUBE_OIL_PRESSURE_ADC_CHANNEL);
	
	
	if( lop_rawADC < (LOP_MIN_ADC - LOP_TOL_ADC)){
			
			lop_sensor.out_of_range = 1;
			lop_sensor.trip = 1;	
			MAX7219_displayError(LOP_SENSOR_OFFSCALE_LOW);
	}
	if(lop_rawADC > LOP_MAX_ADC){
			lop_sensor.out_of_range = 1;
			lop_sensor.trip = 1;
			MAX7219_displayError(LOP_SENSOR_OFFSCALE_HIGH);
	}
	
	if( lop_rawADC < LOP_TRIP_ADC){
				lop_sensor.trip = 1;
	}
	if( (lop_rawADC < LOP_ALARM_ADC) && (lop_rawADC >= LOP_TRIP_ADC)){
			
				lop_sensor.alarm = 1;	
	}
	
	
	if(!lop_sensor.out_of_range){
		
		signed int temp_lop = 0;
		
		temp_lop = lop_rawADC - LOP_MIN_ADC;
		if(temp_lop <= 0) { temp_lop = 0; }
			
		lop = (float)(LOP_SLOPE_ADC*temp_lop);
		MAX7219_display7segment(lop,LUBE_OIL_PRESSURE);
	}
	 
		
}

void process_lot(uint8_t init){
	
	lot_sensor.alarm = 0;
	lot_sensor.out_of_range = 0;
	
	lot_rawADC = ADC_read(LUBE_OIL_TEMP_ADC_CHANNEL);
		if(init)
		{
			for(uint16_t p=SAMPLE;p>1;p--)
			{
				lot_rawADCarray[p] = lot_rawADC;  //fill the array with initial value
			}
		}
		else
		{
			for(uint16_t p=SAMPLE;p>1;p--)
			{
				lot_rawADCarray[p] = lot_rawADCarray[p-1];
			}
		}
		
		
		lot_rawADCarray[1] = lot_rawADC;
		uint16_t lot_rawADCtotal = 0;
		for(uint8_t q=1;q<=SAMPLE;q++)
		{
			lot_rawADCtotal += lot_rawADCarray[q];
		}
		
		lot_rawADC = (lot_rawADCtotal/SAMPLE);
		
		//lot_rawADC += LOT_WIRE_RES;
	
	
	//if( lot_rawADC < (LOT_MIN_ADC - LOT_TOL_ADC)){
			//lot_sensor.out_of_range = 1;
			//lot_sensor.alarm = 1;
			//MAX7219_displayError(LOT_SENSOR_OFFSCALE_LOW);
	//}
	//if(lot_rawADC > LOT_MAX_ADC){
			//lot_sensor.out_of_range = 1;
			//lot_sensor.alarm = 1;
			//MAX7219_displayError(LOT_SENSOR_OFFSCALE_HIGH);
	//}
	//
	//if( lot_rawADC >= LOT_ALARM_ADC){
			//
			//lot_sensor.alarm = 1;
	//}
		
	if(!lot_sensor.out_of_range){		
			
			lot = (float)(lot_rawADC*ADC_REF)/ADC_MAX_VAL;
			lot = (lot*PT100_CURRENT_MA)/(PT100_CURRENT*PT100_GAIN);
			lot = (lot-PT100_RES_AT_0_DEGREE)/PT100_ADC_SCALE;
			
			if(lot <= 0) { lot = 0;}
			MAX7219_display7segment(lot,LUBE_OIL_TEMP);
	}
	
}

void process_wt(uint8_t init){
	
	wt_sensor.alarm = 0;
	wt_sensor.out_of_range = 0;
	
	wt_rawADC = ADC_read(WATER_TEMP_ADC_CHANNEL);
	
	if(init)
	{
		for(uint16_t p=SAMPLE;p>1;p--)
		{
			wt_rawADCarray[p] = wt_rawADC;  //fill the array with initial value
		}
	}
	else
	{
		for(uint16_t p=SAMPLE;p>1;p--)
		{
			wt_rawADCarray[p] = wt_rawADCarray[p-1];
		}
	}
	
	wt_rawADCarray[1] = wt_rawADC;
	uint16_t wt_rawADCtotal = 0;
	for(uint8_t q=1;q<=SAMPLE;q++)
	{
		wt_rawADCtotal += wt_rawADCarray[q];
	}
	
	wt_rawADC = (wt_rawADCtotal/SAMPLE);
	//wt_rawADC -= WT_WIRE_RES;
	
	//if( wt_rawADC < (WT_MIN_ADC - WT_TOL_ADC)){
		//
			//wt_sensor.out_of_range = 1;
			//wt_sensor.alarm = 1;
			//MAX7219_displayError(WT_SENSOR_OFFSCALE_LOW);
	//}
	//if(wt_rawADC > WT_MAX_ADC){
		//
			//wt_sensor.out_of_range = 1;
			//wt_sensor.alarm = 1;
			//MAX7219_displayError(WT_SENSOR_OFFSCALE_HIGH);
	//}
	//
	//if( wt_rawADC >= WT_ALARM_ADC){
			//
			//wt_sensor.alarm = 1;	
	//}
		
	if(!wt_sensor.out_of_range){		
			
			wt = (float)(wt_rawADC*ADC_REF)/ADC_MAX_VAL;
			wt = (wt*PT100_CURRENT_MA)/(PT100_CURRENT*PT100_GAIN);
			wt = (wt-PT100_RES_AT_0_DEGREE)/PT100_ADC_SCALE;
			
			if(wt <= 0){ wt = 0;}
			MAX7219_display7segment(wt,WATER_TEMP);
	}
}
	
void process_bv(uint8_t init){
	
	bv_sensor.alarm = 0;
	bv_sensor.out_of_range = 0;
	bv_rawADC = ADC_read(BATTERY_VGE_ADC_CHANNEL);
	
	if(init)
	{
		for(uint16_t p=SAMPLE;p>1;p--)
		{
			bv_rawADCarray[p] = bv_rawADC;  //fill the array with initial value
		}
	}
	else
	{
		for(uint16_t p=SAMPLE;p>1;p--)
		{
			bv_rawADCarray[p] = bv_rawADCarray[p-1];
		}
	}
	bv_rawADCarray[1] = bv_rawADC;
	uint16_t bv_rawADCtotal = 0;
	for(uint8_t q=1;q<=SAMPLE;q++)
	{
		bv_rawADCtotal += bv_rawADCarray[q];
	}
	
	bv_rawADC = (bv_rawADCtotal/SAMPLE);
	if( bv_rawADC <= BV_ALARM_ADC){

		bv_sensor.alarm = 1;
	}
	
	if(bv_rawADC > BV_SENSOR_FAIL_ADC){
		bv_sensor.out_of_range = 1;
		bv_sensor.alarm = 1;
		MAX7219_displayError(BV_SENSOR_FAIL);
	}else{

		//signed int temp_bat =0;
		//temp_bat = bv_rawADC - BV_MAX_ADC;
		//if(temp_bat<= 0) { temp_bat = 0;} 
			
		bv = (float)(BV_MAX_TO_DISPLAY + BV_SLOPE_ADC*bv_rawADC);
		
		
		MAX7219_display7segment(bv,BATTERY_VGE);
	}
}

void process_fuel_leak(){
	
	if( !(PINA & (1<<FUEL_LEAK_DI24V))){
		
		fuel_leak_sensor.alarm = 1;
	}
	else
	{
		fuel_leak_sensor.alarm = 0;
	}
	
}

void process_charge_fail(){
		
		if(!(PINA & (1<<CHARGE_FAIL_DI24V))){
			charge_fail_sensor.alarm = 1;	
		}else{
			charge_fail_sensor.alarm = 0;
		}
	
}

void eeprom_init(){
	
	for(uint8_t i = 0;i<256;i++){
		
		eeprom_update_byte((uint8_t*)i,0); 
	}
}
void process_runtime() {
	
	uint8_t minute_address;
	uint8_t minutes;
	
	if(count_runtime){
		
		if(counter_routine++ >= EEP_MIN_COUNT){		// 1 min has elapsed
			
			counter_routine = 0;
			
			cli();
			minute_address = eeprom_read_byte((uint8_t*)EEP_MIN_ADDRESS);
			if(minute_address == 0x00 || minute_address == 0xFF){					// at 1st minute
				minute_address = EEP_MIN_ADDRESS_OFFSET;
				eeprom_update_byte((uint8_t*)EEP_MIN_ADDRESS,minute_address);
			}
			
			minutes = eeprom_read_byte((uint8_t*)minute_address);
			
			if(minutes >= MIN_MAX_VAL){  				// 1st time at new address
				minutes = 0;
			}else{
				minutes++;
			}
			
			eeprom_update_byte((uint8_t*)minute_address,minutes);
			
			sei();
			if(minutes >= EEP_HOUR_COUNT){			// 1 hour has elapsed
				
				minutes = 0;
				cli();
				
				minute_address = eeprom_read_byte((uint8_t*)EEP_MIN_ADDRESS);
				minute_address++;
				if(minute_address >= EEP_MAX_ADDRESS){
					minute_address = EEP_MIN_ADDRESS_OFFSET;
				}
				
				eeprom_update_byte((uint8_t*)EEP_MIN_ADDRESS,minute_address);
				
				run_time = eeprom_read_word((uint16_t*)EEP_HOUR_ADDRESS);
				if(run_time >= RUNTIME_MAX_VAL){         // Either 1st hour of engine or greater than 9999
					run_time = 0;
				}else{
					run_time++;
				}
				
				eeprom_update_word((uint16_t*)EEP_HOUR_ADDRESS,run_time);
				
				
				sei();
			}
			
		}
		
	}
	
	run_time = eeprom_read_word((uint16_t*)EEP_HOUR_ADDRESS);
	if(run_time >= RUNTIME_MAX_VAL){         // Either 1st hour of engine or greater than 9999
		run_time = 0;
		eeprom_init();
	}
	MAX7219_display7segment(run_time,RUN_TIME);
	
	
}


int main(void){
	
				
			uint8_t sensor_delay_counter=0;
			
			DDRA |= (1<<DEBUG_LED1)|(1<<DEBUG_LED2);
			DEBUG_PORT |= (1<<DEBUG_LED1);
			_delay_ms(500);
			DEBUG_PORT &= ~(1<<DEBUG_LED1);
			_delay_ms(500);

			DDRA |= ((1<<START_OUTPUT)|(1<<STOP_OUTPUT)|(1<<SPARE_OUTPUT));
			DDRG |= (1<<BUZZER_OUTPUT);
			DDRD |= (1<<LOCKOUT_RELAY_OUTPUT);

			DDRA &= ~((1<<FUEL_LEAK_DI24V)|(1<<CHARGE_FAIL_DI24V)); // digital inputs
			DDRD &= ~((1<<START_INPUT)|(1<<STOP_INPUT));
			DDRE &= ~(1<<READY_TO_START_INPUT);

			spi_init();
			MAX7219_displayInit();
			ADC_init();
			rpm_timer_init();
			system_timer_init();
			

			test_display_main();
			
			rpm_timer_enable();
			system_timer_enable();


			sei();
			_delay_ms(500);
			
			process_lop();
			process_lot(1);
			process_wt(1);
			process_rpm();
			process_bv(1);
			process_fuel_leak();
			process_charge_fail();
			process_runtime();

		while(1){
			
					if(update_status){ // update sensor readings, enabled thorough ISR, 500 ms system timer
				
						update_status = 0;
				
						process_lop();
						process_rpm();
						process_runtime();
						process_charge_fail();
						process_fuel_leak();
						
						if(sensor_delay_counter >= SENSOR_DELAY)
						{
							process_lot(0);
							process_wt(0);
							process_bv(0);
							
							sensor_delay_counter = 0;
						}
						else
						{
							sensor_delay_counter++;
						}
						DEBUG_PORT ^= (1<<DEBUG_LED1);
					}
			
			
				
						if(!(PIND &(1<<START_INPUT))){ 
							START = 1;
						}else{
							START = 0;
						}
				
						if(PINE &(1<<READY_TO_START_INPUT)){
							STOP = 1;
						}else{
							STOP = 0;
						}
						
						if(SW_LOCAL_IN&(1<<SW_LOCAL_MUTE)){ //local mute button pressed
							MUTE = 1;
						}else{
					
							MUTE = 0;
						}
				
						if(SW_LOCAL_IN&(1<<SW_LOCAL_RESET)){ //local mute button pressed
							RESET = 1;
						}else{
					
							RESET = 0;
						}
				
						
				
						if (rpm < RPM_LOCKOUT_ENABLE){rpm100 = 1;}else{rpm100 = 0;}
						if (rpm > RPM_LOCKOUT_DISABLE){rpm500 = 1;}else{rpm500 = 0;}
				
						if(lop_sensor_enable_delay){	
							if(rpm500 || !lop_sensor.trip) {
								lop_sensor.enable = 1;
							}
						}
						if(stop_timer_bit){
							lop_sensor.enable = 0;
						}
				
			
	
					lop_sensor.alarmC1 = (lop_sensor.alarm && lop_sensor.enable) || (lop_sensor.alarmC1 &&  !RESET);	
					
					if((lop_sensor.alarmC1) || (lop_sensor.tripC1)){
						LED_state |= LOP_LOW_LED;
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}else{ 
						LED_state &= ~(LOP_LOW_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}
					lop_sensor.alarmC2 = (lop_sensor.alarmC1) && (MUTE || (lop_sensor.alarmC2));
					lop_sensor.buzzer1 = (lop_sensor.alarmC1) && !(lop_sensor.alarmC2);
			
					lop_sensor.tripC1 = (lop_sensor.trip && (lop_sensor.enable||lop_sensor.out_of_range)) || (lop_sensor.tripC1 && !RESET);
					
					lop_sensor.tripC2 = (lop_sensor.tripC1) && ((MUTE) || (lop_sensor.tripC2));
					lop_sensor.buzzer2 = ((lop_sensor.tripC1) && !(lop_sensor.tripC2));
			

					wt_sensor.alarmC1 = (wt_sensor.alarm) || (wt_sensor.alarmC1 && !RESET);
					if( wt_sensor.alarmC1){
						LED_state |= WATER_TEMP_HIGH_LED;
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						
					}else{
						LED_state &= ~(WATER_TEMP_HIGH_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						
					}
					wt_sensor.alarmC2 = wt_sensor.alarmC1 && (MUTE || wt_sensor.alarmC2);
					wt_sensor.buzzer = wt_sensor.alarmC1 && !wt_sensor.alarmC2;
			
			

					lot_sensor.alarmC1 = (lot_sensor.alarm) || (lot_sensor.alarmC1 && !RESET);
					if(lot_sensor.alarmC1){
				
						LED_state |= LOT_HIGH_LED;
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						
						}else{
						LED_state &= ~(LOT_HIGH_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						
					}
			
					lot_sensor.alarmC2 = lot_sensor.alarmC1 && (MUTE || lot_sensor.alarmC2);
					lot_sensor.buzzer = lot_sensor.alarmC1 && !lot_sensor.alarmC2;
			
			

					bv_sensor.alarmC1 = (bv_sensor.alarm) || (bv_sensor.alarmC1 && !RESET);
					if(bv_sensor.alarmC1){
				
						
						LED_state |= (BAT_VGE_LOW_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						}else{
						
						LED_state &= ~(BAT_VGE_LOW_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}
					bv_sensor.alarmC2 = bv_sensor.alarmC1 && (MUTE || bv_sensor.alarmC2);
					bv_sensor.buzzer = bv_sensor.alarmC1 && !bv_sensor.alarmC2;
			
			
			
			
					fuel_leak_sensor.alarmC1 = (fuel_leak_sensor.alarm) || (fuel_leak_sensor.alarmC1 && !RESET);
					if(fuel_leak_sensor.alarmC1){
				
						LED_state |= (FUEL_LEAK_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT2);
						
						}else{
						LED_state &= ~(FUEL_LEAK_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT2);
						
					}
					fuel_leak_sensor.alarmC2 = fuel_leak_sensor.alarmC1	&& (MUTE || fuel_leak_sensor.alarmC2);
					fuel_leak_sensor.buzzer = fuel_leak_sensor.alarmC1 && !(fuel_leak_sensor.alarmC2);
			
			
			
					charge_fail_sensor.alarmC1 = (charge_fail_sensor.alarm) || (charge_fail_sensor.alarmC1 && !RESET);
					if(charge_fail_sensor.alarmC1){
				
						LED_state |= (CHARGE_FAIL_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT2);
				
					}else{
					
						LED_state &= ~(CHARGE_FAIL_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT2);
					}
					charge_fail_sensor.alarmC2 = charge_fail_sensor.alarmC1	&& (MUTE || charge_fail_sensor.alarmC2);
					charge_fail_sensor.buzzer = charge_fail_sensor.alarmC1 && !(charge_fail_sensor.alarmC2) && (rpm500);
			
			
			
					rpm_sensor.enable = 1;
					rpm_sensor.alarmC1 = (rpm_sensor.alarm) || (rpm_sensor.alarmC1 && !RESET);
					
					if(rpm_sensor.tripC1 || rpm_sensor.alarmC1){
				
						LED_state |= (OVERSPEED_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}else{
						LED_state &= ~(OVERSPEED_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
				
					}
					rpm_sensor.alarmC2 = rpm_sensor.alarmC1 && (MUTE || rpm_sensor.alarmC2);
					rpm_sensor.buzzer1 = rpm_sensor.alarmC1 && !rpm_sensor.alarmC2;
					rpm_sensor.tripC1 = (rpm_sensor.trip && rpm_sensor.enable) || (rpm_sensor.tripC1 && (!RESET || !rpm100));
					
					
					rpm_sensor.tripC2 = rpm_sensor.tripC1 && (MUTE || rpm_sensor.tripC2);
					rpm_sensor.buzzer2 = rpm_sensor.tripC1 && !rpm_sensor.tripC2;
			
			
			
					common_trip = lop_sensor.tripC1 || rpm_sensor.tripC1;

					lockout = (!rpm500 && (lop_sensor.trip)  && lockout) || (rpm100 && lop_sensor_5 && !common_trip && !stop_timer_bit);
			
			
					start_timer_bit = (START || start_timer_bit) && (lockout);
		 
					if(lop_sensor.enable){
				
						LED_state |= (START_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						count_runtime = 1;
					}else{
						LED_state &= ~(START_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						count_runtime = 0;
					}
			
					stop_timer_bit = (STOP || stop_timer_bit || common_trip);
			
					if(lockout){
				
						PORTD |= (1<<LOCKOUT_RELAY_OUTPUT);
						LED_state |= (LOCKOUT_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
						
					}else{
						
						PORTD &= ~(1<<LOCKOUT_RELAY_OUTPUT);
						LED_state &= ~(LOCKOUT_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}

			
					if(start_timer_bit){
						PORTA |= (1<<START_OUTPUT);
				
					}else{
				
						PORTA &= ~(1<<START_OUTPUT);
				
					}
		
		
					if(stop_timer_bit){
						PORTA |= (1<<STOP_OUTPUT);
						LED_state |= (STOP_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}else{
						PORTA &= ~(1<<STOP_OUTPUT);
						LED_state &= ~(STOP_LED);
						MAX7219_displayLED(LED_state,MAX7219_DIGIT1);
					}
		
 
			

					if (lop_rawADC < LOP_STARTER_ENGAGE){
	
						lop_sensor_5= 1;
					}
					else{
	
						lop_sensor_5=0;
					}
			
			
					if(lop_sensor.buzzer1 || lop_sensor.buzzer2 || wt_sensor.buzzer || lot_sensor.buzzer || bv_sensor.buzzer || fuel_leak_sensor.buzzer || rpm_sensor.buzzer1 || rpm_sensor.buzzer2|| charge_fail_sensor.buzzer){
				
						BUZZER_PORT |= (1<<BUZZER_OUTPUT);
				
					}else{
				
						BUZZER_PORT &= ~(1<<BUZZER_OUTPUT);
					}

			
				}		
		
}
			
			



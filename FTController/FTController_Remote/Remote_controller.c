/*
 * Remote_controller_v1.c
 *
 * Created: 24-06-2015 11:17:56
 *  Author: ADMIN
 */ 

#include "Remote_controller.h"


volatile uint8_t update_flag = 0;
volatile uint8_t receive_buffer[24];

volatile uint8_t com_timeout;
volatile uint8_t i=0;
uint8_t MUTE = 0;

REMOTE_SENSOR lop_sensor;
REMOTE_SENSOR lot_sensor;
REMOTE_SENSOR wt_sensor;
REMOTE_SENSOR rpm_sensor;
REMOTE_SENSOR bv_sensor;
REMOTE_SENSOR fuel_leak_sensor;
REMOTE_SENSOR charge_fail_sensor;
REMOTE_SENSOR timeout;

inline void disable_timer1(){
	
	TCCR1B = 0x00;
	TIMSK &= ~(1<<OCIE1A);
	TCNT1 = 0;
}
inline void enable_timer1(uint16_t polling_rate){

	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);					 // enable Timer1 compare interrupt
	OCR1A = polling_rate;
	TCNT1 = 0;
	TCCR1B |= (1 << CS12)|(1 << CS10);		// 1/1024 prescaler
	
}
ISR(TIMER1_COMPA_vect){
	
	
	disable_timer1();
	com_timeout = 1;
	
}
ISR(USART_RX_vect) {
	
	
	receive_buffer[i++] = USART_readchar();
	if(receive_buffer[i-1] == FRAME_END){		// check if all data received on end of message
		
		if(i>= FRAME_SIZE){	
			i=0;
			update_flag = 1;
			
			enable_timer1(COM_TIMEOUT_PACKET);
			com_timeout = 0;
		}
	}
	else{
		
		update_flag = 0;
		enable_timer1(COM_TIMEOUT_FRAME);
		com_timeout = 0;
	}
	
}

void system_init(){
	
	_delay_ms(500);
	
	DDRD |= (1<<PORTD5);
	
	uint8_t j=0;
	for(j=0;j<FRAME_SIZE;j++){
		
		receive_buffer[j] = 0;
	}
	
	BUZZER_DIR |= (1<<BUZZER_RELAY); // Relay pin as output
	BUZZER_DIR &= ~(1<<BUZZER_SWITCH);	 // Mute pin as input
	//BUZZER_PORT |= (1<<BUZZER_SWITCH);
	
	USART_init(BAUDRATE);
	MAX7219_displayInit();
	
	DDRD |= (1<<LOCKOUT_LED);
	_delay_ms(500);
}

void test_display()
{
	uint8_t i=0;
	
	_delay_ms(500);
	
	for( i =0;i<4;i++){
		MAX7219_writeData(MAX7219_MODE_TEST,ON,i);
	}
	PORTD |= (1<<LOCKOUT_LED);
	
	_delay_ms(500);
	
	for(i =0;i<4;i++){
		MAX7219_writeData(MAX7219_MODE_TEST,OFF,i);
	}
	PORTD &= ~(1<<LOCKOUT_LED);
	
	_delay_ms(500);
	
}




int main(void)
{

		
		uint16_t rpm=0;
		uint16_t lube_oil_temp= 0;
		uint16_t water_temp = 0;
		uint16_t battery_vge = 0;
		uint16_t lube_oil_pressure = 0;
		uint16_t run_time = 0;
		uint16_t LED_state = 0;
		uint16_t buzzer_state = 0;
		uint16_t sensor_error_state =0;
		uint16_t received_crc = 0;
		uint16_t calc_crc=0;
		
		
		system_init();
		test_display();
		
		
		USART_enable(1,0);			// remote controller will always receive
		sei();
		enable_timer1(COM_TIMEOUT_INIT);
		
		while(1){
			
		
			if(update_flag){
				
					calc_crc = 0;
				
					rpm =				( (receive_buffer[0]<<8)|(receive_buffer[1]) );	
					water_temp =		( (receive_buffer[2]<<8)|(receive_buffer[3]) );	
					lube_oil_pressure = ( (receive_buffer[4]<<8)|(receive_buffer[5]) );
					lube_oil_temp =		( (receive_buffer[6]<<8)|(receive_buffer[7]) );
					battery_vge =		( (receive_buffer[8]<<8)|(receive_buffer[9]) );		
					run_time =          ( (receive_buffer[10]<<8)|(receive_buffer[11]) );	
					LED_state =         ( (receive_buffer[12]<<8)|(receive_buffer[13]) );
					buzzer_state =		( (receive_buffer[14]<<8)|(receive_buffer[15]) );
					sensor_error_state =( (receive_buffer[16]<<8)|(receive_buffer[17]) );
					received_crc =		( (receive_buffer[18]<<8)|(receive_buffer[19]) );	
				
					calc_crc = get_CRC(receive_buffer,FRAME_SIZE-3);
				
					if(calc_crc != received_crc){
						
						cli();
						
						LED_state = 0;
						LED_state |= (1<<COM_ERROR_LED);
						MAX7219_displayLED(LED_state);
						i = 0;
						uint8_t j=0;
						for(j=0;j<FRAME_SIZE;j++){
							
							receive_buffer[j] = 0;
						}
						update_flag = 0;
						com_timeout = 1;
						sei();
						
					}else{
						
						uint16_t tmp1 = 0;
						uint16_t tmp2 = 0;
						uint16_t tmp3 = 0;	
						uint16_t tmp4 = 0;
						
						receive_buffer[FRAME_SIZE-1] = 0; // every successful frame, remove frame end character
							
						LED_state &= ~(1<<COM_ERROR_LED);						
						com_timeout = 0;
							
						if(LED_state & REMOTE_FROM_LOCAL){
							LED_state |= (1<<REMOTE_STATE_LED);
						}
						else{
							LED_state &= ~(1<<REMOTE_STATE_LED);
						}
							
						if(LED_state & LOCKOUT_FROM_LOCAL){
							PORTD |= (1<<LOCKOUT_LED);
						}
						else{
							PORTD &= ~(1<<LOCKOUT_LED);
						}
						
						
						tmp1 = 0;	tmp2 = 0;	tmp3 = 0; tmp4 = 0;
						tmp1 = buzzer_state & LOP_ALARM_BUZZER_STATE;
						tmp2 = buzzer_state & LOP_TRIP_BUZZER_STATE;
						tmp3 = sensor_error_state&(1<<LOP_SENSOR_OFFSCALE_LOW);
						tmp4 = sensor_error_state&(1<<LOP_SENSOR_OFFSCALE_HIGH);	
						
						if(tmp1 || tmp3 || tmp4 ){
							lop_sensor.alarmC1 = 1;
						}else{
							lop_sensor.alarmC1 = 0;
						}
						if(tmp2){
							lop_sensor.tripC1 = 1;
						}else{
							lop_sensor.tripC1 = 0;
						}
						if(tmp3){
							MAX7219_displayError(LOP_SENSOR_OFFSCALE_LOW);	
						}else if(tmp4){
							MAX7219_displayError(LOP_SENSOR_OFFSCALE_HIGH);	
						}else{
							MAX7219_display7segment(lube_oil_pressure,LUBE_OIL_PRESSURE);
						}
						
						tmp1 = 0;	tmp2 = 0;	tmp3 = 0; tmp4 = 0;
						tmp1 = buzzer_state & OVERSPEED_ALARM_BUZZER_STATE;
						tmp2 = buzzer_state & OVERSPEED_TRIP_BUZZER_STATE;
						tmp3 = sensor_error_state&(1<<RPM_SENSOR_FAIL);
						
						if(tmp1 || tmp3){
							rpm_sensor.alarmC1 = 1;
						}else{
							rpm_sensor.alarmC1 = 0;
						}
						if(tmp2){
							rpm_sensor.tripC1 = 1;
						}else{
							rpm_sensor.tripC1 = 0;
						}
						if(tmp3){
							MAX7219_displayError(RPM_SENSOR_FAIL);
						}else{
							MAX7219_display7segment(rpm,RPM);
						}
						
						
						tmp1 = 0;	tmp2 = 0;	tmp3 = 0; tmp4 = 0;
						tmp1 = buzzer_state & LOT_ALARM_BUZZER_STATE;
						
						tmp3 = sensor_error_state&(1<<LOT_SENSOR_OFFSCALE_LOW);
						tmp4 = sensor_error_state&(1<<LOT_SENSOR_OFFSCALE_HIGH);
						
						if(tmp1 || tmp3 || tmp4 ){
							lot_sensor.alarmC1 = 1;
						}else{
							lot_sensor.alarmC1 = 0;
						}
						
						if(tmp3){
							MAX7219_displayError(LOT_SENSOR_OFFSCALE_LOW);
						}else if(tmp4){
							MAX7219_displayError(LOT_SENSOR_OFFSCALE_HIGH);
						}else{
							MAX7219_display7segment(lube_oil_temp,LUBE_OIL_TEMP);
						}
						
						
						tmp1 = 0;	tmp2 = 0;	tmp3 = 0; tmp4 = 0;
						tmp1 = buzzer_state & WT_ALARM_BUZZER_STATE;
						
						tmp3 = sensor_error_state&(1<<WT_SENSOR_OFFSCALE_LOW);
						tmp4 = sensor_error_state&(1<<WT_SENSOR_OFFSCALE_HIGH);
						
						if(tmp1 || tmp3 || tmp4 ){
							wt_sensor.alarmC1 = 1;
						}else{
							wt_sensor.alarmC1 = 0;
						}
						
						if(tmp3){
							MAX7219_displayError(WT_SENSOR_OFFSCALE_LOW);
						}else if(tmp4){
							MAX7219_displayError(WT_SENSOR_OFFSCALE_HIGH);
						}else{
							MAX7219_display7segment(water_temp,WATER_TEMP);
						}
						
						
						tmp1 = 0;	tmp2 = 0;	tmp3 = 0; tmp4 = 0;
						tmp1 = buzzer_state & BV_ALARM_BUZZER_STATE;
						tmp3 = sensor_error_state&(1<<BV_SENSOR_FAIL);
						
						if(tmp1 || tmp3 ){
							bv_sensor.alarmC1 = 1;
						}else{
							bv_sensor.alarmC1 = 0;
						}
						
						if(tmp3){
							MAX7219_displayError(BV_SENSOR_FAIL);
						}else{
							MAX7219_display7segment(battery_vge,BATTERY_VGE);
						}
						
						if(buzzer_state & FL_ALARM_BUZZER_STATE){
							fuel_leak_sensor.alarmC1 = 1;
						}else{
							fuel_leak_sensor.alarmC1 = 0;
						}
						MAX7219_displayLED(LED_state);
						MAX7219_display7segment(run_time,RUN_TIME);
						update_flag = 0;
						
					}
					
					
			}
			
			if(!(BUZZER_INPUT & (1<<BUZZER_SWITCH))){		// buzzer will be muted only on remote mute 
				MUTE = 1;
			}else{
				MUTE = 0;
			}
			
			
			timeout.alarmC2 = com_timeout && (MUTE ||(timeout.alarmC2)); 
			timeout.buzzer1 = com_timeout && !timeout.alarmC2;
			
			if(com_timeout){
					
					LED_state |= (1<<COM_ERROR_LED);
					MAX7219_displayLED(LED_state);
					i = 0;
					uint8_t j=0;
					for(j=0;j<FRAME_SIZE;j++){
						
						receive_buffer[j] = 0;
					}
					update_flag = 0;
			}else{
				
					LED_state &= ~(1<<COM_ERROR_LED);
			}
			lop_sensor.alarmC2 = (lop_sensor.alarmC1) && (MUTE || (lop_sensor.alarmC2));
			lop_sensor.buzzer1 = (lop_sensor.alarmC1) && !(lop_sensor.alarmC2);
			lop_sensor.tripC2 = (lop_sensor.tripC1) && ((MUTE) || (lop_sensor.tripC2));
			lop_sensor.buzzer2 = ((lop_sensor.tripC1) && !(lop_sensor.tripC2));
			
			rpm_sensor.alarmC2 = rpm_sensor.alarmC1 && (MUTE || rpm_sensor.alarmC2);
			rpm_sensor.buzzer1 = rpm_sensor.alarmC1 && !rpm_sensor.alarmC2;
			rpm_sensor.tripC2 = rpm_sensor.tripC1 && (MUTE || rpm_sensor.tripC2);
			rpm_sensor.buzzer2 = rpm_sensor.tripC1 && !rpm_sensor.tripC2;
			
			lot_sensor.alarmC2 = (lot_sensor.alarmC1) && (MUTE || (lot_sensor.alarmC2));
			lot_sensor.buzzer1 = (lot_sensor.alarmC1) && !(lot_sensor.alarmC2);
			
			wt_sensor.alarmC2 = (wt_sensor.alarmC1) && (MUTE || (wt_sensor.alarmC2));
			wt_sensor.buzzer1 = (wt_sensor.alarmC1) && !(wt_sensor.alarmC2);
			
			bv_sensor.alarmC2 = (bv_sensor.alarmC1) && (MUTE || (bv_sensor.alarmC2));
			bv_sensor.buzzer1 = (bv_sensor.alarmC1) && !(bv_sensor.alarmC2);
			
			fuel_leak_sensor.alarmC2 = fuel_leak_sensor.alarmC1	&& (MUTE || fuel_leak_sensor.alarmC2);
			fuel_leak_sensor.buzzer1 = fuel_leak_sensor.alarmC1 && !(fuel_leak_sensor.alarmC2);
			
			if(timeout.buzzer1 || lop_sensor.buzzer1 || lop_sensor.buzzer2 || wt_sensor.buzzer1 || lot_sensor.buzzer1 || bv_sensor.buzzer1 || fuel_leak_sensor.buzzer1 || rpm_sensor.buzzer1 || rpm_sensor.buzzer2){
				BUZZER_PORT |= (1<<BUZZER_RELAY);
			}else{
				BUZZER_PORT &= ~(1<<BUZZER_RELAY);	
			}
			
		}
		
	
}
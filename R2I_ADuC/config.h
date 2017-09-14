
// config file for R2I 


#define DELAY 250 						// 100 ms between ADC samples

// DAC theoretical min value = 655
// DAC theoretical max value = 3276
#define DAC_MIN 643						// actual Dac value to get 4 mA output
#define DAC_MAX 4096			  // actual Dac value to get 20 mA output

#define ADC_MIN 0
#define ADC_MAX_INIT 51200		// theoretical max adc val
#define ADC_SENSOR_OPEN 60000	// at sensor open, 5V input
#define THRESHHOLD 1312 			// 0.4 V threshhold for relay
#define HYSTERISIS 655				// hysterisis to prevent relay chatter 

#define EE_PAGE_ADDRESS 0x03

#define ON 0
#define OFF 1
sbit RELAY = P0^7;
sbit SET_MAX = P3^1;
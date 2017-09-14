/*
 * Config.h
 *
 * Created: 01/12/2016 15:37:08
 *  Author: ROHANP
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#define	Enter_Critical_Section		do { unsigned char sreg_ = SREG; cli();
#define	Leave_Critical_Section		SREG = sreg_; } while (0);

#define FREQ_MIN		( 60 )
#define FREQ_MAX		( 10000 )
#define RPM_MIN			( 0 )
#define RPM_SECONDS		( 60.0 )
#define RPM_RESET		( 60 )

//#define DIP_SW_PRESENT
#define DIP_SW_ABSENT

#define NUMBER_OF_TEETH ( 142 )
#define RPM_OVERSPEED	( 2160 )

#endif /* CONFIG_H_ */
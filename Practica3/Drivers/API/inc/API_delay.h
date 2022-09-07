
#ifndef __API_DELAY_H_
#define __API_DELAY_H_

/* Delay values*/
#define DELAY_LEDS		200		// [ms] time delay for leds
#define DELAY_LED_1		100		//	[ms] time delay for led 1
#define DELAY_LED_2		500		//	[ms] time delay for led 2
#define DELAY_LED_3		1000	//	[ms] time delay for led 3
#define MAX_DELAY		30000	//	[ms] max time for delay

/* Delay variables */
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
}delay_t;


/*  Prototypes */
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

#endif

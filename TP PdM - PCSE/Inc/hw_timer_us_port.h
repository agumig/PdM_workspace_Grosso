#ifndef __US_TIMER_PORT_H__
#define __US_TIMER_PORT_H__

#include "stdbool.h"
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include "stm32f4xx_hal_tim.h"

typedef bool bool_t;

#define MAX_TIMEOUT 100000

bool_t usTimerInit_port(void);
bool_t usTimerStart_port(void);
void usTimerStop_port(void);
void usTimerDelay_port(uint16_t delay_us);
void msTimerDelay_port(uint16_t delay_us);



#endif

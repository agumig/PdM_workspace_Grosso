#ifndef __US_TIMER_H__
#define __US_TIMER_H__

#include "main.h"


void usTimerInit(void);
void usTimerStart(void);
void usTimerStop(void);
void usTimerDelay(uint16_t delay_us);


#endif

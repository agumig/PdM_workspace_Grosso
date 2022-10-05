
#include "hw_timer_us.h"
#include "hw_timer_us_port.h"

static void Error_Handler(void);

void usTimerInit(void)
{
	if(usTimerInit_port() != true)
		Error_Handler();
}

void usTimerStart(void)
{
	usTimerStart_port();
}

void usTimerStop(void)
{
	usTimerStop_port();
}

void usTimerDelay(uint16_t delay_us)
{
	usTimerDelay_port(delay_us);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	while (1);
}





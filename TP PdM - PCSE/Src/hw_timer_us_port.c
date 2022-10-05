
#include "hw_timer_us_port.h"


static void Error_Handler(void);

static TIM_HandleTypeDef htim1;

bool_t usTimerInit_port(void)
{
	bool_t returnValue = false;

	htim1.Instance = TIM1;

	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0xFFFF;
	htim1.Init.Prescaler = 180;
	htim1.Init.RepetitionCounter = 0;

	 if(HAL_TIM_Base_Init(&htim1) == HAL_OK)
		 returnValue = true;

	 return returnValue;
}

void usTimerStart_port(void)
{
	HAL_TIM_Base_Start(&htim1);
}

void usTimerStop_port(void)
{
	HAL_TIM_Base_Stop(&htim1);
}

void usTimerDelay_port(uint16_t delay_us)
{
	uint32_t timeout = 0;

	__HAL_TIM_SET_COUNTER(&htim1, 0);

	while((__HAL_TIM_GET_COUNTER(&htim1) < delay_us) && timeout < MAX_TIMEOUT)
	{
		timeout++;
	}

	if(timeout >= MAX_TIMEOUT)
		Error_Handler();
}


static void Error_Handler(void)
{
	while (1);
}





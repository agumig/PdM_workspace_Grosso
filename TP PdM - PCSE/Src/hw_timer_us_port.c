
#include "hw_timer_us_port.h"


static TIM_HandleTypeDef htim1;


static void Error_Handler(void);


bool_t usTimerInit_port(void)
{
	bool_t returnValue = false;


	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim1.Instance = TIM1;

	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.Prescaler = 168;
	htim1.Init.RepetitionCounter = 0;


	if (HAL_TIM_Base_Init(&htim1) == HAL_OK)
	{
		returnValue = true;
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) == HAL_OK)
	{
		returnValue = true;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) == HAL_OK)
	{
		returnValue = true;

	}

	return returnValue;
}

bool_t usTimerStart_port(void)
{
	bool_t returnValue = false;

	if(HAL_TIM_Base_Start(&htim1) == HAL_OK)
	{
		returnValue = true;
	}

	return returnValue;
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

void msTimerDelay_port(uint16_t delay_us)
{
	HAL_Delay(delay_us);
}

static void Error_Handler(void)
{
	while (1);
}





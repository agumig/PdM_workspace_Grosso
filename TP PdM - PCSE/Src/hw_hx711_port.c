/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "hw_hx711_port.h"


#define HX711_CLK_GPIO_Port 	GPIOD
#define HX711_DATA_GPIO_Port 	GPIOD
/***	****************************************************************************
 * Functions
 ******************************************************************************/
/**
 * @brief   Chip HX711 Init
 * @details GPIO init.
 * @return 	none
 */
void HX711_init_port(void)
{
	GPIO_InitTypeDef gpioConfig;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(HX711_CLK_GPIO_Port, HX711_CLK, GPIO_PIN_RESET);


	gpioConfig.Pin = HX711_DATA;
	gpioConfig.Mode = GPIO_MODE_INPUT;
	gpioConfig.Pull = GPIO_NOPULL;
	gpioConfig.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(HX711_DATA_GPIO_Port, &gpioConfig);

	gpioConfig.Pin = HX711_CLK;
	gpioConfig.Mode = GPIO_MODE_OUTPUT_PP;
	gpioConfig.Pull = GPIO_NOPULL;
	gpioConfig.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(HX711_CLK_GPIO_Port, &gpioConfig);

	usTimerInit();
	usTimerStart();
}

/**
 * @brief   Set CLK GPIO in low level
 * @return 	none
 */
void HX711_CLK_set_low(void)
{
	HAL_GPIO_WritePin (HX711_CLK_GPIO_Port, HX711_CLK, GPIO_PIN_RESET);
}

/**
 * @brief   Set CLK GPIO in high level
 * @return 	none
 */
void HX711_CLK_set_high(void)
{
	HAL_GPIO_WritePin (HX711_CLK_GPIO_Port, HX711_CLK, GPIO_PIN_SET);
}

/**
 * @brief   Get DATA GPIO level
 * @return 	uint8_t Data pin level
 */
uint8_t HX711_get_level(void)
{
	return HAL_GPIO_ReadPin (HX711_DATA_GPIO_Port, HX711_DATA);

}

/**
 * @brief   HX711 delay
 * @param   uint32_t delay [us]
 * @return 	none
 */
void HW711_delay(uint16_t delay)
{
	//HAL_Delay(delay);
	usTimerDelay(delay);
}


void HW711_msDelay(uint16_t delay)
{
	msTimerDelay(delay);
}
/**
 * @brief   Disable interruptions
 * @return 	none
 */
void HX711_interrupts_enable(void)
{
	//portENABLE_INTERRUPTS();
}

/**
 * @brief   Enable interruptions
 * @return 	none
 */
void HX711_interrupts_disable(void)
{
	//portDISABLE_INTERRUPTS();
}

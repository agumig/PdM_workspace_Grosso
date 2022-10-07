/**
 ******************************************************************************
 * @file    UART/UART_Printf/Src/main.c
 * @author  MCD Application Team
 * @brief   This example shows how to retarget the C library printf function
 *          to the UART.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_debounce.h"
#include "API_delay.h"
#include "API_uart.h"
#include "hw_hx711.h"
#include "hw_timer_us.h"

/** @addtogroup STM32F4xx_HAL_Examples
 * @{
 */

/** @addtogroup UART_Printf
 * @{
 */

/* Private typedef -----------------------------------------------------------*/

// States of MAIN Fine State Machine
typedef enum{
	IDLE,
	FILL,
	EROGATION
}FSM_State_t;

/* Private define ------------------------------------------------------------*/
//#define CUBEMX_TEST
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static bool_t pumpErogationState = false, pumpFillState = false;
static bool_t valveErogationState = false, valveFillState = false;

typedef enum{
	ErogationPump,
	ErogationValve,
	FillPump,
	FillValve
}Output_t;
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

static void valveSet(Output_t valve, bool_t state);
static void pumpSet(Output_t pump, bool_t state);
static void Valves_Init(void);
static void Pump_Init(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	FSM_State_t state = IDLE;
	double reservoirWeight = 0;
	bool_t erogationButton = false;

	/* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Initialize BSP Led */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);


	/* Initialize BSP PB for BUTTON_USER */
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Initialize anti-debounce */
	debounceFSM_init();

	if(!uartInit())
		Error_Handler();

	/* For calibration it's necessary initializing the IC and reading the TARE value.
	 * After that we need to set HX711_REAL_OFFSET (in hw_hx711.h) with the TARE value*/
#ifdef _CALIBRATION__
	HW_HX711_Init_Tare();
	HX711_get_offset();
#endif

#ifndef _CALIBRATION__
	/* Initialize */
	HW_HX711_Init();
#endif

	Pump_Init();	// todo: Create a folder for HW_PUMP
	Valves_Init();	// todo: Create a folder for HW_VALVE

	/* Infinite loop */
	while (1)
	{

		debounceFSM_update();

		reservoirWeight = HX711_get_units(WEIGHT_READINGS);
		erogationButton = readKeyPressed();		// Button User was pressed

		switch(state)
		{
		case IDLE:
			if(RESERVORY_LIMIT_MIN > reservoirWeight)
			{
				state = FILL;
			}else
			{
				if(erogationButton)
				{
					state = EROGATION;
				}
			}

			break;
		case FILL:

			if(erogationButton && (RESERVORY_LIMIT_MIN < reservoirWeight))
			{
				state = EROGATION;

				if(valveFillState)
					valveSet(FillValve, false);
				if(pumpFillState)
					pumpSet(FillPump,false);
			}else
			{
				if(RESERVORY_LIMIT_MAX > reservoirWeight)
				{
					if(!valveFillState)
						valveSet(FillValve, true);
					if(!pumpFillState)
						pumpSet(FillPump,true);
				}else
				{
					if(valveFillState)
						valveSet(FillValve, false);
					if(pumpFillState)
						pumpSet(FillPump,false);

					state = IDLE;
				}
			}
			break;
		case EROGATION:
			if(erogationButton && (RESERVORY_LIMIT_MIN < reservoirWeight))
			{
				if(!valveErogationState)
					valveSet(ErogationValve, true);
				if(!pumpErogationState)
					pumpSet(ErogationPump,true);
			}
			else
			{
				if(valveErogationState)
					valveSet(ErogationValve, false);
				if(pumpErogationState)
					pumpSet(ErogationPump,false);

				if(RESERVORY_LIMIT_MIN > reservoirWeight)
				{
					state = FILL;
				}
				else
				{
					if(!erogationButton)
						state = IDLE;
				}
			}
			break;
		default:
			Error_Handler();
			break;
		}
	}
}


static void Pump_Init(void)
{
	pumpSet(FillPump, false);
	pumpSet(ErogationPump, false);
}

static void Valves_Init(void)
{
	valveSet(FillValve, false);
	valveSet(ErogationValve, false);
}

static void pumpSet(Output_t pump, bool_t state)
{
	if(ErogationPump == pump)
	{
		uartSendString(BOMBA_EROGACION);
		pumpErogationState = state;
	}else
	{
		if(FillPump == pump)
		{
			uartSendString(BOMBA_LLENADO);
			pumpFillState = state;
		}

	}
	if(true == state)
	{
		uartSendString(ENDEDIDO);
	}
	else
	{
		uartSendString(APAGADO);
	}

}

static void valveSet(Output_t valve, bool_t state)
{
	if(ErogationValve == valve)
	{
		uartSendString(VALVULA_EROGACION);
		valveErogationState = state;
	}else
	{
		if(FillValve == valve)
		{
			uartSendString(VALVULA_LLENADO);
			valveFillState = state;
		}

	}
	if(true == state)
	{
		uartSendString(ENDEDIDO);
	}
	else
	{
		uartSendString(APAGADO);
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
#ifndef CUBEMX_TEST
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
#endif
#ifdef CUBEMX_TEST

	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	  {
	    Error_Handler();
	  }
#endif
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    TP PdM - PCSE/Inc/hw_hx711_port.h
  * @brief   Header for hw_hx711_port.c module
  ******************************************************************************
*/
#ifndef _HW_HX711_PORT_H__
#define _HW_HX711_PORT_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <API_timer.h>
#include "main.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

#define CELDA_CARGA_DATA   	GPIO_PIN_7	// Next to A0 in NUCLEO
#define CELDA_CARGA_CLK    	GPIO_PIN_6	// Next to A1 in NUCLEO

#define HX711_DATA			CELDA_CARGA_DATA
#define HX711_CLK			CELDA_CARGA_CLK

#define HX711_DATA_MASK  	HX711_DATA
#define HX711_CLK_MASK    	HX711_CLK

#define HX711_CLK_LOW   	0
#define HX711_CLK_HIGH  	1
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void HX711_initPort(void);
void HX711_CLK_setLow(void);
void HX711_CLK_setHigh(void);
uint8_t HX711_getLevel(void);
void HW711_delay(uint16_t delay);
void HW711_msDelay(uint16_t delay);
void HX711_interruptsEnable(void);
void HX711_interruptsDisable(void);

#endif

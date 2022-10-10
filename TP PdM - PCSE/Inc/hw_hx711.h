/**
  ******************************************************************************
  * @file    TP PdM - PCSE/Inc/hw_hx711.h
  * @brief   Header for hw_hx711..c module
  ******************************************************************************
*/
#ifndef __HW_HX711_H__
#define __HW_HX711_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "main.h"
#include <math.h>

/*******************************************************************************
 * Typedef
 ******************************************************************************/
/**
 * eGAIN_128 correspons to Channel A
 * eGAIN_32 correspons to Channel B
 * eGAIN_64 correspons to Channel A
*/
typedef enum HX711_GAIN
{
	eGAIN_128 = 1,		// 1 pulso pos LSB de la trama (pulso 25 de CLK)
	eGAIN_32 = 2,		// 2 pulsos pos LSB de la trama (pulso 26 de CLK)
	eGAIN_64 = 3,		// 3 pulsos pos LSB de la trama (pulso 27 de CLK)
}HX711_GAIN;

typedef bool bool_t;
/*******************************************************************************
 * Defines
 ******************************************************************************/

#define HX711_ADC_PRECISION			24			// Bits of precision
#define ADC_MIN_SATURATION			0x800000	// Output data saturation value (MIN)
#define ADC_MAX_SATURATION			0x7FFFFF	// Output data saturation value (MAX)

#define POSITIVE_MASK_32_T			0x00FFFFFF
#define NEGATIVE_MASK_32_T			0xFF000000

#define HX711_ZERO_OFFSET   		0
#define HX711_REAL_OFFSET			0x0000B395
#define HX711_REAL_CA2_OFFSET		(0xFF800000 ^ HX711_REAL_OFFSET)

#define BIT_MGR_CONVERTION			0.02845274				//[mg] 1 count equal to 1.192092967 mili gram
#define HX711_SCALE_GR         		1						// Gramos = 1000. Kilogramos = 1000000.
#define HX711_SCALE_KG         		(HX711_SCALE_GR*1000)	// Gramos = 1000. Kilogramos = 1000000.

#define HX711_SCALE					HX711_SCALE_GR

#define CLOCK_DELAY_US      		20		// [us] MIN 0.1  TYP 1 MAX 50
#define CLOCK_POWER_DOWN_DELAY_US 	60		// [us] Time to stay high to turn off
#define WAIT_FOR_READY_DELAY		10		// [ms] Time to wait for chip ready
#define MAX_WAITING_FOR_READY		100		// [ms] Maximun time waiting for ready

#define MAX_TIMES_ALLOWED			20		// How many reading are allowed
#define READINGS_TO_GET_TARE		20		// How many readings to do

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void HX711_Init(void);
void HX711_Init_Tare(void);
void HX711_setGain(HX711_GAIN);
void HX711_setScale(float scale);
float HX711_getScale();
void HX711_tare(uint8_t times);
void HX711_setOffset(int32_t offset);
int32_t HX711_getOffset();
uint32_t HX711_getValue(uint8_t times);
double HX711_getUnits(uint8_t times);
void HX711_powerDown();
void HX711_powerUp();


#endif

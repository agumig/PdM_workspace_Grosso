#ifndef __HW_HX711_H__
#define __HW_HX711_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "main.h"

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

#define HX711_ZERO_OFFSET   		0
#define HX711_REAL_OFFSET			585

#define HX711_SCALE_GR         		1		// Gramos = 1. Kilogramos = 1000.
#define HX711_SCALE_KG         		1000	// Gramos = 1. Kilogramos = 1000.

#define HX711_SCALE					HX711_SCALE_GR

#define CLOCK_DELAY_US      		20		// [us] MIN 0.1  TYP 1 MAX 50
#define CLOCK_POWER_DOWN_DELAY_US 	60		// [us] Time to stay high to turn off
#define WAIT_FOR_READY_DELAY		10000	// [us] Time to wait for chip ready

#define READINGS_TO_GET_TARE		20		// How many readings to do

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void HW_HX711_Init(void);
void HW_HX711_Init_Tare(void);
void HX711_set_gain(HX711_GAIN);
void HX711_set_scale(float scale);
float HX711_get_scale();
void HX711_tare(uint8_t times);
void HX711_set_offset(uint64_t offset);
uint64_t HX711_get_offset();
double HX711_get_value(uint8_t times);
double HX711_get_units(uint8_t times);
void HX711_power_down();
void HX711_power_up();


#endif

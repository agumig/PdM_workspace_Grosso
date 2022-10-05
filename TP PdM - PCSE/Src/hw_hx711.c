/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "hw_hx711.h"
#include "hw_hx711_port.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define TAG  "HX711"

/*******************************************************************************
 * Private functions prototypes
 ******************************************************************************/
static bool_t HX711_is_ready();
static uint64_t HX711_read();
static double  HX711_read_average(uint8_t times) ;

/*******************************************************************************
 * Global private variables
 ******************************************************************************/
static HX711_GAIN Gain;		// Amplification factor
static uint64_t Offset;		// Used for tare weight
static float Scale;	       	// Used to return weight in grams, kg, ounces...

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief   Chip HX711 Init
 * @details GPIO init. Set gain. Set tare
 * @return 	void
 */
void HW_HX711_Init(void)
{
    HX711_init_port();

	HX711_set_scale(HX711_SCALE_GR);
	HX711_set_gain(eGAIN_128);
	HX711_set_offset(HX711_REAL_OFFSET);
}


/**
 * @brief   Chip HX711 Init
 * @details GPIO init. Set gain. Calculate and set tare (calibration)
 * @return 	void
 */
void HW_HX711_Init_Tare(void)
{
    HX711_init_port();

	HX711_set_scale(HX711_SCALE_GR);
	HX711_set_gain(eGAIN_128);
	HX711_set_offset(HX711_ZERO_OFFSET);
    HX711_tare(READINGS_TO_GET_TARE);		// Set the offset. At production time, you must hardcode the tare
}

/**
 * @brief   Set the gain factor
 * @details Channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
 * 			Depending on the parameter, the channel is also set to either A or B
 * @param 	HX711_GAIN gain
 * @return 	void
 */
void HX711_set_gain(HX711_GAIN gain)
{
	Gain = gain;
	HX711_read();		// It's necessary doing a reading to set the gain
}

/**
 * @brief   Get calibrated weight
 * @details Times = how many readings to do
 * @param   uint8_t times
 * @return 	Read_average() - OFFSET
 */
double HX711_get_value(uint8_t times)
{
	double avg = 0, returnValue = 0;

	avg = HX711_read_average(times);

	if(avg > Offset)
		returnValue = avg - Offset;

	return returnValue;
}

/**
 * @brief   Get converted weight
 * @details Times = how many readings to do
 * @param   uint8_t times
 * @return 	Value divided by scale
 */
double HX711_get_units(uint8_t times)
{
	return HX711_get_value(times) / Scale;
}

/**
 * @brief   Set the OFFSET value for tare weight
 * @details Times = how many times to read the tare value
 * @param   uint8_t times
 */
void HX711_tare(uint8_t times)
{
	uint64_t sum = 0;

	//DEBUGOUT(TAG, "Start Tare");	//TODO

	sum = HX711_read_average(times);
	HX711_set_offset(sum);

	//DEBUGOUT(TAG, "End Tare: %llu ",sum);	//TODO
}

/**
 * @brief   Set new offset
 * @details Set the value that's subtracted from the actual reading (tare weight)
 * @param   uint64_t offset
 */
void HX711_set_offset(uint64_t offset)
 {
	Offset = offset;
}

/**
 * @brief   Get the current OFFSET
 * @return	Current offset
 */
uint64_t HX711_get_offset(void)
{
	return Offset;
}

/**
 * @brief   Set new scale value
 * @details This value is used to convert the raw data to "human readable" data (measure units)
 * @param   float scale
 */
void HX711_set_scale(float scale )
{
	Scale = scale;
}

/**
 * @brief   Get the current SCALE
 * @return	Current scale
 */
float HX711_get_scale()
 {
	return Scale;
}

/**
 * @brief   Puts the chip into power down mode
 * @details When PD_SCK pin changes from low to high and stays at high for longer than 60 us , HX711 enters power down mode
 */
void HX711_power_down()
{
	HX711_CLK_set_low();
	HW711_delay(CLOCK_DELAY_US);
	HX711_CLK_set_high();
	HW711_delay(CLOCK_POWER_DOWN_DELAY_US);
}

/**
 * @brief   Wakes up the chip after power down mode
 * @details When PD_SCK returns to low, chip wakes up
 */
void HX711_power_up()
{
	HX711_CLK_set_low();
}

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/**
 * @brief   Check if HX711 is ready
 * @details When output data is not ready for retrieval, digital output pin DOUT is high.
 * 			Serial clock should be low. When DOUT goes to low, it indicates data is ready for retrieval.
 * @return 	True if chip is ready, false if not.
 */
static bool_t HX711_is_ready()
{
	HX711_CLK_set_low();

	if(HX711_get_level())
		return false;
	else
		return true;
}

/**
 * @brief   Get weight
 * @details Waits for the chip to be ready and returns a reading
 * @return 	Weight readed
 */
static uint64_t HX711_read()
{
	uint64_t value = 0;
	uint32_t i = 0;

	HX711_CLK_set_low();

	// Wait for GPIO DATA low level
	while (!HX711_is_ready())	// todo: que no sea bloqueante
	{
		HW711_delay(WAIT_FOR_READY_DELAY);
	}

	HX711_interrupts_disable();	// Enter critical section

	for( i = 0; i < HX711_ADC_PRECISION ; i++)
	{
		HX711_CLK_set_high();
		HW711_delay(CLOCK_DELAY_US);
        value = value << 1;
		HX711_CLK_set_low();
        HW711_delay(CLOCK_DELAY_US);

        if(HX711_get_level())
        	value++;
	}

	// Set the channel and the gain factor for the next reading using the clock pin
	for ( i = 0; i < Gain; i++)
	{
		HX711_CLK_set_high();
		HW711_delay(CLOCK_DELAY_US);
		HX711_CLK_set_low();
		HW711_delay(CLOCK_DELAY_US);
	}

	HX711_interrupts_enable();	// Exit critical section

	//DEBUGOUT(TAG, "Value: %llu", value);	//TODO

	value =value^0x800000;

	return (value);
}


/**
 * @brief   Get an average weight
 * @details Times = how many readings to do
 * @param   uint8_t times
 * @return 	Average reading
 */
static double  HX711_read_average(uint8_t times)
{
	double sum = 0;
	uint8_t i = 0;

	//DEBUGOUT(TAG, "Read Average Start"); //TODO

	for (i = 0; i < times; i++)
	{
		sum += HX711_read();
	}

	//DEBUGOUT(TAG, "Read Average End : %lf",(sum / times));	//TODO

	return sum / times;
}

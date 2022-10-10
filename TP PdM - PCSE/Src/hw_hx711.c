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
static bool_t HX711_isReady();
static uint64_t HX711_read();
static uint64_t  HX711_readAverage(uint8_t times);
static uint32_t complementA2toInt(uint32_t data, uint8_t bits);
static bool_t isNegative(uint32_t data, uint8_t bits);

/*******************************************************************************
 * Global private variables
 ******************************************************************************/
static HX711_GAIN Gain;		// Amplification factor
static int32_t Offset;		// Used for tare weight
static float Scale;	       	// Used to return weight in grams, kg, ounces...

/*******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * @brief   Chip HX711 Init
 * @details GPIO init. Set gain. Set tare
 * @return 	void
 */
void HX711_Init(void)
{
	HX711_initPort();

	HX711_setScale(HX711_SCALE_GR);
	HX711_setGain(eGAIN_128);
	HX711_setOffset(HX711_REAL_CA2_OFFSET);
}


/**
 * @brief   Chip HX711 Init
 * @details GPIO init. Set gain. Calculate and set tare (calibration)
 * @return 	void
 */
void HX711_Init_Tare(void)
{
	HX711_initPort();

	HX711_setScale(HX711_SCALE_GR);
	HX711_setGain(eGAIN_128);
	HX711_setOffset(HX711_ZERO_OFFSET);
	HX711_tare(READINGS_TO_GET_TARE);		// Set the offset. At production time, you must hardcode the tare
}

/**
 * @brief   Set the gain factor
 * @details Channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
 * 			Depending on the parameter, the channel is also set to either A or B
 * @param 	HX711_GAIN gain
 * @return 	void
 */
void HX711_setGain(HX711_GAIN gain)
{
	if(eGAIN_128 == gain || eGAIN_32 == gain || eGAIN_64 == gain)
	{
		Gain = gain;
		HX711_read();		// It's necessary doing a reading to set the gain
	}
}

/**
 * @brief   Get calibrated weight
 * @details Times = how many readings to do
 * @param   uint8_t times
 * @return 	Read_average() - OFFSET
 */
uint32_t HX711_getValue(uint8_t times)
{
	int32_t avg = 0, counts = 0, returnValue = 0;

	counts = HX711_readAverage(times);

	if(isNegative(counts, HX711_ADC_PRECISION))
	{
		avg = NEGATIVE_MASK_32_T | counts;	// Complete negative bits
		if(avg < Offset)
			returnValue = (Offset - avg);
	}
	else
	{
		avg = POSITIVE_MASK_32_T & counts;	// Complete positive bits

		returnValue = (ADC_MAX_SATURATION - avg) + HX711_REAL_OFFSET;
	}

	return returnValue;
}

/**
 * @brief   Get converted weight
 * @details Times = how many readings to do
 * @param   uint8_t times
 * @return 	Value divided by scale
 */
double HX711_getUnits(uint8_t times)
{
	return ((BIT_MGR_CONVERTION) *(HX711_getValue(times)/Scale));
}

/**
 * @brief   Set the OFFSET value for tare weight
 * @details Times = how many times to read the tare value
 * @param   uint8_t times
 */
void HX711_tare(uint8_t times)
{
	uint64_t sum = 0;

	sum = HX711_readAverage(times);
	HX711_setOffset(sum);
}

/**
 * @brief   Set new offset
 * @details Set the value that's subtracted from the actual reading (tare weight)
 * @param   uint64_t offset
 */
void HX711_setOffset(int32_t offset)
{
		Offset = offset;
}

/**
 * @brief   Get the current OFFSET
 * @return	Current offset
 */
int32_t HX711_getOffset(void)
{
	return Offset;
}

/**
 * @brief   Set new scale value
 * @details This value is used to convert the raw data to "human readable" data (measure units)
 * @param   float scale
 */
void HX711_setScale(float scale )
{
	if((HX711_SCALE_GR == scale) || (HX711_SCALE_KG == scale))
		Scale = scale;
}

/**
 * @brief   Get the current SCALE
 * @return	Current scale
 */
float HX711_getScale()
{
	return Scale;
}

/**
 * @brief   Puts the chip into power down mode
 * @details When PD_SCK pin changes from low to high and stays at high for longer than 60 us , HX711 enters power down mode
 */
void HX711_powerDown()
{
	HX711_CLK_setLow();
	HW711_delay(CLOCK_DELAY_US);
	HX711_CLK_setHigh();
	HW711_delay(CLOCK_POWER_DOWN_DELAY_US);
}

/**
 * @brief   Wakes up the chip after power down mode
 * @details When PD_SCK returns to low, chip wakes up
 */
void HX711_powerUp()
{
	HX711_CLK_setLow();
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
static bool_t HX711_isReady()
{
	HX711_CLK_setLow();

	if(HX711_getLevel())
		return false;
	else
		return true;
}

/**
 * @brief   Get weight
 * @details Waits for the chip to be ready and returns a reading
 * @return 	Weight read
 */
static uint64_t HX711_read()
{
	uint64_t value = 0;
	uint32_t i = 0;

	HX711_CLK_setLow();

	// Wait for GPIO DATA low level
	while (!HX711_isReady() && i < (MAX_WAITING_FOR_READY/WAIT_FOR_READY_DELAY))
	{
		HW711_msDelay(WAIT_FOR_READY_DELAY);
		i++;
	}

	if(i >= (MAX_WAITING_FOR_READY/WAIT_FOR_READY_DELAY))	// Chip didn't answer
		errorHandler();

	HX711_interruptsDisable();	// Enter critical section

	for( i = 0; i < HX711_ADC_PRECISION ; i++)
	{
		HX711_CLK_setHigh();
		HW711_delay(CLOCK_DELAY_US);
		value = value << 1;
		HX711_CLK_setLow();
		HW711_delay(CLOCK_DELAY_US);

		if(HX711_getLevel())
			value++;
	}

	// Set the channel and the gain factor for the next reading using the clock pin
	for ( i = 0; i < Gain; i++)
	{
		HX711_CLK_setHigh();
		HW711_delay(CLOCK_DELAY_US);
		HX711_CLK_setLow();
		HW711_delay(CLOCK_DELAY_US);
	}

	HX711_interruptsEnable();	// Exit critical section

	value =value^0x800000;	// Value is in A2 complement but without sign?

	return (value);
}


/**
 * @brief   Get an average weight
 * @details Times = how many readings to do
 * @param   uint8_t times
 * @return 	Average reading
 */
static uint64_t  HX711_readAverage(uint8_t times)
{
	uint64_t sum = 0;
	uint8_t i = 0;

	if(times > MAX_TIMES_ALLOWED)
		times = MAX_TIMES_ALLOWED;

	//HX711_powerUp();

	for (i = 0; i < times; i++)
	{
		sum += HX711_read();
	}
	//HX711_powerDown();

	return (sum/times);
}

/**
 * @brief   Analize if a nunmber in A2 complement is a negative number
 * @param   uint32_t data: 	A2 complement number
 * @param	uint8_t bits:	number bits
 * @return 	True if it's a negative number, false if not
 */
static bool_t isNegative(uint32_t data, uint8_t bits)
{
	bool_t isNegative = (data & (1 << (bits-1))) != 0;

	return isNegative;
}

/**
 * @brief   Convert A2 complement number to integer
 * @param   uint32_t data: 	A2 complement number
 * @param	uint8_t bits:	number bits
 * @return 	Converted number or -1 if an error happened
 */
static uint32_t complementA2toInt(uint32_t data, uint8_t bits)
{
	bool_t isNegativeNumber;
	uint32_t positiveValue, returnValue;
	uint32_t range;

	isNegativeNumber = isNegative(data, bits);
	if(bits > 32)
	{
		returnValue = -1;
	}else
	{
	range = pow(BINARY_BASE,bits);

	if (isNegativeNumber)
		positiveValue = range - data;	// C_{A2}=2^{n}-N
	else
		positiveValue = data;

	returnValue = positiveValue;
	}

	return returnValue;
}

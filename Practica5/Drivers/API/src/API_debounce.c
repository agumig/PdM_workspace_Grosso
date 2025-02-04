/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "API_debounce.h"

/*******************************************************************************
 * Private Typedef
 ******************************************************************************/
typedef enum{	// States of Fine State Machine
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING
}debounceState_t;

/*******************************************************************************
 * Private function prototypes
 ******************************************************************************/
static void Error_Handler(void);	// Error state function

/*******************************************************************************
 * Private global variables
 ******************************************************************************/
static delay_t delayButton;
static debounceState_t currentState;
static bool_t fallingEdge, risingEdge;

/*******************************************************************************
 * Public functions
 ******************************************************************************/
/*!
 * @brief   Anti-debounce Initialization
 * @details	Set the initial state
 * @param 	none
 * @return 	none
 */
void debounceFSM_init(void)
{
	delayInit(&delayButton, BUTTON_DEBOUNCE_DELAY);
	currentState = BUTTON_UP;
	fallingEdge = false;
}

/*!
 * @brief   Anti-debounce State Machine
 * @details Read the inputs, transition the states.
 * 			Must be called frequently.
 * @param 	none
 * @return 	none
 */
void debounceFSM_update()
{
	switch(currentState){
			case BUTTON_UP:
				if(BSP_PB_GetState(BUTTON_USER))
				{
					currentState = BUTTON_FALLING;
					delayRead(&delayButton);		// Set the initial value of the delay
				}

				break;
			case BUTTON_FALLING:
				if(delayRead(&delayButton))
				{
					if(BSP_PB_GetState(BUTTON_USER))
					{
						currentState = BUTTON_DOWN;
						fallingEdge = true;
					}
					else
					{
						currentState = BUTTON_UP;
					}
				}
				break;
			case BUTTON_DOWN:
				if(!BSP_PB_GetState(BUTTON_USER))
				{
					currentState = BUTTON_RAISING;
					delayRead(&delayButton);		// Set the initial value of the delay
				}
				break;
			case BUTTON_RAISING:
				if(delayRead(&delayButton))
				{
					if(!BSP_PB_GetState(BUTTON_USER))
					{
						currentState = BUTTON_UP;
						risingEdge = true;
					}
					else
					{
						currentState = BUTTON_DOWN;
					}
				}
				break;
			default:
				Error_Handler();
				break;
			}
}

/*!
 * @brief   Read button state.
 * @param 	none
 * @return	True if button was pressed, False if not.
 */
bool_t readKey(void)
{
	bool_t returnValue = false;

	if(fallingEdge)
	{
		fallingEdge = false;
		returnValue = true;
	}

	return returnValue;
}

/*!
 * @brief   Read button state.
 * @param 	none
 * @return	True if button was released, False if not.
 */
bool_t readKeyReleased(void)
{
	bool_t returnValue = false;

	if(risingEdge)
	{
		risingEdge = false;
		returnValue = true;
	}

	return returnValue;
}
/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @return None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

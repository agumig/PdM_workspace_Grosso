#include "API_debounce.h"

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING
}debounceState_t;

static void Error_Handler(void);	// Error state function
static void buttonPressed(void);    // must toggle LED1.
static void buttonReleased(void);	// must toggle LED3.


static delay_t delayButton;
static debounceState_t currentState;
static bool_t fallingEdge;


void debounceFSM_init(void)
{
	delayInit(&delayButton, BUTTON_DEBOUNCE_DELAY);
	currentState = BUTTON_UP;
}


void debounceFSM_update()
{
	switch(currentState){
			case BUTTON_UP:
				if(BSP_PB_GetState(BUTTON_USER))
					currentState = BUTTON_FALLING;

				break;
			case BUTTON_FALLING:
				if(delayRead(&delayButton))
				{
					if(BSP_PB_GetState(BUTTON_USER))
					{
						currentState = BUTTON_DOWN;
						fallingEdge = true;
						//buttonPressed();
					}
					else
					{
						currentState = BUTTON_UP;
					}
				}
				break;
			case BUTTON_DOWN:
				if(!BSP_PB_GetState(BUTTON_USER))
					currentState = BUTTON_RAISING;
				break;
			case BUTTON_RAISING:
				if(delayRead(&delayButton))
				{
					if(!BSP_PB_GetState(BUTTON_USER))
					{
						currentState = BUTTON_UP;
						//buttonReleased();
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


static void buttonPressed(void)
{
	BSP_LED_Toggle(LED1);
}


static void buttonReleased(void)
{
	BSP_LED_Toggle(LED3);
}

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

static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

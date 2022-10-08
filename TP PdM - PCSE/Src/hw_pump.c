/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "hw_pump.h"

/*******************************************************************************
 * Global private variables
 ******************************************************************************/
static bool_t pumpErogationState = false, pumpFillState = false;

/*******************************************************************************
 * Functions
 ******************************************************************************/
/**
 * @brief   Pumps init
 * @details By default, set off the pumps
 * @param   none
 * @return 	none
 */
void Pump_Init(void)
{
	pumpSet(FillPump, false);
	pumpSet(ErogationPump, false);
}

/**
 * @brief   Pumps state setting
 * @details Turn on or turn off a pump
 * @param   Output_Pump_t pump: pump selected
 * 			bool_t state :	state to set
 * @return 	none
 */
void pumpSet(Output_Pump_t pump, bool_t state)
{
	if(ErogationPump == pump)
	{
		uartSendString(BOMBA_EROGACION_MSJ);
		pumpErogationState = state;
	}else
	{
		if(FillPump == pump)
		{
			uartSendString(BOMBA_LLENADO_MSJ);
			pumpFillState = state;
		}

	}
	if(true == state)
	{
		uartSendString(ENDEDIDO_MSJ);
	}
	else
	{
		uartSendString(APAGADO_MSJ);
	}
}

/**
 * @brief   Pumps state reading
 * @details Read the current state of a pump
 * @param   Output_Pump_t pump: pump selected
 * @return 	bool_t:	current state
 */
bool_t getPumpState(Output_Pump_t pump)
{
	bool_t returnValue = false;

	switch(pump)
	{
	case ErogationPump:
		returnValue = pumpErogationState;
		break;
	case FillPump:
		returnValue = pumpFillState;
		break;
	default:
		Error_Handler();
		break;
	}
	return returnValue;
}

#ifndef __API_DEBOUNCE_H_
#define __API_DEBOUNCE_H_

#include "main.h"
#include "API_delay.h"

/* Defines*/
#define BUTTON_DEBOUNCE_DELAY			40	// [ms] Delay for debounce


/* Function prototypes */
void debounceFSM_init();	// debe cargar el estado inicial.
void debounceFSM_update();	// debe leer las entradas, resolver la lógica de transición de estados y actualizar las salidas.
bool_t readKey();			//

#endif

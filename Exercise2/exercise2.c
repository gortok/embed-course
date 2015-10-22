/** \file main.c
*
* @brief Embedded Software Boot Camp
*
* @par
* COPYRIGHT NOTICE: (c) 2014 Barr Group, LLC.
* All rights reserved.
*/

#include <stdint.h>
#include "led.h"
#include "adc.h"


/*!
* @brief Delay for some number of milliseconds, by wasting CPU cycles.
*
* @param[in] ms  Number of milliseconds to busy-wait.
* @return void
*/

static void
busywait_ms (uint32_t ms)
{
   // NOTE: This constant was determined through empirical testing.
   uint32_t const ITERATIONS_PER_MS = 8850;

   for (uint32_t volatile cnt = ms * ITERATIONS_PER_MS; cnt > 0; cnt--)
   {
       // NOTE: The volatile keyword assures this empty loop won't optimize.
   }
}

/*!
* @brief This is main() for an Embedded Software Boot Camp exercise.
*/
void main (void)
{
    // Initialize the board.
	BSP_Init();
        init_adc_register();
        uint32_t return_value;
	for (;;)
    {
        // Blink LED4 once a second at 50% duty cycle.
        LED_Toggle(4);
                return_value = (uint32_t) pot_read();
		busywait_ms(return_value);
                
    }
}


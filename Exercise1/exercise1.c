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

int16_t signextend(uint16_t);

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

#define NUM_TESTS   4

static const uint16_t test[NUM_TESTS] = {0x0f64, 0x0bff, 0x2c, 0  };
static const int16_t  expect[NUM_TESTS] = {-156, -1793, 44, 0 };

/*!
* @brief This is main() for an Embedded Software Boot Camp exercise.
*/
void main (void)
{
    // Initialize the board.
	BSP_Init();

    // Test the implementation of signextend().
    int n_errors = 0;

    for (int i = 0; i < NUM_TESTS; ++i)
    {
        if (expect[i] != signextend(test[i]))
        {
            n_errors++;
        }
    }

    // Set the output pattern.
    if (n_errors > 0)
    {
        // Turn on red LED6.
        LED_On(6);

        for (;;)
        {
            // Do nothing.
        }
    }
    else
    {
        for (;;)
        {
            // Blink green LED5 once a second at 50% duty cycle.
		    LED_Toggle(5);
		    busywait_ms(500);
	}
    }
}

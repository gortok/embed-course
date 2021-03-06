/** \file pushbutton.c
*
* @brief Button Debouncer
*
* @par
* COPYRIGHT NOTICE: (C) 2014 Barr Group, LLC.
* All rights reserved.
*/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "os.h"
#include "iorx63n.h"
#include "bsp_glcd.h"
#include "pushbutton.h"								

static uint16_t sw1_counter;
static uint16_t sw2_counter;
/*!
*
* @brief Button Debounce Task
*/
void
debounce_task (void * p_arg)
{
    // State variables for debouncing SW1.
    uint8_t	    b_sw1_curr = 1;
    uint8_t     b_sw1_prev = 1;

    // State variables for debouncing SW2.
    uint8_t	    b_sw2_curr = 1;
    uint8_t     b_sw2_prev = 1;
    uint8_t     b_sw2_retriggered = 1;

    OS_ERR      err;


    (void)p_arg;    // NOTE: Silence compiler warning about unused param.

    // Configure GPIO Port4 as an input.
    PORT4.PDR.BYTE = 0;

    for (;;)
    {
        
        // Delay for 50 ms.
	OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
	
        // Read the current state of the buttons.
        uint8_t raw = PORT4.PIDR.BYTE;

        // Test button SW1.
	b_sw1_curr = raw & 0x01;

        // Execute SW1 debouncing state machine.
	if ((0 == b_sw1_curr) && (0 == b_sw1_prev))
        {
            OSSemPost(&g_sw1_sem, OS_OPT_POST_ALL, &err);
	}

        // Save current SW1 state for next cycle.
	b_sw1_prev = b_sw1_curr;
	
        // Test button SW2.
	b_sw2_curr = raw & 0x02;

        // Execute SW2 debouncing state machine.
	if ((0 == b_sw2_curr) && (0 == b_sw2_prev))
        {
            if (b_sw2_retriggered)
            {
                // TODO: Signal that SW2 has been pressed.;
                OSSemPost(&g_sw2_sem, OS_OPT_POST_ALL, &err);
                // Do not allow repeats while held down.
                b_sw2_retriggered = 0;
            }
        }
        else
        {
            // Button released; reset trigger.
            b_sw2_retriggered = 1;
	}

        // Save current SW2 state for next cycle.
	b_sw2_prev = b_sw2_curr;
    }
}

void handle_sw1_press_task(void * p_arg)
{
  
  OS_ERR err;
  for (;;) {
    OSSemPend(&g_sw1_sem, 0, OS_OPT_PEND_BLOCKING, (void *)0, &err);
    
    sw1_counter = sw1_counter + 1;
    char p_str[12];
    sprintf(p_str, "SW1: %4u", sw1_counter);
    BSP_GraphLCD_String(1, p_str);
  }
  
  
}
void handle_sw2_press_task(void * p_arg)
{
  
  OS_ERR err;
  for (;;) {
    OSSemPend(&g_sw2_sem, 0, OS_OPT_PEND_BLOCKING, (void *)0, &err);
    sw2_counter = sw2_counter + 1;
    char p_str[12];
    sprintf(p_str, "SW2: %4u", sw2_counter);
    BSP_GraphLCD_String(3, p_str);
  }
}

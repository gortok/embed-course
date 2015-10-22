/** \file main.c
*
* @brief Embedded Software Boot Camp
*
* @par
* COPYRIGHT NOTICE: (C) 2014 Barr Group, LLC.
* All rights reserved.
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "cpu_cfg.h"
#include  "bsp_cfg.h"
#include  "assert.h"

#include  <cpu_core.h>
#include  <os.h>
#include  <bsp_glcd.h>

#include  "bsp.h"
#include  "bsp_int_vect_tbl.h"
#include  "bsp_led.h"
#include  "os_app_hooks.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

// Relative Task Priorities (0 = highest; 15 = idle task)
#define  LED5_PRIO              14   // Every 500 ms, in a timed loop.

// Allocate Task Stacks
#define  TASK_STACK_SIZE      128

static CPU_STK  g_led5_stack[TASK_STACK_SIZE];

// Allocate Task Control Blocks
static OS_TCB   g_led5_tcb;


/*
*********************************************************************************************************
*                                            LOCAL MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void
led5_task (void * p_arg)
{
    OS_ERR  err;


    (void)p_arg;    // NOTE: Silence compiler warning about unused param.

    // Perform hardware initializations that should be after multitasking.
    BSP_Init();
    CPU_Init();
    Mem_Init();

    for (;;)
    {
        // Flash LED at 1 Hz.
	BSP_LED_Toggle(5);
        OSTimeDlyHMSM(0,0,0,500, OS_OPT_TIME_HMSM_STRICT, &err);  
	// TODO: Use an OS call to delay for 500 ms.
        assert(OS_ERR_NONE == err);
    }
}


/*
*********************************************************************************************************
*                                               main()
*
* Description : Entry point for C code.
*
* Arguments   : none.
*
* Returns     : none.
*
* Note(s)     : (1) It is assumed that your code will call main() once you have performed all necessary
*                   initialization.
*********************************************************************************************************
*/

void  main (void)
{
    OS_ERR  err;
    char * task_name = "LED 5 Task";
    CPU_IntDis();                                               /* Disable all interrupts.                              */

    BSP_IntVectSet(27, (CPU_FNCT_VOID)OSCtxSwISR);              /* Setup kernel context switch                          */

    // TODO: Initialize the operating system's internal data structures
    OSInit(&err);
    // TODO: Create at least one task
    assert(OS_ERR_NONE == err);
    OSTaskCreate(&g_led5_tcb, task_name, led5_task, (void *)0, 14, &g_led5_stack[0], 10, TASK_STACK_SIZE,5,0,(void *)0, 0, &err);
    // TODO: Start multitasking
    assert(OS_ERR_NONE == err);
    OSStart(&err);
    // We should never get here.
    assert(0);
}


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
#define  LED_PRIORITY              14   // Every 500 ms, in a timed loop.
#define  HIGH_PRIORITY             5
// Allocate Task Stacks
#define  TASK_STACK_SIZE      128
#define LED_OFF 0x00
#define LED_ON  0x01

static CPU_STK  g_led5_stack[TASK_STACK_SIZE];
static CPU_STK  g_led6_stack[TASK_STACK_SIZE];
static CPU_STK  g_startup_stack[TASK_STACK_SIZE];
// Allocate Task Control Blocks
static OS_TCB   g_led5_tcb;
static OS_TCB   g_led6_tcb;
static OS_TCB   g_startup_tcb;
static uint8_t led5_toggle_state;
OS_MUTEX led_mutex;


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
   

    for (;;)
    {
        // Flash LED at 1 Hz.
        OSMutexPend(&led_mutex, 0, OS_OPT_PEND_BLOCKING, 0, &err);
	BSP_LED_Toggle(5);
        OSMutexPost(&led_mutex, OS_OPT_POST_NO_SCHED, &err);
        OSTimeDlyHMSM(0,0,0,500, OS_OPT_TIME_HMSM_STRICT, &err);  
	// TODO: Use an OS call to delay for 500 ms.
        assert(OS_ERR_NONE == err);
    }
}
void led6_task (void * p_arg)
{
  OS_ERR err;
  (void)p_arg;
  
  for (;;)
  {
    OSMutexPend(&led_mutex, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    BSP_LED_Toggle(6);
    OSMutexPost(&led_mutex, OS_OPT_POST_NO_SCHED, &err);
    OSTimeDlyHMSM(0,0,0,167, OS_OPT_TIME_HMSM_STRICT, &err);
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
void startup_task (void * p_arg)
{
    OS_ERR err;
    BSP_Init();
    CPU_Init();
    Mem_Init(); 
    //create mutex
    
    OSMutexCreate(&led_mutex, "LED Mutex", &err);
    char * task5_name = "LED 5 Task";
    char * task6_name = "LED 6 Task";
    //LED5 Create
    OSTaskCreate(&g_led5_tcb, task5_name, led5_task, (void *)0, LED_PRIORITY, &g_led5_stack[0], 10, TASK_STACK_SIZE,5,0,(void *)0, 0, &err);
    //LED6 Create
    OSTaskCreate(&g_led6_tcb, task6_name, led6_task, (void *)0, LED_PRIORITY, &g_led6_stack[0], 10, TASK_STACK_SIZE,5,0,(void *)0, 0, &err);
    // Delete self
    OSTaskDel(&g_startup_tcb, &err);
}
void  main (void)
{
    OS_ERR  err;
    
    CPU_IntDis();                                               /* Disable all interrupts.                              */

    BSP_IntVectSet(27, (CPU_FNCT_VOID)OSCtxSwISR);              /* Setup kernel context switch                          */

    // TODO: Initialize the operating system's internal data structures
    OSInit(&err);
    // TODO: Create at least one task
    assert(OS_ERR_NONE == err);
    OSTaskCreate(&g_startup_tcb, "Startup", startup_task, (void *)0, HIGH_PRIORITY, &g_startup_stack[0], 10, TASK_STACK_SIZE,5,0,(void *)0, 0, &err);
    // TODO: Start multitasking
    assert(OS_ERR_NONE == err);
    OSStart(&err);
    // We should never get here.
    assert(0);
}


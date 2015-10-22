
#include  <os.h>

#include  "assert.h"
#include  "bsp_led.h"
#include "os_cfg_app.h"
#include "heartbeat.h"


#define HEART_BEAT_LED 4
#define LED_FREQUENCY 3 //LED frequency in Hz


OS_TMR g_heartBeatTmr;

void
ToggleHeartBeat (void)
{
    BSP_LED_Toggle( HEART_BEAT_LED );
}  


void HeartBeatTimerCreateAndStart(void){
 OS_ERR  err;

   OSTmrCreate(
             &g_heartBeatTmr,
             "heartBeatTimer",
             10, //delay
             OS_CFG_TMR_TASK_RATE_HZ/(2*LED_FREQUENCY), //period in ticks
             OS_OPT_TMR_PERIODIC,
             (OS_TMR_CALLBACK_PTR)ToggleHeartBeat,
             0,
             &err);
          
   assert( 0 == err);
    
   OSTmrStart(
              &g_heartBeatTmr,
              &err);
   
   assert( 0 == err);
   
}
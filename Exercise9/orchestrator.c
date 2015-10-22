
#include <stdint.h>
#include <stdio.h>
#include "scuba.h"

#include "orchestrator.h"
#include "depth.h"

#include "alarm.h"
#include "pushbutton.h"
#include "os.h"
#include "display.h"

static int32_t depth;

static uint32_t total_air;
static void calculations (uint32_t delta_rate_in_m);
//static uint8_t units; //0x1 imperial ; 0x0 metric
static OS_ERR err;
static uint32_t time_elapsed;
static int32_t dep_change_half_sec_mm;
void orchestrator_task(void * p_arg)
{ 
  time_elapsed = 0;
  dep_change_half_sec_mm = 0;
  depth = 0;
  
  for (;;) {
    OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);
    time_elapsed++;
    int32_t current = depth_delta_calc_in_m();
    //OSSemPend(&g_sw1_sem, 0, OS_OPT_PEND_NON_BLOCKING, (void *)0, &err);
    calculations(current);
    //void CheckAlarmFlags(int32_t depth_m,int32_t ascent_rate_m_min,uint32_t air_in_tank)
    CheckAlarmFlags(depth, current, total_air);
    updateDisplay(get_depth(), get_rate(), get_air(), get_elapsed_dive_time(), get_alarm());
  }
}

// updateDisplay(char * depth, char * rate, char * air,  char * edt, char * alarm)

char * get_alarm()
{
  uint16_t alarm = OSFlagPend(&g_alarm_flags, ALARM_HIGH | ALARM_MEDIUM | ALARM_LOW | ALARM_NONE, 0, OS_OPT_PEND_FLAG_SET_ANY, (void *)0, &err);
  static char return_as[13];
  if(alarm & ALARM_HIGH)
  {
    sprintf(return_as,"%s","Alarm: HIGH ");
  }
  else if(alarm & ALARM_MEDIUM)
  {
    sprintf(return_as,"%s","Alarm: Med.");
  }
  else if(alarm & ALARM_LOW)
  {
    sprintf(return_as,"%s","Alarm: Low ");
  }
  else
  {
    sprintf(return_as,"%s","Alarm: None");
  }
  return return_as;
}
char * get_elapsed_dive_time(void)
{
  static char formatted_time[13];
  uint32_t time_in_seconds, time_in_minutes, time_in_hours;
  time_in_seconds = time_elapsed / 2;
  time_in_hours = time_in_seconds / 3600;
  uint32_t seconds_left_over, over_over;
  seconds_left_over = time_in_seconds % 3600; 
  time_in_minutes = seconds_left_over / 60;
  over_over = time_in_minutes % 60;
  sprintf(formatted_time, "EDT: %2u:%2u:%2u", time_in_hours, time_in_minutes, over_over); 
  return formatted_time;
}

uint16_t get_air(void)
{
 return total_air;
}

char * get_depth(void)
{
  static char return_string[13];
  sprintf(return_string, "DEPTH: %6d", depth);
 return return_string;
}
char * get_rate(void) 
{
  static char return_string[13];
  sprintf(return_string, "RATE: %6d", dep_change_half_sec_mm);
  return return_string;
}


static void calculations (uint32_t delta_rate_in_m)
{
  OS_ERR err;
   // *** UPDATE DEPTH  ***
  // Must be called every 500ms -
  // Signed channged in depth every half second (up or down);
  // in milli-meters.
  //int32_t dep_change_half_sec_mm;
  // Calculate the change in depth - can be pos or neg (up or down)  
  dep_change_half_sec_mm = depth_change_in_mm(delta_rate_in_m);  
  depth += (dep_change_half_sec_mm / 1000);
  
  
  // *** UPDATE TANK ***
  // Must be called every 500ms - 
  // takes in milli-meters (*1000), returns centilitres in gas.
  uint32_t air_used_half_sec = gas_rate_in_cl(depth * 1000);
  
  // update tank based on USED air
  // Total air in tank in LITERS (100 cL = 1L)
  total_air = total_air - (air_used_half_sec / 100);
  
  // update tank based on ADDED air - if any,
  // should be 0 otherwise and not make any difference:
  OSSemPend(&g_sw1_sem, 0, OS_OPT_PEND_NON_BLOCKING, (void *)0, &err);
  if ( !(err == OS_ERR_PEND_WOULD_BLOCK )) {
    if (err == OS_ERR_NONE) {
      total_air = total_air + 5;
    }
  }  
}


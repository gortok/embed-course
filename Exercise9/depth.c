//depth.c

#include "depth.h"
#include "adc.h"
#include "scuba.h"
#include <stdint.h>
#include "os.h"
#include "assert.h"

int depth_delta;

/* depth_rate_calc
Pends Blocking on Queue from ADC returns as depth change in mm
since last call
*/
int32_t depth_delta_calc_in_m()
{
  OS_ERR     err;
  //int depth_delta;
  static uint16_t adc_raw;
  
  
  adc_raw = (uint32_t)OSQPend(&g_adc_orch_q,0,OS_OPT_PEND_BLOCKING,0,0,&err);
  assert(OS_ERR_NONE == err);
  //adc_raw = adc_raw >> 2;
  depth_delta = ADC2RATE(adc_raw);

  return depth_delta;
}
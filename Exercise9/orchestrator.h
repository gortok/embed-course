/* orchestrator.h */

#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <stdint.h>
#include "adc.h"

char * get_alarm(void);
char * get_elapsed_dive_time(void);
uint16_t get_air(void);
char * get_depth(void);
char * get_rate(void); 
void orchestrator_task(void * p_arg);
void  adc_task (void * p_arg);
#endif

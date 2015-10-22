/** \file adc.h
*
* @brief ADC Driver
*
* @par
* COPYRIGHT NOTICE: (c) 2011 Netrino, LLC.
* All rights reserved.
*/
#ifndef _ADC_H
#define _ADC_H
extern OS_FLAG_GRP adc_flag_group;
void  adc_task (void * p_arg);
#define HIGH_ALARM              0x8
#define MEDIUM_ALARM            0x4
#define LOW_ALARM               0x2
#define NO_ALARM                0x1
#endif /* _ADC_H */

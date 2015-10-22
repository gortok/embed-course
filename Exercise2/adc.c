/* adc.c */

#include <intrinsics.h>
#include  <stdint.h>
#include  "adc.h"


typedef struct {
  uint8_t ad_control_register; //adcsr //0008 9000h
  uint8_t const unused_0[3]; //padding from 0008 9001h -> 0008 9003h
  uint16_t ad_channel_select_register_0;
  uint16_t ad_channel_select_register_1;
  uint16_t ad_converted_value_addition_mode_select_register_0;
  uint16_t ad_converted_value_addition_mode_select_register_1;
  uint8_t ad_converted_value_addition_count_select_register; // 0008 900Ch
  uint8_t const unused_1[1]  ; //padding from 0008 900Dh -> 0008900Eh
  uint16_t ad_control_extended_register;
  uint8_t ad_start_trigger_select_register ; // 0008 9010h
  uint8_t const unused_2[1] ; 
  uint16_t ad_converted_extended_input_control_register ; //0008 9012h
  uint8_t const unused_3[6]; 
  uint16_t ad_temperature_sensor_data_register ;
  uint16_t ad_internal_reference_voltage_data_register ; // 0008 901Ch
  uint8_t const unused_5[2];
  uint16_t ad_data_registers [21] ;
  uint16_t ad_sampling_state_register_01 ; //0008 9060h
  uint16_t ad_sampling_state_register_23 ; //0008 9070h
}  adc_t;


static adc_t volatile * const p_adc_registers = (adc_t *) 0x00089000;
static void pot_init(void);
static void start_scan(void);
static uint16_t volatile p_data_registers[21];

#pragma vector=102
__interrupt void isr_interrupt(void)
{
  int len = 22;
  for (int i = 0; i < len; i=i+1) {
    p_data_registers[i] = p_adc_registers->ad_data_registers[i];
  }
}

void init_adc_register(void) {

  pot_init();
  p_adc_registers->ad_control_register |= 0x50 ; // 0101 0000b
  p_adc_registers->ad_channel_select_register_0 |= 0xFF; // 1111 1111h  
  p_adc_registers->ad_channel_select_register_1 |= 0x1F; // 0001 1111h 
  start_scan();
} 



static void pot_init(void) {
  uint8_t * p_IER = (uint8_t *) 0x0008720C;
  uint8_t * p_IPR = (uint8_t *) 0x00087366;
  *p_IPR = 9;
  *p_IER |= 0x40;
  
}
static void start_scan(void) {
   p_adc_registers->ad_control_register |= 0x80;
}
uint16_t pot_read(void) {
    uint16_t sample;
      __disable_interrupt();
      {
        sample = p_data_registers[2] / 4;
      }
      __enable_interrupt();
      return sample;
}

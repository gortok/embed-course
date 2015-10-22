/** \file signextend.c
*
* @brief Two's Complement Sign-Extension
*
* @par
* COPYRIGHT NOTICE: (c) 2014 Barr Group, LLC.
* All rights reserved.
*/


#include <stdint.h>


/*!
* @brief Sign extend a signed 12-bit value to a signed 16-bit value.
*
* @param[in] input A 12-bit signed value, in 2's complement format.
*
* @return int16_t A 16-bit signed value, in 2's complement format.
*/
int16_t
signextend(uint16_t input)
{
  int16_t output;
  if (input & 0x0800)
  {
    output = input | 0xF000;
  }
  else {
    output = input & 0x0FFF;
  }
  
  // NOTE: There's good info on the 2's complement representation of signed
   // integers at: http://en.wikipedia.org/wiki/Two's_complement
   // Here only the least significant 12-bits are properly formated as 2's
   // complement. The upper 4 bits are garbage that your code needs to modify.
   
   return (output);
}
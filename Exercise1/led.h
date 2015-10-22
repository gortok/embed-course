/** \file led.h
*
* @brief Renesas YRDKRX63N LED Driver
*/

#ifndef  _LED_H
#define  _LED_H

void        BSP_Init          (void);
void        LED_On            (uint8_t  led);
void        LED_Off           (uint8_t  led);
void        LED_Toggle        (uint8_t  led);

#endif /* _LED_H */

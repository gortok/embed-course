/** \file led.c
*
* @brief Renesas YRDKRX63N LED Driver
* @brief Embedded Software Boot Camp
*
* @par
* COPYRIGHT NOTICE: (c) 2014 Barr Group, LLC.
* All rights reserved.
*/

#include  <stdint.h>
/* Includes assembley level definitions */
#include <machine.h>
#include  "led.h"
#include  "iorx63n.h"


// LED-to-GPIO port mappings.
#define  LED4                   PORTD.PODR.BIT.B5
#define  LED5                   PORTE.PODR.BIT.B3
#define  LED6                   PORTD.PODR.BIT.B2
#define  LED7                   PORTE.PODR.BIT.B0
#define  LED8                   PORTD.PODR.BIT.B4
#define  LED9                   PORTE.PODR.BIT.B2
#define  LED10                  PORTD.PODR.BIT.B1
#define  LED11                  PORTD.PODR.BIT.B7
#define  LED12                  PORTD.PODR.BIT.B3
#define  LED13                  PORTE.PODR.BIT.B1
#define  LED14                  PORTD.PODR.BIT.B0
#define  LED15                  PORTD.PODR.BIT.B6

#define OUTPUT_PIN          (1)

enum { LED_ON = 0, LED_OFF };


/*!
* @brief Enable control of the user LEDs.
*/
static void
LED_Init (void)
{
    // Set GPIO pin function.
    PORTD.PDR.BIT.B0 = OUTPUT_PIN;
    PORTD.PDR.BIT.B1 = OUTPUT_PIN;
    PORTD.PDR.BIT.B2 = OUTPUT_PIN;
    PORTD.PDR.BIT.B3 = OUTPUT_PIN;
    PORTD.PDR.BIT.B4 = OUTPUT_PIN;
    PORTD.PDR.BIT.B5 = OUTPUT_PIN;
    PORTD.PDR.BIT.B6 = OUTPUT_PIN;
    PORTD.PDR.BIT.B7 = OUTPUT_PIN;

    PORTE.PDR.BIT.B0 = OUTPUT_PIN;
    PORTE.PDR.BIT.B1 = OUTPUT_PIN;
    PORTE.PDR.BIT.B2 = OUTPUT_PIN;
    PORTE.PDR.BIT.B3 = OUTPUT_PIN;

    // Turn off all user LEDs.
    LED_Off(0);
}


/*!
* @brief Initialize the board. This function should be called from main().
*/
void
BSP_Init (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;            
    
    /* Stop sub-clock */
    SYSTEM.SOSCCR.BYTE = 0x01u;            
                                        
    /* Set main oscillator settling time to 10ms (131072 cycles @ 12MHz) */ 
    SYSTEM.MOSCWTCR.BYTE = 0x0Du;        
        
    /* Set PLL circuit settling time to 10ms (2097152 cycles @ 192MHz) */                        
    SYSTEM.PLLWTCR.BYTE = 0x0Eu;            
    
    /* Set PLL circuit to x16 */
    SYSTEM.PLLCR.WORD = 0x0F00u;        

    /* Start the external 12Mhz oscillator */
    SYSTEM.MOSCCR.BYTE = 0x00u;    
            
    /* Turn on the PLL */
    SYSTEM.PLLCR2.BYTE = 0x00u;    
    
    /* Wait over 12ms (~2075op/s @ 125KHz) */
    for(volatile uint16_t i = 0; i < 2075u; i++)
    {
        nop();
    }
        
    /* Configure the clocks as follows -
    Clock Description              Frequency
    ----------------------------------------
    PLL Clock frequency...............192MHz
    System Clock Frequency.............96MHz
    Peripheral Module Clock B..........48MHz     
    FlashIF Clock......................48MHz
    External Bus Clock.................48MHz */                 
    SYSTEM.SCKCR.LONG = 0x21821211u;    
    
    /* Configure the clocks as follows -
    Clock Description              Frequency
    ----------------------------------------
    USB Clock..........................48MHz      
    IEBus Clock........................24MHz */ 
    SYSTEM.SCKCR2.WORD = 0x0033u;
    
    /* Set the clock source to PLL */
    SYSTEM.SCKCR3.WORD = 0x0400u;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;        

    // Select the 12-bit ADC.
    SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;

    /* Use the AN000 (Potentiometer) pin 
       as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01;

    LED_Init();
}


/*!
* @brief Turn on one or all of the user LEDs on the board.
*
* @param[in] led The user LED number (4-15) or 0 for all the LEDs.
*/
void  LED_On (uint8_t  led)
{
    switch (led) {
        case 0:   // All.
            LED4  = LED_ON;
            LED5  = LED_ON;
            LED6  = LED_ON;
            LED7  = LED_ON;
            LED8  = LED_ON;
            LED9  = LED_ON;
            LED10 = LED_ON;
            LED11 = LED_ON;
            LED12 = LED_ON;
            LED13 = LED_ON;
            LED14 = LED_ON;
            LED15 = LED_ON;
            break;

        case 4:
            LED4  = LED_ON;
            break;

        case 5:
            LED5  = LED_ON;
            break;

        case 6:
            LED6  = LED_ON;
            break;

        case 7:
            LED7  = LED_ON;
            break;

        case 8:
            LED8  = LED_ON;
            break;

        case 9:
            LED9  = LED_ON;
            break;

        case 10:
            LED10 = LED_ON;
            break;

        case 11:
            LED11 = LED_ON;
            break;

        case 12:
            LED12 = LED_ON;
            break;

       case 13:
            LED13 = LED_ON;
            break;

       case 14:
            LED14 = LED_ON;
            break;

       case 15:
            LED15 = LED_ON;
            break;

       default:
            break;
    }
}


/*!
* @brief Turn off one or all of the user LEDs on the board.
*
* @param[in] led The user LED number (4-15) or 0 for all the LEDs.
*/
void
LED_Off (uint8_t led)
{
    switch (led) {
        case 0:   // All.
            LED4  = LED_OFF;
            LED5  = LED_OFF;
            LED6  = LED_OFF;
            LED7  = LED_OFF;
            LED8  = LED_OFF;
            LED9  = LED_OFF;
            LED10 = LED_OFF;
            LED11 = LED_OFF;
            LED12 = LED_OFF;
            LED13 = LED_OFF;
            LED14 = LED_OFF;
            LED15 = LED_OFF;
            break;

        case 4:
            LED4  = LED_OFF;
            break;

        case 5:
            LED5  = LED_OFF;
            break;

        case 6:
            LED6  = LED_OFF;
            break;

        case 7:
            LED7  = LED_OFF;
            break;

        case 8:
            LED8  = LED_OFF;
            break;

        case 9:
            LED9  = LED_OFF;
            break;

        case 10:
            LED10 = LED_OFF;
            break;

        case 11:
            LED11 = LED_OFF;
            break;

        case 12:
            LED12 = LED_OFF;
            break;

       case 13:
            LED13 = LED_OFF;
            break;

       case 14:
            LED14 = LED_OFF;
            break;

       case 15:
            LED15 = LED_OFF;
            break;

       default:
            break;
    }
}

/*!
* @brief Toggle the state of one or all the user LEDs on the board.
*
* @param[in] led The user LED number (4-15) or 0 for all the LEDs.
*/
void
LED_Toggle (uint8_t led)
{
    switch (led) {
        case 0:   // All.
            LED4  = ~LED4;
            LED5  = ~LED5;
            LED6  = ~LED6;
            LED7  = ~LED7;
            LED8  = ~LED8;
            LED9  = ~LED9;
            LED10 = ~LED10;
            LED11 = ~LED11;
            LED12 = ~LED12;
            LED13 = ~LED13;
            LED14 = ~LED14;
            LED15 = ~LED15;
            break;

        case 4:
            LED4  = ~LED4;
            break;

        case 5:
            LED5  = ~LED5;
            break;

        case 6:
            LED6  = ~LED6;
            break;

        case 7:
            LED7  = ~LED7;
            break;

        case 8:
            LED8  = ~LED8;
            break;

        case 9:
            LED9  = ~LED9;
            break;

        case 10:
            LED10 = ~LED10;
            break;

        case 11:
            LED11 = ~LED11;
            break;

        case 12:
            LED12 = ~LED12;
            break;

       case 13:
            LED13 = ~LED13;
            break;

       case 14:
            LED14 = ~LED14;
            break;

       case 15:
            LED15 = ~LED15;
            break;

       default:
            break;
    }
}

#ifndef _LED_BUTTONS_H
#define _LED_BUTTONS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

// Define LAT registers for all the LEDs

/**************************
 * Colour click module LEDs
 **************************/
#define RED_LED LATGbits.LATG1   // red LED LAT register
#define GREEN_LED LATAbits.LATA4 // green LED LAT register
#define BLUE_LED LATFbits.LATF7  // blue LED LAT register

/**************************
 * Function Protocols
 **************************/
void LED_init(void);
void WhiteLight(void);

#endif
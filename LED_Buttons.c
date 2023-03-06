#include <xc.h>
#include "LED_Buttons.h"

// Here we will initialize all the clicker 2 LEDs, buttons, Color Click LEDs and Buggy LEDs

// Initialize all the LEDs here
void LED_init(void){
    TRISGbits.TRISG1 = 0; // red LED
    TRISAbits.TRISA4 = 0; // green LED
    TRISFbits.TRISF7 = 0; // blue LED
    
    LATHbits.LATH3 = 0;
    TRISHbits.TRISH3 = 0;
}

// Turn all all the red, green and blue LEDs to produce white light on color click
void WhiteLight(void){
    RED_LED = 1;
    GREEN_LED = 1;
    BLUE_LED = 1;
}
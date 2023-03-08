// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "i2c.h"
#include "LED_Buttons.h"
#include "interrupts.h"
#include "serial.h"
#include "timers.h"
#include "color_instructions.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    
    // RGBC structure call and initial color values initialisation
    RGB_val initial_color;
    initial_color.R = 0;
    initial_color.G = 0;
    initial_color.B = 0;
    initial_color.C = 0;
    // Initialization sequences
    LED_init();
    color_click_init();
    Interrupts_init();
    //initUSART4(); // For Serial Comms
    Timer0_init();
    
    // Motor Struct Initialisation
    initDCmotorsPWM(99); // Set PWM Period to 99
    // Here we define the motor structs:
    struct DC_motor motorL, motorR; 		//declare two DC_motor structures 
    // motorL
    motorL.power=0; 						//zero power to start
    motorL.direction=1; 					//set default motor direction
    motorL.brakemode=1;						// brake mode (slow decay)
    motorL.posDutyHighByte=(unsigned char *)(&CCPR1H);  //store address of CCP1 duty high byte
    motorL.negDutyHighByte=(unsigned char *)(&CCPR2H);  //store address of CCP2 duty high byte
    motorL.PWMperiod= T2PR; 			//store PWMperiod for motor (value of T2PR in this case)
    // motorR
    motorR.power=0; 						//zero power to start
    motorR.direction=1; 					//set default motor direction
    motorR.brakemode=1;						// brake mode (slow decay)
    motorR.posDutyHighByte=(unsigned char *)(&CCPR3H);  //store address of CCP3 duty high byte
    motorR.negDutyHighByte=(unsigned char *)(&CCPR4H);  //store address of CCP4 duty high byte
    motorR.PWMperiod= T2PR; 			//store PWMperiod for motor (value of T2PR in this case)
    
    // Initialize color detected variable
    unsigned char color_detected = 0;
    WhiteLight(); // Turn on the white light
    while (1){
        Update_RGBC(&initial_color); // Update the RGBC values
        color_detected = detect_color(&initial_color); // Pass initial color values into detect color function
        // If statement to continue moving Buggy forward while color detected is null (0)
        unsigned char b = 0;
        if(color_detected == 0){
            fullSpeedAhead(&motorL,&motorR);
            b++;
        }  
        // If color detected is no longer null, stop Buggy and decide the movement of the Buggy
        else if(color_detected != 0){
            LATHbits.LATH3 = 1; // Turn ON LED to indicate a color has been detected
            // Stop motor
            while(b>0){
                stop(&motorL,&motorR);
                b--;
            }
            __delay_ms(500); // Delay before movement execution
            LATHbits.LATH3 = 0; // Turn OFF LED
            // Check the color detected and determine the instruction for the buggy
            MoveBuggy(color_detected,&motorL,&motorR);
            color_detected = 0; // Set color detected back to zero after movement is executed
        }
        
    }
 
}

#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    
    // Initialize TMR0 interrupts
    PIE0bits.TMR0IE=1; 	//enable interrupt source TMR0
    IPR0bits.TMR0IP = 0; // low priority
    
    // Initialize interrupt for receiving data via serial4
    PIE4bits.RC4IE=1;	//receive interrupt

    // Other interrupts
    INTCONbits.IPEN = 1; // Enable interrupt priority
    INTCONbits.PEIE = 1; // peripheral interrupts
    INTCONbits.GIE=1; 	//turn on interrupts globally (when this is off, all interrupts are deactivated)
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/

unsigned char tmr_ovf = 0; // Timer overflow flag
// Low priority interrupt
void __interrupt(low_priority) LowISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){ 	//check the interrupt source
        LATHbits.LATH3 = !LATHbits.LATH3; // Toggle LED 2
        TMR0H = 0b11000010; // Reset timer value to 49910 (decimal) this is to accurately measure 250ms intervals
        TMR0L = 0b11110110;
        tmr_ovf = 1;
        PIR0bits.TMR0IF=0; //clear the interrupt flag!     
    }
}

// High priority interrupt
// Configure to receive data via serial4 and transmit
void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    // Here we interrupt to receive data
    if(PIR4bits.RC4IF){ 	//check the interrupt source
        unsigned char a = RC4REG;
        putCharToRxBuf(a); // Receive byte from PC and add to buffer
    }
    
    // Here we interrupt to transmit data
    if(!isDataInTxBuf()){ // Check if buffer is empty
        PIE4bits.TX4IE=0; // Turn off interrupt if buffer is empty
    }
    if(PIR4bits.TX4IF){
        TX4REG = getCharFromTxBuf(); // Transmit character to PC
    }
}



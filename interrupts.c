#include <xc.h>
#include "interrupts.h"
#include "serial.h"
#include "color.h"
#include "i2c.h"
#include "LED_Buttons.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use INT1IE.
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    
    // Initialize color clicker interrupts
    TRISBbits.TRISB1 = 1; // Set RB1 pin as input
    ANSELBbits.ANSELB1 = 0; // Turn off analog on RB1 pin
    //INT1PPS = 0x09; // This is incase we decide to remap RB1 for different I/0
    PIE0bits.INT1IE = 1; // Enable interrupt on RB1 pin
    IPR0bits.INT1IP = 1; // high priority
    INTCONbits.INT1EDG = 0; // Set RB1 interrupt to trigger on falling edge
    
    // Initialize TMR0 interrupts
    PIE0bits.TMR0IE=1; 	//enable interrupt source TMR0
    IPR0bits.TMR0IP = 0; // low priority
    
    // Initialize interrupt for receiving data via serial4
    PIE4bits.RC4IE=1;	//receive interrupt
    
    interrupts_clear_colorclick(); // Clear and initialize the color clicker interrupts
    // Other interrupts
    INTCONbits.IPEN = 1; // Enable interrupt priority
    INTCONbits.PEIE = 1; // peripheral interrupts
    INTCONbits.GIE=1; 	//turn on interrupts globally (when this is off, all interrupts are deactivated)
}

// Function to initialize the color clicker interrupt, set threshold values and persistence registers
unsigned int int_threshold_low = 0; // Low threshold for color click interrupts
unsigned int int_threshold_high = 2900; // High threshold for color click interrupts

void init_colorclick_interrupts(void)
{
    color_writetoaddr(0x00,0b10011); // Enable RGBC interrupt, RGBC ADC, and internal oscillator, refer pg 13 of TCS3471 document
    __delay_ms(3); // Delay before writing again
    // NOTE 1: A minimum interval of 2.4 ms must pass after PON is asserted before an RGBC can be initiated (pg 13 of TCS3471 document)
    color_writetoaddr(0x0C,0b0100); // Write to enable persistence register to trigger interrupt after 5 readings outside of threshold range (pg 14 TCS3471)
    color_writetoaddr(0x04,(int_threshold_low & 0xFF)); // RGBC clear channel low threshold lower byte (pg 14 TCS3471)
    color_writetoaddr(0x05,(int_threshold_low >> 8)); // RGBC clear channel low threshold higher byte
    color_writetoaddr(0x06,(int_threshold_high & 0xFF)); // RGBC clear channel high threshold lower byte
    color_writetoaddr(0x07,(int_threshold_high >> 8)); // RGBC clear channel high threshold higher byte
    
}

// Function to clear color-click interrupts
void interrupts_clear_colorclick(void)
{
    I2C_2_Master_Start();            // Start condition
    I2C_2_Master_Write(0x52 | 0x00); // 7 bit device address + Write mode
    I2C_2_Master_Write(0b11100110);  // Command + Register address (Command 1 (7), Special function 11 (6:5), RGBC interrupt clear 00110 (4:0) TCS3471 pg 12)
    I2C_2_Master_Stop();             // Send stop bit
    
    init_colorclick_interrupts(); // Initialize the colorclick interrupts
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/

unsigned char tmr_ovf = 0; // Timer overflow flag
unsigned char lost_flag = 0; // Lost flag
// Low priority interrupt
void __interrupt(low_priority) LowISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){ 	//check the interrupt source
        TMR0H = 0b00001011; // Reset timer value to 3035 (decimal) this is to accurately measure 1s intervals
        TMR0L = 0b11011011;
        tmr_ovf = 1;
        PIR0bits.TMR0IF=0; //clear the interrupt flag!     
    }
}

// High priority interrupt
// Configure to receive data via serial4 and transmit and detect colored card
unsigned char color_flag = 0;
void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    
    /*******************************************************
    // Here we interrupt to send and receive via serial comm
    ********************************************************/
    // Here we interrupt to receive data (uncomment this section to use serial comms)
    /*
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
    */
    
    /****************************************************
    // Here we interrupt to check if color card is found
    *****************************************************/
    
    if(PIR0bits.INT1IF){ // If the flag is raised (clear channel value is outside of threshold range)
        color_flag = 1; // Raise flag for colored card detection
        //LATHbits.LATH3 = !LATHbits.LATH3; // Turn ON and OFF LED for debugging purposes
        interrupts_clear_colorclick(); // Clear colorclick interrupts
        PIR0bits.INT1IF = 0; // Clear interrupt flag
    }
    
}



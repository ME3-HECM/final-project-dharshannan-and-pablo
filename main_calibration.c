// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "color.h"
#include "i2c.h"
#include "LED_Buttons.h"
#include "interrupts.h"
#include "serial.h"
#include "timers.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    
    // RGBC structure call
    RGB_val initial_color;
    initial_color.R = 0;
    initial_color.G = 0;
    initial_color.B = 0;
    initial_color.C = 0;
    // Initialization sequences
    LED_init();
    color_click_init();
    Interrupts_init();
    initUSART4();
    Timer0_init();
    // Now we will configure to send RGBC color values to PC via serial comm
    char a = 0; // Define temp variable for serial comm
    //int readingsR[100]; // Array to hold 100 RGBC readings via serial
    //int readingsG[100];
    //int readingsB[100];
    //int readingsC[100];
    
    while(1){
        WhiteLight(); // Turn on the white light
        Update_RGBC(&initial_color); // Update the RGBC values
        
        // Uncomment this section for calibration
        /***********************************************************************
        
        // Now when a timer overflow occurs append the value of RGBC to the array
        while(tmr_ovf==1 && a<100){
            readingsR[a] = initial_color.R;// Append red value to array
            readingsG[a] = initial_color.G;// Append green value to array
            readingsB[a] = initial_color.B;// Append blue value to array
            readingsC[a] = initial_color.C;// Append clear value to array
            a++; // Increment a
            tmr_ovf = 0; // Reset timer overflow flag to 0
        }
        
        // Define a control character which will be received from the PC
        char cont = 0x00; // 0 is initial control value
        // Get control value from PC
        while(isDataInRxBuf()){
            // Set cont to this new value
            cont = getCharFromRxBuf();
        }
        
        // Check if the control character is either 0x10 or 0x01
        // 0x10: Send 100 RGBC readings, 0x01: Send current RGBC reading
        if(cont == 0x10){
            unsigned char i;
            for(i=0;i<a;i++){
                // Send to PC
                char string[40];
                sprintf(string,"\n%05d %05d %05d %05d\n",readingsR[i],readingsG[i],readingsB[i],readingsC[i]);
                TxBufferedString(string);
                // Raise interrupt flag for transmission if buffer is not empty
                sendTxBuf();
                __delay_ms(250); // Delay to see it appear on the screen RealTerm at a better interval of time
            }
            cont = 0x00; // Reset cont value
            a = 0; // Reset a also
        }
        
        // Similar for a single RGBC reading
        if(cont == 0x01){
            // Send to PC
            char string2[40];
            sprintf(string2,"\n%05d %05d %05d %05d\n",initial_color.R,initial_color.G,initial_color.B,initial_color.C);
            TxBufferedString(string2);
            // Raise interrupt flag for transmission if buffer is not empty
            sendTxBuf();
            cont = 0x00; // Reset cont value
            a = 0; // Reset a also
        }
        
        ************************************************************************/
        
        /**********************************************
         * This section we will send the decided color via serial for testing
        ***********************************************/
        unsigned char color_detected;
        color_detected = detect_color(&initial_color);
        // Define a control character which will be received from the PC
        char cont = 0x00; // 0 is initial control value
        // Get control value from PC
        while(isDataInRxBuf()){
            // Set cont to this new value
            cont = getCharFromRxBuf();
        }
        // Send value via Serial
        if(cont == 0x01){
            // Send to PC
            char string2[40];
            sprintf(string2,"\nColor Detected = %02d %05d %05d %05d\n",color_detected,initial_color.R,initial_color.G,initial_color.B);
            TxBufferedString(string2);
            // Raise interrupt flag for transmission if buffer is not empty
            sendTxBuf();
            cont = 0x00; // Reset cont value
        }
        __delay_ms(100);
    }
}

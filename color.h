#ifndef _color_H
#define _color_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

//definition of RGB structure
typedef struct RGB_val { 
	unsigned int R; // RED
	unsigned int G; // GREEN
	unsigned int B; // BLUE
    unsigned int C; // CLEAR
} RGB_val;

/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);

/********************************************//**
 *  Functions to read the red, green, blue and clear channels
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Red(void);
unsigned int color_read_Green(void);
unsigned int color_read_Blue(void);
unsigned int color_read_Clear(void);

// Function to update colors read
void Update_RGBC(RGB_val *tempval);
// Function to decide color detected
unsigned char detect_color(RGB_val *tempval);

#endif

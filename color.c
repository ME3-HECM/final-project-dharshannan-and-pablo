#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "interrupts.h"

void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xD5);
}

void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

// Functions to read the colors

unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Green(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at GREEN low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Blue(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Clear(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

// Get RGBC values and update the structure RGBC

void Update_RGBC(RGB_val *tempval){
    // Call read color functions to update the color values 
    tempval->R = color_read_Red();
    tempval->G = color_read_Green();
    tempval->B = color_read_Blue();
    tempval->C = color_read_Clear();
}

// Function to detect and decide what color is sensed by light sensors based of RGBC % distribution values
unsigned char detect_color(RGB_val tempval)
{
    unsigned char color = 0; // Initialize color detected, 1:red, 2:green, 3:blue, 4:yellow, 5:pink
    // 6:orange, 7:light blue, 8:white
    // Calculate % dist of RGB values
    unsigned char dist_R, dist_G, dist_B; // Variables to hold % dist for red, green and blue
    dist_R = ((float)(tempval.R)/(float)(tempval.R + tempval.G + tempval.B))*100;
    dist_G = ((float)(tempval.G)/(float)(tempval.R + tempval.G + tempval.B))*100;
    dist_B = ((float)(tempval.B)/(float)(tempval.R + tempval.G + tempval.B))*100;
    
    if(color_flag){ // If color flag is 1 (flagged) check % dist for RGB values and decide color (use a confidence interval of 3%)
        __delay_ms(500); // Delay to allow readings to stabilize
        if((72<=dist_R) && (dist_R<=78) && (8<=dist_G) && (dist_G<=14) && (11<=dist_B) && (dist_B<=17)){ // Distribution of RGB values for RED
            color = 1; // Set color to RED
        }
        else if((34<=dist_R) && (dist_R<=40) && (39<=dist_G) && (dist_G<=45) && (18<=dist_B) && (dist_B<=24)){ // Dist for GREEN
            color = 2; // Set color to GREEN
        }
        else if((28<=dist_R) && (dist_R<=34) && (32<=dist_G) && (dist_G<=38) && (31<=dist_B) && (dist_B<=37)){ // Dist for BLUE
            color = 3; // Set color to BLUE
        }
        else if((50<=dist_R) && (dist_R<=52) && (30<=dist_G) && (dist_G<=32) && (17<=dist_B) && (dist_B<=19)){ // Dist for YELLOW
            color = 4; // Set color to YELLOW
        }
        else if((48<=dist_R) && (dist_R<=50) && (28<=dist_G) && (dist_G<=30) && (21<=dist_B) && (dist_B<=23)){ // Dist for PINK
            color = 5; // Set color to PINK
        }
        else if((57<=dist_R) && (dist_R<=63) && (20<=dist_G) && (dist_G<=26) && (14<=dist_B) && (dist_B<=20)){ // Dist for ORANGE
            color = 6; // Set color to ORANGE
        }
        else if((31<=dist_R) && (dist_R<=37) && (36<=dist_G) && (dist_G<=42) && (24<=dist_B) && (dist_B<=30)){ // Dist for LIGHT BLUE
            color = 7; // Set color to LIGHT BLUE
        }
        else if((41<=dist_R) && (dist_R<=47) && (30<=dist_G) && (dist_G<=36) && (20<=dist_B) && (dist_B<=26)){ // Dist for WHITE
            color = 8; // Set color to WHITE
        }
        
        color_flag = 0; // Reset color flag
        return color;
    }
    //return color; // Return the value of the decided color
}

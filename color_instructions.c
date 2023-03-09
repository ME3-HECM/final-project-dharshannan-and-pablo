#include <xc.h>
#include "dc_motor.h"

/*********************************************************************
 // Here we will define the different colored card movement instructions
**********************************************************************/

void RedInstructions(DC_motor *mL, DC_motor *mR){
    
    unsigned char a=0;
    while (a<8){
        fullSpeedBackwards(mL, mR); // Reverse Buggy a little as it might be too close to wall
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(250); // Delay before executing turn
    turnRight90(mL, mR); 
}

void GreenInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<8){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(250); 
    turnLeft90(mL, mR);
}

void BlueInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<8){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(500);
    
    // Turn 180 deg in 2 bursts of 90deg
    turnLeft90(mL, mR);
    __delay_ms(50);
    turnLeft90(mL, mR);
}


void YellowInstructions(DC_motor *mL, DC_motor *mR){
    
    unsigned char a=0;
    while (a<20){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(500);
    turnRight90(mL, mR);
}

void PinkInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<20){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(500);
    turnLeft90(mL, mR);
    
}

void OrangeInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<20){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(500);
    
    turnRight135(mL, mR);
}

void LightBlueInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<20){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(500);
    
    turnLeft135(mL, mR);
}

// Function to call the above functions based on detected color
void MoveBuggy(unsigned char *color_detected, DC_motor *mL, DC_motor *mR){
    if (color_detected==1){
        RedInstructions(mL, mR);
    }
    else if (color_detected==2){
        GreenInstructions(mL, mR);
    }
    else if (color_detected==3){
        BlueInstructions(mL, mR);
    }
    else if (color_detected==4){
        YellowInstructions(mL, mR);
    }
    else if (color_detected==5){
        PinkInstructions(mL, mR);
    }
    else if (color_detected==6){
        OrangeInstructions(mL, mR);
    }
    else if (color_detected==7){
        LightBlueInstructions(mL, mR);
    }
}
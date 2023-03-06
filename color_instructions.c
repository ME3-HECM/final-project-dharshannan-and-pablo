
#include <xc.h>
#include "dc_motor.h"

void RedInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<30){
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

void GreenInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<30){
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

void BlueInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<30){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(500);
    
    turn180(mL, mR);
}


void YellowInstructions(DC_motor *mL, DC_motor *mR){
    
    unsigned char a=0;
    while (a<60){
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
    while (a<60){
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

void OrangeInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<30){
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
    while (a<30){
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

void MoveBuggy(unsigned char color_detected, DC_motor *mL, DC_motor *mR){
    if (color_detected==1){
        RedInstructions(mL, mR);
    }
    if (color_detected==2){
        GreenInstructions(mL, mR);
    }
    if (color_detected==3){
        BlueInstructions(mL, mR);
    }
    if (color_detected==4){
        YellowInstructions(mL, mR);
    }
    if (color_detected==5){
        PinkInstructions(mL, mR);
    }
    if (color_detected==6){
        OrangeInstructions(mL, mR);
    }
    if (color_detected==7){
        LightBlueInstructions(mL, mR);
    }
}
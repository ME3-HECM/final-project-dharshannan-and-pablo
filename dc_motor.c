#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and CCP for DC motor control
void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM  
    LATEbits.LATE2 = 0;
    TRISEbits.TRISE2 = 0;
    LATEbits.LATE4 = 0;
    TRISEbits.TRISE4 = 0;
    LATCbits.LATC7 = 0;
    TRISCbits.TRISC7 = 0;
    LATGbits.LATG6 = 0;
    TRISGbits.TRISG6 = 0;
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 config
    T2CONbits.CKPS=0b100; // 1:16 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    // 0.0001s*16MHz/16 -1 = 99
    T2PR=PWMperiod; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles 
    CCPR1H=0; 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
}

// function to set CCP PWM output from the values in the motor structure
void setMotorPWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty=m->PWMperiod; //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

//function to stop the robot gradually 
void stop(DC_motor *mL, DC_motor *mR)
{   
    // Set the brake mode for both left and right motors to be 1 (so here we will slow decay)
    mL->brakemode = 1;
    mR->brakemode = 1;
    // Check if motor power is below 0, if yes reset to 0 (saturate)
    if(mL->power<0){mL->power=0;}
    if(mR->power<0){mR->power=0;}
    // Set the left and right motor powers to decrement gradually 
    mL->power = mL->power - 5 ;
    mR->power = mR->power - 5;
    // Call function for both left and right motor to setPWM
    mL->power = 0;
    mR->power = 0;
    setMotorPWM(mL);
    setMotorPWM(mR);
    __delay_ms(10); // Delay the decrementation of power
}

//function to make the robot turn left (*from the front of the Buggy)
void turnLeft(DC_motor *mL, DC_motor *mR)
{
    // Set left motor direction to be forward and brakemode to be zero 
    mL->direction = 0;
    mL->brakemode = 0;
    // Similar as above for right motor but make right move backwards
    mR->direction = 1;
    mR->brakemode = 0;
    
    // Check if motor power is above 100, if yes reset to 100 (saturate)
    if(mL->power>62){mL->power=62;}
    if(mR->power>62){mR->power=62;}
    // Set the left and right motor powers to increment gradually 
    mL->power = mL->power + 10 ;
    mR->power = mR->power + 10;
    // Call function for both left and right motor to setPWM
    setMotorPWM(mL);
    setMotorPWM(mR);
    __delay_ms(20); // Delay the incrementation
}

//function to make the robot turn right (*from the front of the Buggy)
void turnRight(DC_motor *mL, DC_motor *mR) 
{
    // Set left motor direction to be backwards and brakemode to be zero 
    mL->direction = 1;
    mL->brakemode = 0;
    // Similar as above for right motor but make right move forwards
    mR->direction = 0;
    mR->brakemode = 0;
    
    // Check if motor power is above 100, if yes reset to 100 (saturate)
    if(mL->power>62){mL->power=62;}
    if(mR->power>62){mR->power=62;}
    // Set the left and right motor powers to increment gradually 
    mL->power = mL->power + 10 ;
    mR->power = mR->power + 10;
    // Call function for both left and right motor to setPWM
    setMotorPWM(mL);
    setMotorPWM(mR);
    __delay_ms(20); // Delay the incrementation
}

//function to make the robot go straight
void fullSpeedAhead(DC_motor *mL, DC_motor *mR)
{   
    // Set left motor direction to be forward and brakemode to be zero 
    mL->direction = 1;
    mL->brakemode = 0;
    // Similar as above for right motor
    mR->direction = 1;
    mR->brakemode = 0;

    // Check if motor power is above 50, if yes reset to 50 (saturate)
    // For maze navigation lets move a little slower (initially 75% power), lets only have it at 50%
    if(mL->power>40){mL->power=40;}
    if(mR->power>43){mR->power=43;} // Calibrate right motors (to make Buggy move reasonably straight)
    // Set the left and right motor powers to increment gradually 
    mL->power = mL->power + 5 ;
    mR->power = mR->power + 7; // Calibrate right motors (to make Buggy move reasonably straight)
    // Call function for both left and right motor to setPWM
    setMotorPWM(mL);
    setMotorPWM(mR);
    __delay_ms(50); // Delay the incrementation
    
}

void fullSpeedBackwards(DC_motor *mL, DC_motor *mR){
     
    // Set left motor direction to be rearwards and brakemode to be zero 
    mL->direction = 0;
    mL->brakemode = 0;
    // Similar as above for right motor
    mR->direction = 0;
    mR->brakemode = 0;

    // Check if motor power is above 50, if yes reset to 50 (saturate)
    if(mL->power>38){mL->power=38;}
    if(mR->power>46){mR->power=46;} // Calibrate right motors (to make Buggy move reasonably straight)
    // Set the left and right motor powers to increment gradually 
    mL->power = mL->power + 5 ;
    mR->power = mR->power + 8; // Calibrate right motors (to make Buggy move reasonably straight)
    // Call function for both left and right motor to setPWM
    setMotorPWM(mL);
    setMotorPWM(mR);
    __delay_ms(50); // Delay the incrementation
}

/*****************************************************
 Based on calibration the values below gives reasonably
 consistent turnings (might require more calibrating)
******************************************************/

void turnLeft90(DC_motor *mL, DC_motor *mR){ // THIS MIGHT NEED TO BE A LITTLE LONGER THAN RIGHT
    
    unsigned char a=0;
    while(a<16){ // (Calibration of 16 gives 45 deg turn but may differ for different surfaces and buggy acceleration)
        turnLeft(mL, mR); // Make the Buggy turn left
        a++;
    }
    while(a>0){
        stop(mL, mR); // Stop turn 
        a--;
    }
    while(a<15){ // (Calibration of 16 gives 45 deg turn but may differ for different surfaces and buggy acceleration)
        turnLeft(mL, mR); // Make the Buggy turn left
        a++;
    }
    while(a>0){
        stop(mL, mR); // Stop turn 
        a--;
    }
}

void turnRight90(DC_motor *mL, DC_motor *mR){
    
    unsigned char a=0;
    while(a<15){ // (Calibration of 15 gives 45 deg turn but may differ for different surfaces and buggy acceleration)
        turnRight(mL, mR); // Make the Buggy turn right
        a++;
    }
    while(a>0){
        stop(mL, mR); // Stop turn 
        a--;
    }
    while(a<15){ // (Calibration of 15 gives 45 deg turn but may differ for different surfaces and buggy acceleration)
        turnRight(mL, mR); // Make the Buggy turn right
        a++;
    }
    while(a>0){
        stop(mL, mR); // Stop turn 
        a--;
    }
}

// 180deg function not used (instead call 90deg twice)
void turn180(DC_motor *mL, DC_motor *mR){
    
    turnLeft90(mL,mR);
    turnLeft90(mL,mR);
}

void turnRight135(DC_motor *mL, DC_motor *mR){
    
    turnRight90(mL,mR);
    unsigned char a=0;
    while(a<15){ // (Calibration of 15 gives 45 deg turn but may differ for different surfaces and buggy acceleration)
        turnRight(mL, mR); // Make the Buggy turn right
        a++;
    }
    while(a>0){
        stop(mL, mR); // Stop turn 
        a--;
    }
}

void turnLeft135(DC_motor *mL, DC_motor *mR){
    
    turnLeft90(mL,mR);
    unsigned char a=0;
    while(a<16){ // (Calibration of 16 gives 45 deg turn but may differ for different surfaces and buggy acceleration)
        turnLeft(mL, mR); // Make the Buggy turn left
        a++;
    }
    while(a>0){
        stop(mL, mR); // Stop turn 
        a--;
    }
}


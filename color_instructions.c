#include <xc.h>
#include "color_instructions.h"
#include "dc_motor.h"
#include "Memorization.h"

/*********************************************************************
 // Here we will define the different colored card movement instructions
**********************************************************************/

void RedInstructions(DC_motor *mL, DC_motor *mR){
    
    unsigned char a=0;
    while (a<16){
        fullSpeedBackwards(mL, mR); // Reverse Buggy a little as it might be too close to wall
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50); // Delay before executing turn
    turnRight90(mL, mR); 
}

void GreenInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<16){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50); 
    turnLeft90(mL, mR);
}

void BlueInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<16){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    // Turn 180 deg in 2 bursts of 90deg
    turnLeft90(mL, mR);
    __delay_ms(50);
    turnLeft90(mL, mR);
}


void YellowInstructions(DC_motor *mL, DC_motor *mR){
    
    unsigned char a=0;
    while (a<45){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    turnRight90(mL, mR);
}

void PinkInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<45){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    turnLeft90(mL, mR);
    
}

void OrangeInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<16){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    turnRight135(mL, mR);
}

void LightBlueInstructions(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    while (a<16){
        fullSpeedBackwards(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    turnLeft135(mL, mR);
}
// Anti moves function for Yellow and Pink (the opposite of each color moves)
void AntiYellow(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    // Reverse slightly and turn Right
    RedInstructions(mL, mR);
    __delay_ms(50);
    
    while (a<45){
        fullSpeedAhead(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    // Turn 180deg at the end of Anti Yellow
    BlueInstructions(mL, mR);
}

void AntiPink(DC_motor *mL, DC_motor *mR){
    unsigned char a=0;
    // Reverse slightly and turn Left
    GreenInstructions(mL, mR);
    __delay_ms(50);
    
    while (a<45){
        fullSpeedAhead(mL, mR);
        a++;
    }
    while (a>0){
        stop(mL, mR);
        a--;
    }
    __delay_ms(50);
    // Turn 180deg at the end of Anti Pink
    BlueInstructions(mL, mR);
}

// Function for White Card track back
void WhiteInstructions(DC_motor *mL, DC_motor *mR){
    PIE0bits.INT1IE = 0; // Disable interrupt on RB1 pin (disable color-click interrupt)
    // Execute Blue instruction for 180 deg turn 1st
    BlueInstructions(mL,mR);
    // Now we move the Buggy using a while loop based on the time index
    while(time_index>0){ // While the time index is greater than 0
        // Move forward based on index (starting from the last in the time array)
        unsigned int a = 0; // Temporary variable to time forward movement
        unsigned int c = Return_Time(&time_index,time_array);
        while(a<c){ // Check the time index, and decrement it using the return time function
            fullSpeedAhead(mL,mR); // Move forward
            a++; // increment a
        }
        // Stop motor to stop buggy
        while(a>0){
            stop(mL,mR); // Call stop function
            a--; // decrement a
        }
        // Now pass the indexed value in the anti-move array (starting from the last index and indexing backwards)
        unsigned char anticolor = Return_Anti_Moves(&moves_index, anti_moves_array);
        MoveBuggy(anticolor,mL,mR);
    }
}

// Function to call the above functions based on detected color
// Call the append anti-moves functions to append the anti-moves of the executed moves
void MoveBuggy(unsigned char *color_detected, DC_motor *mL, DC_motor *mR){
    if (color_detected==1){
        RedInstructions(mL, mR);
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(2,&moves_index,anti_moves_array); // The anti-move of Red is Green
        }
    }
    else if (color_detected==2){
        GreenInstructions(mL, mR);
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(1,&moves_index,anti_moves_array); // The anti-move of Green is Red
        }
    }
    else if (color_detected==3){
        BlueInstructions(mL, mR);
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(3,&moves_index,anti_moves_array); // The anti-move of Blue is itself
        }
    }
    else if (color_detected==4){
        YellowInstructions(mL, mR);
        // Separate anti-move function
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(9,&moves_index,anti_moves_array); // The anti-move of Blue is itself
        }
    }
    else if (color_detected==5){
        PinkInstructions(mL, mR);
        // Separate anti-move function
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(10,&moves_index,anti_moves_array); // The anti-move of Blue is itself
        }
    }
    else if (color_detected==6){
        OrangeInstructions(mL, mR);
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(7,&moves_index,anti_moves_array); // The anti-move of Orange is Light Blue
        }
    }
    else if (color_detected==7){
        LightBlueInstructions(mL, mR);
        if(LATDbits.LATD7 != 1){ // Only append when the Buggy is not in track back mode (indicated by RD7 LED)
            AppendMoves(6,&moves_index,anti_moves_array); // The anti-move of Light Blue is Orange
        }
    }
    // White instructions (8) will be called outside of this function to prevent recursive function call error (called in main.c)
    // values of 9 and 10 will be used to call anti-Yellow and anti-Pink instructions
    else if (color_detected==9){
        AntiYellow(mL, mR);
    }
    else if (color_detected==10){
        AntiPink(mL, mR);
    }
}
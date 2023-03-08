#ifndef _color_instructions_H
#define _color_instructions_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void RedInstructions(DC_motor *mL, DC_motor *mR);
void GreenInstructions(DC_motor *mL, DC_motor *mR);
void BlueInstructions(DC_motor *mL, DC_motor *mR);
void YellowInstructions (DC_motor *mL, DC_motor *mR);
void PinkInstructions(DC_motor *mL, DC_motor *mR);
void OrangeInstructions(DC_motor *mL, DC_motor *mR);
void LightBlueInstructions(DC_motor *mL, DC_motor *mR);

void MoveBuggy(unsigned char *color_detected, DC_motor *mL, DC_motor *mR);

#endif	/* XC_HEADER_TEMPLATE_H */


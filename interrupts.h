#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

// Variable declarements
extern unsigned char tmr_ovf; // Timer overflow flag
extern unsigned char color_flag; // Color card flag (flagged when any colored card is found)
extern unsigned char lost_flag; // Lost flag
extern unsigned int int_threshold_low; // Low thershold for color click interrupts
extern unsigned int int_threshold_high; // High thershold for color click interrupts

// Function Prototypes
void Interrupts_init(void);
void init_colorclick_interrupts(void);
void interrupts_clear_colorclick(void);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

#endif

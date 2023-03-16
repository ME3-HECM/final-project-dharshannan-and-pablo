#ifndef Memorization_H
#define	Memorization_H

#include <xc.h> 

// Initialize index and arrays to be global variables
extern unsigned char moves_index; // Initialize move index for move memorization array
extern unsigned char time_index; // Initialize time index for time memorization array

extern unsigned char anti_moves_array[40]; // Array holding the anti moves of executed movements
extern unsigned int time_array[40]; // Array holding the time for forward movement of Buggy

// Function Prototypes
void AppendMoves(unsigned char temp, unsigned char *moves_index, unsigned char *anti_moves_array);
void AppendTime(unsigned int temp, unsigned char *time_index, unsigned int *time_array);
unsigned char Return_Anti_Moves(unsigned char *moves_index, unsigned char *anti_moves_array);
unsigned int Return_Time(unsigned char *time_index, unsigned int *time_array);

#endif


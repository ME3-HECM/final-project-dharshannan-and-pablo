#include <xc.h>
#include "Memorization.h"

// Functions to hold in memory the time the Buggy moves forward and moves it has made
// This will contain 2 arrays, one to hold the time, the other the anti-moves of executed movements

unsigned char moves_index = 0; // Initialize move index for move memorization array
unsigned char time_index = 0; // Initialize time index for time memorization array

unsigned char anti_moves_array[40]; // Array holding the anti moves of executed movements
unsigned char time_array[40]; // Array holding timer values for forward movement of Buggy

/*******************************************************************************
// Functions to append antimoves and timer values to each of the above arrays
********************************************************************************/
// Append Moves Function
void AppendMoves(unsigned char temp, unsigned char *moves_index, unsigned char *anti_moves_array){ // Both moves index and the passed array are pointers
    // Append the anti moves to the array and increment the moves index
    anti_moves_array[(*moves_index)++] = temp; // Temp value will hold the anti-moves corresponding to the color values
}

// Append Time Function
void AppendTime(unsigned char temp, unsigned char *time_index, unsigned char *time_array){ // Similar to AppendMoves function above
    // Append the time values for forward movement (We can try different approaches to this *using Timer0 module or *using forward movement incremntation variable)
    time_array[(*time_index)++] = temp; // Temp will hold either the Timer0 or forward incrementation values (experiment on which is better)
}

/************************************************************************************************************************
// Functions to traverse the functions backwards for both moves and time and return the value of the function indexed to
*************************************************************************************************************************/
// Traverse Moves Array Function
unsigned char Return_Anti_Moves(unsigned char *moves_index, unsigned char *anti_moves_array){
    unsigned char temp; // Temp variable to return indexed function value
    // Traverse the array backwards and set temp to the indexed value (decrement index value)
    temp = anti_moves_array[--(*moves_index)];
    return temp; // Return color associated with anti-move
}

// Traverse Time Array Function
unsigned char Return_Time(unsigned char *time_index, unsigned char *time_array){
    unsigned char temp; // Temp variable to return the indexed time for forward movement
    // Traverse the array backwards and set temp to the indexed value (decrement index value)
    temp = time_array[--(*time_index)];
    return temp; // Return the time for forward movement
}


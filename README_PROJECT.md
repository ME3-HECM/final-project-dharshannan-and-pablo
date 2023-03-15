# Mini project - Energy saving automatic outside light - Dharshannan and Pablo
## Table of contents
- [Instructions to users](#instructions-to-users)
- [Hardware setup](#hardware-setup)
- [Demonstration video](#demonstration-video)
- [Program structure](#program-structure)
- [Achieving sun synchronocity](#achieving-sun-synchronocity)
- [Learning outcomes](#learning-outcomes)
- [Project brief](#project-brief)
- [Project specification](#project-specification)
- [Supplementary information and help](#supplementary-information-and-help)



## Instructions to users
I do not think we need this section.


## Hardware setup
Our hardware has been set up as in Lab 4:
- The **LED on pin RD7** is used as to signal when the buggy has detected a colour.
- The **Colour Clicker** is used to detect the colours as RGB values.

[Insert Picture of the Buggy here]


## Demonstration video
Our video demonstrates the operation of the buggy in the medium level maze, showing that it works for almost all colours.

[Insert video of the medium maze buggy here]


## Program structure
Our program has been structured as follows:
1. ```"main.c"```

We define the main structures used along the code and the basic instructions to be completed by the buggy when colours are detected. 

The source file contains our main function. We start by manually the initial color values for Red, Green, Blue and Clear. We also start the initialization sequences for the colour clicker, LED, Interrupts and Timer0. We then proceed by defining the twostrcutures for both right and left motors and set the PWM period to 99. 

1. ```"color.c/h"```

    The header file defines a structure ```RGB_val``` which is used throughout the project:
    ```
    typedef struct RGB_val {
        unsigned int R;
        unsigned int G;
        unsigned int B;
        unisgned int C;
    } RGB_val;
    ```
    
    The source file contains 2 key sets of functions:
    - The colour clicker initialization functions. Allows the color clicker to detetect Red, Green Blue and clear
    - ```detect_color(RGB_val *tempval)``` which initialises the color ddetected and checks which colour it has detected given the confidence the value and confidence intervals manuallt set after extensive data Analysis. 

1. ```"color_instructions.c/h"```

    The header file simply contains the function prototypes from the corresponding source file.
    
    The source file contains functions that contain the instructions to be performed by the buggy for every colour as well as the inverse functions when needed for the white card instructions.

1. ```"dc_motor.c/h"```

 The header file defines a structure ```DC_motor``` which is used throughout the project:
    ```
    typedef struct DC_motor {
        signed char power;
        char direction;
        char brakemode;
        unsigned int PWMperiod;
        unsigned char *posDutyHighByte;
        unsigned char *negDutyHighByte:
    } DC_motor;
    ```
    
    The source file contains functions that initialise the different movements that the buggy can make as well as initialising the DCmotors PWM nad setting the motor PWM. Some of the movements menstioned include moving forwards, backwards, breaking, turning right and turning left. 

1. ```"i2c.c/h"```
    .........................................................
    .........................................................
    .........................................................
    .........................................................
    .........................................................
    .........................................................


1. ```"interrupts.c/h"```
    
    The header file simply contains the function prototypes from the corresponding source file.

    The source file turns on interrupts, and defines:
    - a high priority interrupt every time the buggy detects a colour;
    - a low priority interrupt every time the timer overflows (1 second has passed).

1. ```"LED_Buttons.c"```
    
    The header file simply contains the function prototypes from the corresponding source file.
    
    The source file contains functions to initialie all the LED's and the function to turn on all the red, green and blue LEDs to produce white light on the color clicker

1. ```"Memorization.c/h"```
    The header file simply contains the function prototypes from the corresponding source file and initialises move index and arrays to be global variables.  
    
    The source contains functions to hold in memory the time the buggy moves forward and the moves it has made
    
1. ```"serial.c/h"```

    The source file contains functions to send information to a computer through the serial port


1. ```"timers.c/h"```

    The header file defines the prescaler values and Timer0 initial bits for both the "testing mode" and "normal mode".
    
    The source file initialises Timer0 to be used in the low priority interrupt (to increment out time each second).
    



## Learning outcomes
The principal learning objectives for this project are:
- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions

## Learning outcomes

The principal learning objectives for this project are:

- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions

## Brief

Outside lights that respond to ambient light levels are commonplace (i.e. street lights, porch lights, garden lights etc). These types of lights switch on at dusk and then turn off at dawn. However, energy and money can be saved if these lights are switched off during the small hours of the morning (for example, between 1am and 5am), when there are very few people around. Many councils in the UK have implemented/trialled this idea for street lights (https://www.bbc.co.uk/news/uk-england-derbyshire-16811386). Your task is to use the knowledge of microcontrollers and hardware that you have gained in labs 1-3 from this module to develop a fully automated solution.

## Specification
Design and program a device that meets the following requirements:

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displays the current hour of day on the LED array in binary
1. Turns the light off between approx. 1am and 5am
1. Adjusts for daylight savings time
1. Maintain synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Please use this GitHub repo to manage your software development and submit your mini project code.

## Supplementary information and help
At first the task may seem quite straightforward but there are several points that often prove more tricky. The first is how to test code during development? You could test in real world conditions but you would be limited to one test cycle per day and this would severely slow down your development and debugging progress. To get around this you could implement a "testing mode" and pretend that a day lasts 24 seconds. This could be done using a #define directive to switch between "normal" and "testing" modes for your code.

Adjusting for daylight savings time is not too tricky. The clocks always change (in the UK) on the last Sunday in March (they go forward an hour) and the last Sunday in October (they go back an hour). One method of achieving this is to initialise what day it is when device is first switched on (using manual input) and then keep track of the days that pass and what the day of the week it is. Another method might be to automatically figure out what time of year it is (see below). Also don't forget about leap years! 

No clock is perfect, they can all run slightly fast/slow and can by influenced be external factors such as temperature. Ultimately this will result in drift over time and eventually the time will drift so far out of sync with real time that it is meaningless. For the purposes of our device the main requirement is that it remains in sync with the sun. You could use light from the sun to keep your clock in sync. Although the length of daylight varies considerably during the year, the midpoint between dusk and dawn only varies by a few minutes. This is termed solar midnight approx. 12am or solar noon approx. 12pm. One method of staying in sync with the sun is to use the LDR and record/calculate when these times occur and adjust your clock accordingly. The length of daylight also tells us information about what time of year it is and can be used to help us know when to adjust for daylight savings time.

![Day length](gifs/day-length-london.jpg)
http://wordpress.mrreid.org/2010/10/31/why-change-the-clocks/





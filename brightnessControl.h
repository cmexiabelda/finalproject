#ifndef brightnessControl_h
#define brightnessControl_h
#include "mbed.h"
 
 
extern float brightnessLevel;
 
extern void getSoilmeasure();
extern void get_brightness();
extern void commsLoop();
#define LED_RED PH_0
#define LED_GREEN PH_1
#define LED_BLUE PB_13
 
#define BLACK 000
#define RED 110
#define GREEN 101
#define BLUE 011
#define MAGENTA 010
#define CYAN 001
#define YELLOW 100
#define WHITE 111
 
#define MAX_SOIL 50
#define MIN_SOIL 25
#define MAX_LIGHT 75
#define MIN_LIGHT 25
 

extern float soil_value;
extern float light;
 

#endif
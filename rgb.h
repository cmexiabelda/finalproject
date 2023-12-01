#ifndef rgb_h
#define rgb_h
 
#include "mbed.h"
#define Add_TCS34725 0x29  //dir estandar
#define COMMAND_BIT 0x80 //Command bit
#define ENABLE 0x00      //Interrupt Enable register
#define CDATAL COMMAND_BIT | 0x14 //OR, quiero leer los datos del canal
#define BDATAL COMMAND_BIT | 0x1A // BLUE
#define RDATAL COMMAND_BIT | 0x16 // RED
#define GDATAL COMMAND_BIT | 0x18 // GREEN

 

#define BLACK 111
#define RED 110
#define GREEN 101
#define BLUE 011
#define MAGENTA 010
#define CYAN 001
#define YELLOW 100
#define WHITE 000

#define MAX_COLOR 255
#define MIN_COLOR 0


extern uint16_t rgb_clear;
extern uint16_t rgb_red;
extern uint16_t rgb_green;
extern uint16_t rgb_blue;
extern uint16_t highestValue;
 
extern uint8_t dom_col;
extern char colorines[5];

extern void rgbmeasure();
extern void setup();
#endif
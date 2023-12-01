#ifndef LED_H
#define LED_H

#include "mbed.h"
#include "PinNames.h"

#define LED_RED PH_0
#define LED_GREEN PH_1
#define LED_BLUE PB_13

#define BLACK 111
#define RED 110
#define GREEN 101
#define BLUE 011
#define MAGENTA 010
#define CYAN 001
#define YELLOW 100
#define WHITE 000

extern BusOut rgbLed;
extern void setRGBColor (uint8_t colorled);

#endif
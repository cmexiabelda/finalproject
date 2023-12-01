#include "led.h"
#include "main.h"
BusOut rgbLed(PH_0, PH_1, PB_13);

void setRGBColor (uint8_t colorled) {
    rgbLed = colorled;
}

void setLedColor(int color) {
    rgbLed = color;
}

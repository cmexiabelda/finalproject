
#ifndef MAIN_H
#define MAIN_H
 
#include "mbed.h"
#include "PinNames.h"
#include <cstdint>
 
extern uint8_t hour_cnt;
 
extern I2C i2c;
extern bool isMeasureEnabled;

typedef enum {TEST, NORMAL, ADVANCED} t_state;
extern t_state mode;
extern bool enable_printf;

// FOR ADVANCED MODE
struct measureColor {
    uint8_t color;
    bool activated;
};

#endif

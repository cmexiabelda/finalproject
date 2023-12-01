#ifndef MEASURE_H
#define MEASURE_H
 
#include "mbed.h"
#include "PinNames.h"
#include <cstdint>
 
extern bool enable_printf;
 
extern uint8_t cnt;
 
extern float mean_temp;
extern float max_temp;
extern float min_temp;
 
extern float mean_hum;
extern float max_hum;
extern float min_hum;
 
extern float mean_soil;
extern float max_soil;
extern float min_soil;
 
extern float mean_light;
extern float max_light;
extern float min_light;

 
 
//extern void accMeasure();
 
extern void th_measures(void);
 
#endif
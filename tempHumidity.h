#ifndef tempHumidity_h
#define tempHumidity_h
#include "mbed.h"
 
#define HUMIDITY_CMD 0xE5 // Hold master
#define TEMP_CMD 0xE3 // Hold master
#define SLAVE_ADDRESS 0x40 << 1//0x80 1 bit izqqqqq
extern float temperature, humidity;
 
#include "main.h"
 
#define MIN_TEMP -10
#define MAX_TEMP 50
 
#define MIN_HUM 25
#define MAX_HUM 75
 
 
extern void getRelativeHumidity();
extern void getTemperature();
 
 
 
#endif
#ifndef GPS_H
#define GPS_H
 
#include "main.h"
 
extern float longitude;
extern float latitude;
extern float timeutc;  
extern int sats;    
extern float alt;
 
extern int horas;
extern int minutos;
extern int segundos;
 
extern char ns, ew;
 
extern void gpsInit();
extern void gpsMeasure();
 
#endif
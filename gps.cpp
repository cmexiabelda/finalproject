#include "gps.h"
#include <string>
 
 
float longitude;
float latitude;
float timeutc;  
int sats;    
char ns, ew, unit;
float hdop;
float alt;
float geoid;
int quality; //QoS 1
float degrees;
float minutes;
char msg[256];
char line[256];
BufferedSerial uart(PA_9,PA_10);
int horas;
int minutos;
int segundos;
 
void gpsInit(){
    uart.set_format(//serial line data transmission
        8, //bits
        BufferedSerial::None, //parity
       1 //stop
    );
    uart.set_baud(9600);//vel trans
    longitude = 0.0;
    latitude = 0.0;        
}

float roundd(float v) {
    if (v < 0.0) {
        return -floor(-v); //paso a abs
    } else {
        return floor(v);
    }
}

 
 
void buscarString() {
    int i = 0;  //pos en msg
    // inicio de linea ('$')
    while (msg[i++] != '$');
    for (int j = 0; j < 256; j++) {
        line[j] = msg[i++];  //copiar la linea entera
        if (msg[i] == '\r') {  //terminar el string al encontrar el fin d linea
            line[j] = 0;
            return;
        }
    }
}


 
void gpsMeasure(){
    uart.read(msg, sizeof(msg));//calculo el tamaño de los datos
    buscarString();
    //time, lat, dir lat, long, dir long, calidad DE FIJACION (GPS), num sats, dilución horizontal de la precisión (HDOP), alt nivel mar, ud alt, id estacion gps
    //$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
    //leo la cadeena en el formato q le especifico
    sscanf(line, "GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c,%f", &timeutc, &latitude, &ns, &longitude, &ew, &quality, &sats, &hdop, &alt, &unit, &geoid);
    if(ns == 'S') {//direccion latt (N-S)
        latitude  *= -1.0;}
    if(ew == 'W') { //dir long (E-W)
        longitude *= -1.0;}
    //LAT: 2307.1256 ddmm.mmmm
    degrees = roundd(latitude / 100.0f);
    //2307.1256-(23*100)=07.1256
    minutes = latitude - (degrees * 100.0f);
//23+07.1256/60-----grados decimales
    latitude = degrees + minutes / 60.0f;    
    degrees = roundd(longitude / 100.0f * 0.01f);
    minutes = longitude - (degrees * 100.0f);
    longitude = degrees + minutes / 60.0f;
//hhmmss.ss
//asumimos que las horas están representadas por los primeros dos dígitos del tiempo UTC
//064951.000
    int ent = (int)timeutc;    
//0.0
    double parteDecimal = timeutc - ent;
 //horas = 64951 / 10000=6
    horas = ent / 10000;  
    //modulo para tener los restantes de la op anterior y divido para sacar los mins
//minutos = (64951 % 10000) / 100 = 49
    minutos = (ent % 10000) / 100;
    //segundos = 64951 % 100 = 51
    segundos = ent % 100;
}
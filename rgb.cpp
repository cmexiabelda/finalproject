#include "mbed.h"
#include "PinNames.h"
#include "rgb.h"
extern I2C i2c;
 
uint16_t rgb_clear;
uint16_t rgb_red;
uint16_t rgb_green;
uint16_t rgb_blue;
uint16_t highestValue;
 
 
uint16_t read_color(uint8_t reg);
char colorines[5];

uint8_t dom_col;
 

 
void rgb_setup() {
    //habilitar sensor e indicar que se esta mandando un comando y no un dato
    uint8_t buffer[2] = {ENABLE | COMMAND_BIT, 0x03};
    //se desplaza 1bit a la izq para incluir el bit de escritura,menos significativo
    if (i2c.write(Add_TCS34725 << 1, (char*)buffer, 2) != 0) {
        printf("Error SETUP\n");
    }
}
void rgbmeasure() {
    //wait_us(3000);
    rgb_clear = read_color(CDATAL);
    rgb_red = read_color(RDATAL);
    rgb_green = read_color(GDATAL);
    rgb_blue = read_color(BDATAL);
   //printf("rgb_clear: %d, rgb_red: %d, rgb_green: %d, rgb_blue: %d\n", rgb_clear, rgb_red, rgb_green, rgb_blue);
}
 
uint16_t read_color(uint8_t reg) {//registro que quiero leer del sensor
    uint8_t buffer_tx[1] = {reg};//el buffer de 1 byte lo almacena
    //bit -sig en 0 (escritura)|||||||el contenido del buffer se envian como datos para escribirlos
    if (i2c.write(Add_TCS34725 << 1, (char*)buffer_tx, 1, true) != 0) {//se escribe solo 1byte
    //si la señal generada despues de escribir da true, se ha realizado correct y deberia ser 0
        printf("Error READCOLOR\n");
    }
    uint8_t buffer_rx[2];//datos recibidos del sesor
    if (i2c.read(Add_TCS34725 << 1, (char*)buffer_rx, 2) != 0) {//lee 2 bytes
        printf("Error READCOLOR2\n");
    }
    
 if (rgb_red > rgb_green && rgb_red > rgb_blue){
        dom_col= RED;
        strcpy(colorines, "RED");
    } else if (rgb_green > rgb_red && rgb_green > rgb_blue){
        dom_col = GREEN;
        strcpy(colorines, "GREEN");
    } else if (rgb_blue > rgb_red && rgb_blue > rgb_green){
        dom_col = BLUE;
        strcpy(colorines, "BLUE");
    }
    //los 8 bits más significativos se obtienen de buffer_rx[1] y los 8 bits menos significativos se obtienen de buffer_rx[0]
    return (buffer_rx[1] << 8) | buffer_rx[0]; //el return recoge 16 bits




}
 


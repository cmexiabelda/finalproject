#include "tempHumidity.h"
#include "mbed.h"
extern I2C i2c;
 
float temperature, humidity;
int hum_code;

void getRelativeHumidity() {

    char cmd[1];//para escribir el registro
    char buff[2];//para alm la lect del registro
    cmd[0] = 0xE5; // Humedad
    i2c.write(SLAVE_ADDRESS << 1, cmd, 1);//pos, 1 byte
    i2c.read(SLAVE_ADDRESS << 1, buff, 2);
    hum_code = (buff[0]<<8) + buff[1];//MSB LSB
    humidity = ((125.0 * hum_code)/65536) - 6;
 
}
 
    /*
    char cmd[1];
    char data[2];
    cmd[0] = HUMIDITY_CMD;
    i2c.write(SLAVE_ADDRESS, cmd, 1);
    i2c.read(SLAVE_ADDRESS, data, 2);
    int16_t rawHumidity = (data[0] << 8) | data[1];//+sign, -
    humidity = ((125.0 * rawHumidity) / 65536) - 6; // datasheet page 21*/
void getTemperature() {
    char cmd[1];
    char data[2];
    cmd[0] = TEMP_CMD;
    i2c.write(SLAVE_ADDRESS, cmd, 1);
    i2c.read(SLAVE_ADDRESS, data, 2);
    int16_t rawTemperature = (data[0] << 8) | data[1];
    temperature = ((175.72 * rawTemperature) / 65536.0) - 46.85; // datasheet page 22
}



#include "acc.h"
#include "main.h"
#include <cstdint>

// dir i2c
MMA8451Q acc(PB_9, PB_8, MMA8451_I2C_ADDRESS);
//SENSIBILITY +-2G 4096 COUNTS/G------------
float accx, accy, accz;

float max_accel_X;
float min_accel_X;
float mean_accel_X;

float max_accel_Y;
float min_accel_Y;
float mean_accel_Y;

float max_accel_Z;
float min_accel_Z;
float mean_accel_Z;

float accel[3];

void read_accel() {
  acc.getAccAxis(accel);
  accel[0] = accel[0] * 9.8;//MEDIDA EN ms2 (antes lo tenia en g)
  accel[1] = accel[1] * 9.8;
  accel[2] = accel[2] * 9.8;
  
}

MMA8451Q::MMA8451Q(PinName sda, PinName scl, int addr)
    : m_i2c(sda, scl), m_addr(addr) {//
  // activate the peripheral
  uint8_t data[2];
  data[0] = REG_CTRL_REG_1; 
  data[1] = 0x01;//registro d cntrol un 1 para activar, el bit 0 a 1
  writeRegs(data, 2);
}

//res en 14 bits
int16_t MMA8451Q::concatAxis(int16_t reg0, int16_t reg1) {//14 bits resoll
  int16_t value = (reg0 << 6) | (reg1 >> 2);//despl 6 , quiero 8 del ms y 6 les
  if (value > (UINT14_MAX / 2)) {// max 16383, 2^14, es complemento a 2
  //como me da valores negativos, -8192 a 8192
    value -= UINT14_MAX;//le cambio el signo, le resto 16mil
  }
  return value;
}
void MMA8451Q::getAccAxis(float *returnValue) {
  int16_t acc;
  uint8_t res[6];

  readRegs(REG_OUT_X_MSB, res, 6);//los vals de x +sig
//sensibilidad de 2g es de 4096 cuentas/g
  returnValue[0] = float(concatAxis(res[0], res[1])) / 4096.0;//cuentas x g----lo paso a g
  returnValue[1] = float(concatAxis(res[2], res[3])) / 4096.0;
  returnValue[2] = float(concatAxis(res[4], res[5])) / 4069.0;
}

void MMA8451Q::readRegs(int addr, uint8_t *data, int len) {//dir reg puntero, longi datos que quiero leer
  char t[1] = {(char)addr};
  m_i2c.write(m_addr, t, 1, true);//pongo a true àra la señal d parada
  m_i2c.read(m_addr, (char *)data, len);//los datos leidos se alm en data
}

void MMA8451Q::writeRegs(uint8_t *data, int len) {
  m_i2c.write(m_addr, (char *)data, len);//los datos los cojo d data y escribo los bytes q m diga el len
}

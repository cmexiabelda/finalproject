#ifndef MMA8451Q_H
#define MMA8451Q_H

#include "main.h"
#include "mbed.h"

#define REG_WHO_AM_I 0x0d
#define REG_CTRL_REG_1 0x2a
#define REG_OUT_X_MSB 0x01

#define UINT14_MAX 16383
#define MMA8451_I2C_ADDRESS (0x1d << 1)//A SIN CONECTAR
#define L_MAX_ACCE_X 10
#define L_MIN_ACCE_X -10
#define L_MAX_ACCE_Y 10
#define L_MIN_ACCE_Y -10
#define L_MAX_ACCE_Z 10
#define L_MIN_ACCE_Z -10

extern bool b_print_m;
// extern t_state automaton;

extern t_state mode;

extern float max_accel_X;
extern float min_accel_X;
extern float mean_accel_X;

extern float max_accel_Y;
extern float min_accel_Y;
extern float mean_accel_Y;

extern float max_accel_Z;
extern float min_accel_Z;
extern float mean_accel_Z;

extern float accel[3];

//  extern void check_limits (float max, float min, float value, int color);

void read_accel();
extern float accx, accy, accz;

class MMA8451Q {
public:
  /**
   * MMA8451Q constructor
   *
   * @param sda SDA pin
   * @param sdl SCL pin
   * @param addr addr of the I2C peripheral
   */
  MMA8451Q(PinName sda, PinName scl, int addr);

  /**
   * Get the value of the WHO_AM_I register
   *
   * @returns WHO_AM_I value
   */
  uint8_t getWhoAmI();

  /**
   * Get XYZ axis acceleration
   *
   * @param res array where acceleration data will be stored
   */
  void getAccAllAxis(float *res);
  int16_t concatAxis(int16_t reg0, int16_t reg1);
  void getAccAxis(float *returnValue);
  float getAxisX(uint8_t addr);

private:
  I2C m_i2c;
  int m_addr;
  void readRegs(int addr, uint8_t *data, int len);
  void writeRegs(uint8_t *data, int len);
};

#endif
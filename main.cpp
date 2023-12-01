#include "main.h"
#include "acc.h"
#include "brightnessControl.h"
#include "gps.h"
#include "led.h"
#include "mbed.h"
#include "measure.h"
#include "rgb.h"
#include "tempHumidity.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

I2C i2c(PB_9, PB_8); // SDA, SCL

static BufferedSerial serial_port(USBTX, USBRX);

extern float accel[3];

InterruptIn userbtn(PB_2);
Timer tmr;
string serialText;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
// ADVANCED MODE
DigitalOut led3(LED3);

bool isMaximumsPrintEnabled;
bool isMeasureEnabled;
bool isPrintEnabled;
void start_measure(void) { isMeasureEnabled = true; }
void print_limits(void) { isMaximumsPrintEnabled = true; }
t_state mode = TEST; // por defecto es TEST

Ticker tick;
Ticker thour;
Thread th(osPriorityNormal, 2048); // priority, stacksize
// extern int initAcc();
// extern void gpsInit();

void rgb_setup();
void gpsInit();

//    ADVANCED MODE: DOESNT WORK
Thread th_advanced(osPriorityNormal, 2048);
extern bool putAlarmInQueue(measureColor* ptrColor);
bool isAdvLimitsEnabled;
Ticker advancedTicker;

measureColor *ptr_measure_temperature = new measureColor;
measureColor *ptr_measure_humidity = new measureColor;
measureColor *ptr_measure_ambient = new measureColor;
measureColor *ptr_measure_soil = new measureColor;
measureColor *ptr_measure_leaf = new measureColor;
measureColor *ptr_measure_acc = new measureColor;
void initMeasureColors() {
    ptr_measure_temperature->color = RED;
    ptr_measure_temperature->activated = false;

    ptr_measure_humidity->color = BLUE;
    ptr_measure_humidity->activated = false;

    ptr_measure_ambient->color = GREEN;
    ptr_measure_ambient->activated = false;

    ptr_measure_soil->color = YELLOW;
    ptr_measure_soil->activated = false;

    ptr_measure_leaf->color = WHITE;
    ptr_measure_leaf->activated = false;

    ptr_measure_acc->color = MAGENTA;
    ptr_measure_acc->activated = false;
}


void advanced_check_limits (float measure, float maximum, float minimum, measureColor* c){

    if((measure > maximum) || (measure < minimum)) {
        //printf("LIMIT OFF!!\n");
        if (c->activated == false) {
            //printf("Appending color to Queue!");
            putAlarmInQueue(c);
        }
    }
}

void enableAdvancedLimits (void) {isAdvLimitsEnabled = true;}

extern void advancedModeFun();

//    END OF ADVANCED DEFINITIOS


void change_mode(void) {
  if (mode == TEST) {
    mode = NORMAL;
    tick.attach(start_measure, 30s);
    thour.attach(print_limits, 45s);
  } else if (mode == NORMAL) {
        mode = ADVANCED;
        advancedTicker.attach(enableAdvancedLimits, 3s);
        // tick.attach(start_measure, 2s);
        // thour.detach();
    } else if (mode == ADVANCED) {
        mode = TEST;
        advancedTicker.detach();
        tick.attach(start_measure, 2s);
        thour.detach();
    }
}

void limits(float measure, uint8_t maximum, uint8_t minimum,
                  uint8_t RGBcolor) {
  if (measure > maximum | measure < minimum) {
    setRGBColor(RGBcolor);
  }
}




void printMaximums() {
    printf("\nSOIL MOISTURE: Maximum: %.1f Minimum: %.1f Mean:%.1f "
               "\nLIGHT: Maximum: %.1f Minimum: %.1f Mean:%.1f \nTEMP/HUM: "
               "Temperature: Maximum: %.1f Minimum: %.1f Mean:%.1f, Humidity: "
               "Maximum: %.1f Minimum: %.1f Mean:%.1f \nMax Axis X: %.2f Min "
               "Axis X: %.2f Mean Axis X: %.2f \nMax Axis Y: %.2f Min Axis Y: "
               "%.2f Mean Axis Y: %.2f \nMax Axis Z: %.2f Min Axis Z: %.2f "
               "Mean Axis Z: %.2f\n",
               max_soil, min_soil, float(mean_soil / cnt), max_light, min_light,
               float(mean_light / cnt), max_temp, min_temp,
               float(mean_temp / cnt), max_hum, min_hum, float(mean_hum / cnt),
               max_accel_X, min_accel_X, (mean_accel_X / cnt), max_accel_Y,
               min_accel_Y, (mean_accel_Y / cnt), max_accel_Z, min_accel_Z,
               (mean_accel_Z / cnt));
}

void printMeasurements() {
    printf("\nSOIL MOISTURE: %.1f \nLIGHT: %.1f \nCOLOR SENSOR: Clear: %d "
               "Red: %d Green: %d Blue: %d \nTEMP/HUM: Temperature: %.2f, "
               "Humidity: %.2f \nGPS: #sats: %d, Lat(UTC): %f %c, Long(UTC): "
               "%f %c, Altitude: %.lf m, GPS_time: %02d:%02d:%02d Dominant "
               "Color: %s\n",
               soil_value, light, rgb_clear, rgb_red, rgb_green, rgb_blue,
               temperature, humidity, sats, latitude, ns, longitude, ew, alt,
               horas + 1, minutos, segundos, colorines);

    string s = "accx: " + to_string(accel[0]);
    printf("%s", s.c_str());
    string sy = "accy: " + to_string(accel[1]);
    printf("%s", sy.c_str());
    string sz = "accz: " + to_string(accel[2]);
    printf("%s", sz.c_str());
}


int main() {
  userbtn.mode(PullUp);
  userbtn.fall(change_mode);
  th.start(th_measures);
  tick.attach(start_measure, 2s);

  th_advanced.start(advancedModeFun);

  isMeasureEnabled = false;
  isMaximumsPrintEnabled = false;

    led1 = 0;
    led2 = 0;
    led3 = 0;
  // initAcc();
  rgb_setup();
  gpsInit();
  initMeasureColors();

  //  setRGBColor(BLACK);
  while (true) {

    switch (mode) {
    case TEST:

      if (enable_printf) {
        enable_printf = false;
        printMeasurements();

        setRGBColor(dom_col);
      }
      led1 = 1;
      led2 = 0;
      led3 = 0;

      break;

    case NORMAL:

      if (isMaximumsPrintEnabled) {
        isMaximumsPrintEnabled = false;
        printMaximums();
        cnt = 0;
      }

      if (enable_printf) {
        enable_printf = false;

        limits(temperature, MAX_TEMP, MIN_TEMP, RED);
        limits(humidity, MAX_HUM, MIN_HUM, BLUE);
        limits(soil_value, MAX_SOIL, MIN_SOIL, GREEN);
        limits(light, MAX_LIGHT, MIN_LIGHT, MAGENTA);
        limits(light, max_accel_X, min_accel_X, MAGENTA);
        limits(light, max_accel_Y, min_accel_Y, MAGENTA);
        limits(light, max_accel_Z, min_accel_Z, MAGENTA);
        printMeasurements();
      }

      // cambio las leds
      led1 = 0;
      led2 = 1;
      led3 = 0;

      break;


    case ADVANCED:
        if (isPrintEnabled){
                isPrintEnabled = false;
                printMeasurements();
                if(isMaximumsPrintEnabled) {
                    isMaximumsPrintEnabled = false;
                    printMaximums();
                    cnt = 0; // so that hourly values are reset
                }
            }
        if (isAdvLimitsEnabled) {
            isAdvLimitsEnabled = false;
            advanced_check_limits(temperature, MAX_TEMP, MIN_TEMP, ptr_measure_temperature);
            advanced_check_limits(humidity, MAX_HUM, MIN_HUM, ptr_measure_humidity);
            advanced_check_limits(soil_value, MAX_SOIL, MIN_SOIL, ptr_measure_soil);
            advanced_check_limits(light, MAX_LIGHT, MIN_LIGHT, ptr_measure_ambient);
            advanced_check_limits(rgb_red, MAX_COLOR, MIN_COLOR, ptr_measure_leaf);
            advanced_check_limits(rgb_green, MAX_COLOR, MIN_COLOR, ptr_measure_leaf);
            advanced_check_limits(rgb_blue, MAX_COLOR, MIN_COLOR, ptr_measure_leaf);
            advanced_check_limits(accel[0], L_MAX_ACCE_X, L_MIN_ACCE_X, ptr_measure_acc);
            advanced_check_limits(accel[1], L_MAX_ACCE_Y, L_MIN_ACCE_Y, ptr_measure_acc);
            advanced_check_limits(accel[2], L_MAX_ACCE_Z, L_MIN_ACCE_Z, ptr_measure_acc);
        }

            led1 = 0;
            led2 = 0;
            led3 = 1;
            break;
    }
  }
}


#include "main.h"
#include "brightnessControl.h"
#include "rgb.h"
#include "tempHumidity.h"
#include "measure.h"
#include "acc.h"
#include "gps.h"
#include <cstdint>
 
bool enable_printf;
 
uint8_t cnt;
 
float mean_temp;
float max_temp;
float min_temp;
 
float mean_hum;
float max_hum;
float min_hum;
 
float mean_soil;
float max_soil;
float min_soil;
 
float mean_light;
float max_light;
float min_light;

float red_sum;
float green_sum;
float blue_sum;
 
 
void th_measures(void) {
 
    mean_temp = 0;
    max_temp = 0;
    min_temp = 0;
 
    mean_hum = 0;
    max_hum = 0;
    min_hum = 0;
 
    mean_soil = 0;
    max_soil = 0;
    min_soil = 0;
 
    mean_light = 0;
    max_light = 0;
    min_light = 0;
    
 
    cnt = 0;
 
    enable_printf = false;
 
 
    while (true) {
        
 
        if (isMeasureEnabled){
           
            isMeasureEnabled = false;
           
            get_brightness();
            getSoilmeasure();
            rgbmeasure();
            getTemperature();
            getRelativeHumidity();
            read_accel();
            gpsMeasure();

            if (mode == NORMAL){
                if (cnt == 0){
 
                    max_temp = temperature;
                    min_temp = temperature;
                    mean_temp = 0;
 
                    max_hum = humidity;
                    min_hum = humidity;
                    mean_hum = 0;
 
                    max_soil = soil_value;
                    min_soil = soil_value;
                    mean_soil = 0;
 
                    max_light = light;
                    min_light = light;
                    mean_light = 0;
 
                    max_accel_X = accel[0];
                    min_accel_X = accel[0];
                    max_accel_Y = accel[1];
                    min_accel_Y = accel[1];
                    max_accel_Z = accel[2];
                    min_accel_Z = accel[2];

                    mean_accel_X = 0;
                    mean_accel_Y = 0;
                    mean_accel_Z = 0;

                    red_sum = 0;
                    green_sum = 0;
                    blue_sum = 0;
 
                }

                               cnt++;

                    mean_temp += temperature;
                    mean_hum += humidity;
                    mean_soil += soil_value;
                    mean_light += light;
                    mean_accel_X += accel[0];
                    mean_accel_Y += accel[1];
                    mean_accel_Z += accel[2];
                    red_sum +=rgb_red;
                    green_sum +=rgb_green;
                    blue_sum +=rgb_blue;

                


               printf("\n\ncont: %d", cnt);


            
                if (temperature > max_temp){
                    max_temp = temperature;
                }
 
                if (temperature < min_temp){
                    min_temp = temperature;
                }
 
                if (humidity > max_hum){
                    max_hum = humidity;
                }
 
                if (humidity < min_hum){
                    min_hum = humidity;
                }
 
                if (soil_value > max_soil){
                    max_soil = soil_value;
                }
 
                if (soil_value < min_soil){
                    min_soil = soil_value;
                }
 
                if (light > max_light){
                    max_light = light;
                }
 
                if (light < min_light){
                    min_light = light;
                }

                if(accel[0] > max_accel_X){
                    max_accel_X = accel[0];
                }

                if(accel[0] < min_accel_X){
                    min_accel_X = accel[0];
                }

                if(accel[1] > max_accel_Y){
                    max_accel_Y = accel[1];
                }

                if(accel[1] < min_accel_Y){
                    min_accel_Y = accel[1];
                }

                if(accel[2] > max_accel_Z){
                    max_accel_Z = accel[2];
                }

                if(accel[2] < min_accel_Z){
                    min_accel_Z = accel[2];
                }
            }

            enable_printf = true;
 
        }  
 
    }
 
}



 

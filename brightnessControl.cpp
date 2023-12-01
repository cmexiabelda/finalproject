#include "brightnessControl.h"
 
/*
extern float getRelativeHumidity();
extern float getTemperature();
extern void rgb_setup();
extern void rgbmeasure();
extern void setDominantColor();*/
 
float soil_value;
 
float light;
AnalogIn lightSensor(PA_4);
void get_brightness() {
    light= lightSensor.read();
}
/*
void setup_all_sensors() {
    rgb_setup();
}*/
 
AnalogIn soil_pin(PA_0);
void getSoilmeasure(){
    soil_value = soil_pin.read()*100;
}
 
/*
void commsLoop() {
    setup_all_sensors();
    while(1){
        brightnessLevel = get_brightness();
        getTemperature();                                                                                                                                                                                                    
        getRelativeHumidity();
        rgbmeasure();
        setDominantColor();
        getSoilmeasure();
    }
 
    ThisThread::sleep_for(2s);
}
*/
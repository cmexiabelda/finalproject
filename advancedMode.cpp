#include "mbed.h"
#include "main.h"
#include "led.h"

#define ALARM_QUEUE_SIZE 6
//extern bool colorChanged;
extern void setRGBColor(uint8_t color);
enum ledColors {
    C_WHITE,
    C_WHITEBLUE,
    C_PURPLE,
    C_BLUE,
    C_YELLOW,
    C_GREEN,
    C_RED,
    C_NOCOLOR
};
measureColor *retrievedColor;

Queue<measureColor, ALARM_QUEUE_SIZE> alarmFIFO;

bool putAlarmInQueue(measureColor* ptrColor) {
    //printf("CALLED putQueue\n");
    // Check if the color has already been added
    if (ptrColor->activated) {
        //printf("Color already added, do not add again\n");
        return false;
    }

    // Try to put in the queue
    if (alarmFIFO.try_put(ptrColor)) {
        // Successfully added to the queue
        //printf("Successfully added to the queue\n");
        ptrColor->activated=true;
        return true;
    } else {
        // Failed to put in the queue
        //printf("Failed to put in the queue\n");
        return false;
    }
}

bool lightAlarmLed() {

    // Try to get an element from the queue
    if (alarmFIFO.try_get(&retrievedColor)) {
        // Successfully retrieved from the queue
        //printf("RETRIEVED COLOR = %d, ACTIVATED = %d\n", retrievedColor->color, retrievedColor->activated);
        setRGBColor(retrievedColor->color);
        // Mark the color as not added in the colorAdded array
        retrievedColor->activated = false;
        //colorAdded[retrievedColor->color] = false;

        return true;
    } else {
        // Queue is empty
        setRGBColor(BLACK);
        return false;
    }
}

void advancedMode_sleep(int activatedAlarmsCount) {
    // We have implemented this because we cannot do:
    // ThisThread::sleep_for(VARIABLE);
    if (activatedAlarmsCount > 5) return;

    setRGBColor(BLACK);
    if (activatedAlarmsCount == 0) ThisThread::sleep_for(3000ms);
    else if (activatedAlarmsCount == 1) ThisThread::sleep_for(2500ms);
    else if (activatedAlarmsCount == 2) ThisThread::sleep_for(2000ms);
    else if (activatedAlarmsCount == 3) ThisThread::sleep_for(1500ms);
    else if (activatedAlarmsCount == 4) ThisThread::sleep_for(1000ms);
    else if (activatedAlarmsCount == 5) ThisThread::sleep_for(500ms);
}

void lightAlarmLedsSequentially() {
    int activatedAlarmsCount = alarmFIFO.count();
    //printf("ADV2. Activated alarms = %d\n", activatedAlarmsCount);
    measureColor *retrievedColor; // guardar el objeto 'measureColor' que cojo del FIFO
    if (activatedAlarmsCount > 0) {
        for (int i=0; i<activatedAlarmsCount;i++) {
            if(alarmFIFO.try_get(&retrievedColor)) {
                setRGBColor(retrievedColor->color);
                retrievedColor->activated=false;
                ThisThread::sleep_for(500ms);
            } else {
                //printf("ERROR WHILE GETTING ALARM FROM QUEUE\n");
            }
        }
        //printf("QUEUE SIZE AFTER LOOP = %d\n", alarmFIFO.count());
    }
    advancedMode_sleep(activatedAlarmsCount);
}

void runAdvancedLoop() {
    int cnt = 0;
    //printf("Entered advanced loop!\n");
    if(mode == ADVANCED) {
        //printf("\nEntered ADVANCED loop");
        cnt = alarmFIFO.count();
        for (int i=0;i<cnt;i++) {
            lightAlarmLed();
            ThisThread::sleep_for(500ms);
        }
        advancedMode_sleep(cnt);
    }
}


// bool newColor = false;


void advancedModeFun() {
    while(true) {
        if(mode == ADVANCED) {
            //printf("ADVANCED TH LOOP\n");
            lightAlarmLedsSequentially();
        }
    }
}
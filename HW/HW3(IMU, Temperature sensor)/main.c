#include "../ledtest.h"
#include "../buttontest.h"
#include "../buzzertest.h"
#include "../fndtest.h"
#include "../textlcdtest.h"
#include "../colorLEDtest.h"
#include "../accelManGyroTest.h"
#include "../temperatureTest.h"
#include <stdio.h>

void sensors_init() {
    ledLibInit();
    buttonInit();
    buzzerInit();
    textLCDInit();
    colorLED_init();
    colorLED_pwmEnable();
}

void sensors_close() {
    buttonExit();
    ledLibExit();
    buzzerExit();
    fndExit();
    textLCDExit();
}
int main() {
    int gyro[3]; int accel[3]; int magnet[3];

    while(1) {
        getGyro(gyro);
        getAccel(accel);
        getMagnet(magnet);

        printf("[Gyro] : %d, %d, %d\n", gyro[0], gyro[1], gyro[2]);
        printf("[Accel] : %d, %d, %d\n", accel[0], accel[1], accel[2]);
        printf("[Magnet] : %d, %d, %d\n", magnet[0], magnet[1], magnet[2]);
        temp_read();
        printf("\n");
        sleep(1);
    }
    return 0;
}
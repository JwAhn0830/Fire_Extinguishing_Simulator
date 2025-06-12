#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "colorLEDtest.h"

/* 
/sys/class/pwm/pwmchip"N"/export 에 0 작성 -> disable
*/
int colorLED_pwmActiveAll() {
    int fd = 0;
    fd = open(COLOR_LED_DEV_R_ PWM_EXPORT, O_WRONLY);
    write(fd, &"0", 1);
    close(fd);

    fd = open (COLOR_LED_DEV_G_ PWM_EXPORT, O_WRONLY);
    write(fd, &"0", 1);
    close(fd); 
    fd = open (COLOR_LED_DEV_B_ PWM_EXPORT, O_WRONLY);
    write(fd, &"0", 1);
    close(fd); 

    return 1;
}

/* 
/sys/class/pwm/pwmchip"N"/unexport 에 0 작성 -> disable
*/
int colorLED_pwmDisable() {
    int fd = 0;
    fd = open(COLOR_LED_DEV_R_ PWM_UNEXPORT, O_WRONLY);
    write(fd, &"0", 1);
    close(fd); 

    fd = open(COLOR_LED_DEV_G_ PWM_UNEXPORT, O_WRONLY);
    write(fd, &"0", 1);
    close(fd); 

    fd = open(COLOR_LED_DEV_B_ PWM_UNEXPORT, O_WRONLY);
    write(fd, &"0", 1);
    close(fd);

    return 1;
}
int colorLED_setPeriod(int period, int index) {
    int fd = 0;
    if (index == 0) {
        fd = open(COLOR_LED_DEV_R_ PWM_PERIOD, O_WRONLY);
    } 
    else if (index == 1) {
        fd = open(COLOR_LED_DEV_G_ PWM_PERIOD, O_WRONLY);
    }
    else if (index == 2) {
        fd = open(COLOR_LED_DEV_B_ PWM_PERIOD, O_WRONLY);
    }

    dprintf(fd, "%d", period);
    close(fd);

    return 1;
}
int colorLED_setDuty(int dutyCycle, int index) {
    int fd = 0;
    if (index == 0) {
        fd = open(COLOR_LED_DEV_R_ PWM_DUTY, O_WRONLY);
    } 
    else if (index == 1) {
        fd = open(COLOR_LED_DEV_G_ PWM_DUTY, O_WRONLY);
    }
    else if (index == 2) {
        fd = open(COLOR_LED_DEV_B_ PWM_DUTY, O_WRONLY);
    }

    dprintf(fd, "%d", dutyCycle);
}
int colorLED_serPercent(int percent, int ledColor) {
    int fd = 0;

    if (percent < 0 || percent > 100) {
        printf("wrong percent : %d, should be 0 ~ 100\n", percent);
        return 0;
    }
    
    int duty = (100-percent) * PWM_PERIOD_NS / 100;

    colorLED_setDuty(duty, ledColor);
}

int colorLED_pwmEnable() {
    int fd = 0;

    fd = open(COLOR_LED_DEV_R_ PWM_ENABLE, O_WRONLY);
    write(fd, &"1", 1);
    close(fd);

    fd = open(COLOR_LED_DEV_G_ PWM_ENABLE, O_WRONLY);
    write(fd, &"1", 1);
    close(fd);

    fd = open(COLOR_LED_DEV_B_ PWM_ENABLE, O_WRONLY);
    write(fd, &"1", 1);
    close(fd);

    return 1;
}

void colorLED_init() {
    colorLED_pwmActiveAll();
    colorLED_setDuty(0,0);
    colorLED_setDuty(0,1);
    colorLED_setDuty(0,2);
    colorLED_setPeriod(PWM_PERIOD_NS, 0);
    colorLED_setPeriod(PWM_PERIOD_NS, 1);
    colorLED_setPeriod(PWM_PERIOD_NS, 2);
}
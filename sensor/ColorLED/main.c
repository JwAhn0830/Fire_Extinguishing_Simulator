#include "colorLED.h"

int main() {
    colorLED_init();
    
    colorLED_serPercent(0, 0);
    colorLED_serPercent(25, 1);
    colorLED_serPercent(0, 2);
    colorLED_pwmEnable();
}
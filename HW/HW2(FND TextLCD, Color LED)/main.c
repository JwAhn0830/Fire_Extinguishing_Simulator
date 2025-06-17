#include "../ledtest.h"
#include "../buttontest.h"
#include "../buzzertest.h"
#include "../fndtest.h"
#include "../textlcdtest.h"
#include "../colorLEDtest.h"
#include <stdio.h>

int main() {
    ledLibInit();
    buttonInit();
    buzzerInit();
    textLCDInit();
    colorLED_init();
    colorLED_pwmEnable();


    textLCDWrite("Hello", 1);
    textLCDWrite("Jaewon", 2);
    printf("[TEXTLCD] :\n\tHello\n\tJaewon\n");

    for (int i = 0; i <= 10; i++) {
        fndInit(i, 0);
        printf("[FND] : %d\r\n", i);
        sleep(1);
    }

    while(1) {
        colorLED_serPercent(25, 0);
        colorLED_serPercent(0, 1);
        colorLED_serPercent(0, 2);
        printf("[COLOR LED] : RED\r\n");
        sleep(1);

        colorLED_serPercent(0, 0);
        colorLED_serPercent(25, 1);
        colorLED_serPercent(0, 2);
        printf("[COLOR LED] : GREEN\r\n");
        sleep(1);
        
        colorLED_serPercent(0, 0);
        colorLED_serPercent(0, 1);
        colorLED_serPercent(25, 2);
        printf("[COLOR LED] : BLUE\r\n");
        sleep(1);
    }

    buttonExit();
    ledLibExit();
    buzzerExit();
    fndExit();
    textLCDExit();
    

    return 0;
}
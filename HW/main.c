#include "ledtest.h"
#include "buttontest.h"
#include "buzzertest.h"
#include "fndtest.h"
#include "textlcdtest.h"
#include "colorLEDtest.h"
#include "temperatureTest.h"
#include "accelManGyroTest.h"
pthread_t tid[1];
int gyro[3];
volatile int run = 1;
void *fnd() {
    int i = 0;
    while(run) {
        fndInit(i++, 0);
        delay_ms(1000);
    }
}

void *readTempandGyro() {
    while(run) {
        temp_read();
        getGyro(gyro);
        sleep(1);
    }
}

int main() {
    run = 1;
    BUTTON_MSG_T Data;
    ledLibInit();
    buttonInit();
    buzzerInit();
    textLCDInit();
    colorLED_init();
    colorLED_pwmEnable();
    int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
    textLCDWrite("Hello", 1);

    pthread_create(&tid[0], NULL, &fnd, NULL);
    pthread_create(&tid[1], NULL, &readTempandGyro, NULL);
    while(run) {
    //struct input_event stEvent;	
        int returnValue = 0;
        returnValue = msgrcv(msgID, &Data, sizeof(Data) - sizeof(long int), 0, 0);
            
        if(Data.type== EV_KEY){
            
            if ( Data.pressed ) {

                switch(Data.keyInput)
                {
                    case KEY_HOME: 
                        textLCDWrite("DO", 1);
                        ledOnOff(0, 1);
                        buzzerTone(DO);
                        colorLED_serPercent(0, 0);
                        colorLED_serPercent(25, 1);
                        colorLED_serPercent(0, 2);
                        break;
                    case KEY_BACK: 
                        textLCDWrite("RE", 1);
                        ledOnOff(1, 1);
                        buzzerTone(RE);
                        colorLED_serPercent(0, 0);
                        colorLED_serPercent(0, 1);
                        colorLED_serPercent(25, 2);
                        break;
                    case KEY_SEARCH: 
                        textLCDWrite("MI", 1);
                        ledOnOff(2, 1);
                        buzzerTone(MI);
                        colorLED_serPercent(25, 0);
                        colorLED_serPercent(0, 1);
                        colorLED_serPercent(0, 2);
                        break;
                    case KEY_MENU: 
                        textLCDWrite("FA", 1);
                        ledOnOff(3, 1);
                        buzzerTone(FA);
                        break;
                    case KEY_VOLUMEUP: 
                        textLCDWrite("SO", 1);
                        ledOnOff(4, 1);
                        buzzerTone(SOL);
                        break;
                    case KEY_VOLUMEDOWN: 
                        
                        textLCDWrite("RA", 1);
                        ledOnOff(5, 2);
                        //buzzerTone(RA);
                        run = 0;
                        break;
                }
                printf("pressed\n");
            }
            else {
                ledOnOff(0, 0); ledOnOff(1, 0); ledOnOff(2, 0); ledOnOff(3, 0); ledOnOff(4, 0); ledOnOff(5, 0);
                buzzerStop();
                //textLCDClear();
            }
        } 
    } 
        textLCDWrite("BYEBYE",1);
        sleep(3);
        buttonExit();
        ledLibExit();
        buzzerExit();
        fndExit();
        textLCDExit();
        pthread_join(tid[0], NULL); pthread_join(tid[1], NULL); 

        return 0;
}
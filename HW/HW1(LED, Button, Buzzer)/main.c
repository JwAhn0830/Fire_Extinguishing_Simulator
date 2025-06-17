#include "../ledtest.h"
#include "../buttontest.h"
#include "../buzzertest.h"
#include "../fndtest.h"
#include "../TextLCDtest"
#include "../colorLEDtest.h"
#include "stdio.h"

int main() {
    BUTTON_MSG_T Data;
    ledLibInit();
    buttonInit();
    buzzerInit();

    int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);

    while(1) {
    //struct input_event stEvent;	
        int returnValue = 0;
        returnValue = msgrcv(msgID, &Data, sizeof(Data) - sizeof(long int), 0, 0);
            
        if(Data.type== EV_KEY){
            
            if ( Data.pressed ) {
                switch(Data.keyInput) {
                    case KEY_HOME: 
                        printf("HOME KEY pressed -> [LED 0] : ON | [BUZZER] : DO \r\n");
                        ledOnOff(0, 1);
                        buzzerTone(DO);
                        break;
                    case KEY_BACK: 
                        printf("BACK KEY pressed -> [LED 1] : ON | [BUZZER] : DO \r\n");
                        ledOnOff(1, 1);
                        buzzerTone(RE);
                        break;
                    case KEY_SEARCH: 
                        printf("SEARCH KEY pressed -> [LED 2] : ON | [BUZZER] : DO \r\n");
                        ledOnOff(2, 1);
                        buzzerTone(MI);
                        break;
                    case KEY_MENU: 
                        printf("MEMU KEY pressed -> [LED 3] : ON | [BUZZER] : DO \r\n");
                        ledOnOff(3, 1);
                        buzzerTone(FA);
                        break;
                    case KEY_VOLUMEUP: 
                        printf("VOLUMEUP KEY pressed -> [LED 4] : ON | [BUZZER] : DO \r\n");
                        ledOnOff(4, 1);
                        buzzerTone(SOL);
                        break;
                    case KEY_VOLUMEDOWN: 
                        printf("VOLUMEDOWN KEY pressed [LED 5] : ON | [BUZZER] : DO \r\n");
                        ledOnOff(5, 2);
                        buzzerTone(RA);
                        break;
                }
            }
            else {
                printf("Button Relased -> [LED ALL] : OFF | [BUZZER] : OFF\r\n");
                ledOnOff(0, 0); ledOnOff(1, 0); ledOnOff(2, 0); ledOnOff(3, 0); ledOnOff(4, 0); ledOnOff(5, 0);
                buzzerStop();
                //textLCDClear();
            }
        } 
    } 


        buttonExit();
        ledLibExit();
        buzzerExit();

        return 0;
}
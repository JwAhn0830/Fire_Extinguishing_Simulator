#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

void buzzerEnable(int bEnable) {
    char path[200];
    sprintf(path, "%s%s", gBuzzerBaseSysDir, BUZZER_ENABLE_NAME);
    int fd = open(path, O_WRONLY);
    if (bEnable == 1)
        write(fd, &"1", 1);
    else
        write(fd, &"0", 1);
}

void setFrequency(int frequency) {
    char path[200];
    sprintf(path, "%s%s", gBuzzerBaseSysDir, BUZZER_FREQUENCY_NAME);
    int fd = open(path, O_WRONLY);
    dprintf(fd, "%d", frequency);
    close(fd);
}

int buzzerInit() {
    //Fine Buzzer System Path
    DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
    int ifNotFound = 1;
    if (dir_info != NULL) {
        while(1) {
            struct dirent *dir_entry;
            dir_entry = readdir(dir_info);
            if (dir_entry == NULL) break;
            if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0) {
                ifNotFound = 0;
                sprintf(gBuzzerBaseSysDir, "%s%s/", BUZZER_BASE_SYS_PATH, dir_entry->d_name );
            }
        }
    }

    printf("find %s\n", gBuzzerBaseSysDir);


    return ifNotFound;
}

void buzzerExit() {
    buzzerEnable(0);
    char path[200];
    sprintf(path, "%s%s", gBuzzerBaseSysDir, BUZZER_ENABLE_NAME);
    close(path);
}
void buzzerTone(int scale) {
    setFrequency(scale);
    buzzerEnable(1);
}
void buzzerStop() {
    buzzerEnable(0);
}

void delay_ms(unsigned int milliseconds) {
    usleep(milliseconds * 1000);  
}

void bgm(int tempo) {
    int melody[] = {
        NOTE_DS5, 16, NOTE_GS4, 16, NOTE_B4, 16, NOTE_GS4, 16, NOTE_CS5, 16, NOTE_GS4, 16, NOTE_DS5, 16, NOTE_GS4, 16, //81
        NOTE_CS5, 16, NOTE_FS4, 16, NOTE_AS4, 16, NOTE_FS4, 16, NOTE_FS5, 16, NOTE_FS4, 16, NOTE_DS5, 16, NOTE_E5, 16,
        NOTE_D5, 4, REST, 4,
        NOTE_CS5, 8, REST, 8, NOTE_AS4, -16,  NOTE_B4, -16, NOTE_CS5, 16,
        NOTE_DS5, 16, NOTE_GS4, 16, NOTE_B4, 16, NOTE_GS4, 16, NOTE_CS5, 16, NOTE_GS4, 16, NOTE_DS5, 16, NOTE_GS4, 16,
        
        NOTE_CS5, 16, NOTE_FS4, 16, NOTE_AS4, 16, NOTE_FS4, 16, NOTE_FS5, 16, NOTE_FS4, 16, NOTE_DS5, 16, NOTE_E5, 16,
        NOTE_DS5, 4, REST, 8, NOTE_DS5, 16,  NOTE_E5, 16,
        NOTE_FS5, 16, NOTE_CS5, 16, NOTE_E5, 16, NOTE_CS4, 16, NOTE_DS5, 16, NOTE_E5, 16, NOTE_G5, 16, NOTE_AS5, 16,
        NOTE_GS5, 16, NOTE_DS5, 16, NOTE_DS6, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,

        NOTE_CS6, 8, NOTE_FS6, 16, NOTE_DS6, 8, REST,16, REST,8, //117
        REST,4, NOTE_CS5, 8, NOTE_DS5, 8,
        NOTE_E5, -8, NOTE_F5, -8,
        NOTE_DS5, 8, NOTE_G5, 8, NOTE_GS5, 8, NOTE_AS5, 8,
        NOTE_AS5, -8, NOTE_GS5, -8,
      
        NOTE_AS5, -8, NOTE_GS5, -8,//122
        NOTE_AS5, -8, NOTE_GS5, -8,
        NOTE_B6, 8, NOTE_AS5, 8, NOTE_GS5, 8, NOTE_FS5, 8,
        NOTE_AS5,-8, NOTE_GS6, -8, NOTE_DS5, 8,
        NOTE_AS5,-8, NOTE_GS6, -8, NOTE_DS5, 8,
        NOTE_AS5,-8, NOTE_GS6, -8, NOTE_DS5, 8,
      
        NOTE_B5,8, NOTE_CS6, 8, NOTE_AS5, 8, NOTE_B5, 8,//128
        NOTE_GS5,8, REST,8, REST, 16
    };
    int wholenote = (60000 * 4) / tempo;
    int divider = 0, noteDuration = 0;
    int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  
    buzzerInit();
    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
  
      // calculates the duration of each note
      divider = melody[thisNote + 1];
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
      } 
      else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }
  
      buzzerTone(melody[thisNote]);
      delay_ms(noteDuration * 0.9);
      buzzerStop();
      delay_ms(noteDuration);
    }
    buzzerExit();
  }
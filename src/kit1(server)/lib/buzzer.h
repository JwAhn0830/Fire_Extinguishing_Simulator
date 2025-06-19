#ifndef _BUZZER_H_
#define _BUZZER_H_

#define DO 262
#define RE 294
#define MI 330
#define FA 349
#define SOL 392
#define RA 440
#define SI 494
#define HIGH_DO 524

#define NOTE_B0  7
#define NOTE_C1  8
#define NOTE_CS1 8
#define NOTE_D1  9
#define NOTE_DS1 9
#define NOTE_E1  10
#define NOTE_F1  11
#define NOTE_FS1 11
#define NOTE_G1  12
#define NOTE_GS1 13
#define NOTE_A1  14
#define NOTE_AS1 14
#define NOTE_B1  15
#define NOTE_C2  16
#define NOTE_CS2 17
#define NOTE_D2  18
#define NOTE_DS2 19
#define NOTE_E2  20
#define NOTE_F2  21
#define NOTE_FS2 23
#define NOTE_G2  24
#define NOTE_GS2 26
#define NOTE_A2  27
#define NOTE_AS2 29
#define NOTE_B2  30
#define NOTE_C3  33
#define NOTE_CS3 34
#define NOTE_D3  36
#define NOTE_DS3 39
#define NOTE_E3  41
#define NOTE_F3  43
#define NOTE_FS3 46
#define NOTE_G3  49
#define NOTE_GS3 52
#define NOTE_A3  55
#define NOTE_AS3 58
#define NOTE_B3  61
#define NOTE_C4  65
#define NOTE_CS4 69
#define NOTE_D4  73
#define NOTE_DS4 78
#define NOTE_E4  82
#define NOTE_F4  87
#define NOTE_FS4 92
#define NOTE_G4  98
#define NOTE_GS4 104
#define NOTE_A4  110
#define NOTE_AS4 116
#define NOTE_B4  123
#define NOTE_C5  131
#define NOTE_CS5 138
#define NOTE_D5  147
#define NOTE_DS5 155
#define NOTE_E5  165
#define NOTE_F5  174
#define NOTE_FS5 185
#define NOTE_G5  196
#define NOTE_GS5 208
#define NOTE_A5  220
#define NOTE_AS5 233
#define NOTE_B5  247
#define NOTE_C6  261
#define NOTE_CS6 277
#define NOTE_D6  293
#define NOTE_DS6 311
#define NOTE_E6  330
#define NOTE_F6  349
#define NOTE_FS6 370
#define NOTE_G6  392
#define NOTE_GS6 415
#define NOTE_A6  440
#define NOTE_AS6 466
#define NOTE_B6  494
#define NOTE_C7  523
#define NOTE_CS7 554
#define NOTE_D7  587
#define NOTE_DS7 622
#define NOTE_E7  659
#define NOTE_F7  698
#define NOTE_FS7 740
#define NOTE_G7  784
#define NOTE_GS7 831
#define NOTE_A7  880
#define NOTE_AS7 932
#define NOTE_B7  988
#define NOTE_C8  1046
#define NOTE_CS8 1108
#define NOTE_D8  1174
#define NOTE_DS8 1244
#define REST     0

#define MAX_SCALE_SETP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128]; //sys/bus/platform/devices/peribuzzer.XX가 결정됨 -> 랜덤으로
// const int musicScale[MAX_SCALE_SETP] = {262, 294, 330, 349, 392, 440, 494}; // do re mi ... si do
void buzzerEnable();
void setFrequency(int frequency);
int buzzerInit();
void buzzerExit();
void buzzerTone();
void buzzerStop();
void delay_ms(unsigned int milliseconds);
void bgm(int tempo);
#endif 
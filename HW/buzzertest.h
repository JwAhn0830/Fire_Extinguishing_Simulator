#ifndef _BUZZERTEST_H_
#define _BUZZERTEST_H_

#define DO 262
#define RE 294
#define MI 330
#define FA 349
#define SOL 392
#define RA 440
#define SI 494
#define HIGH_DO 524
#define MAX_SCALE_SETP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128]; //sys/bus/platform/devices/peribuzzer.XX가 결정됨 -> 랜덤으로
// const int musicScale[MAX_SCALE_SETP] = {262, 294, 330, 349, 392, 440, 494}; // do re mi ... si do
void buzzerEnable();
void setFrequency(int frequency);
void buzzerInit();
void buzzerExit();
void buzzerTone();
void buzzerStop();
void delay_ms(unsigned int milliseconds);
#endif 
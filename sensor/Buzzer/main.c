#include "buzzer.h"
// gomsemali 
int melody[] = {
    DO, DO, DO, DO, DO,
    MI, SOL, SOL, MI, DO,
    SOL, SOL, MI, SOL, SOL, MI,
    DO, DO, DO
};

int noteDurations[] = {
    4, 8, 8, 4, 4,
    4, 8, 8, 4, 4,
    8, 8, 4, 8, 8, 4,
    4, 4, 2
};
int main() {

    buzzerInit();

    for (int i = 0; i < 19; i++) {
        int durations = 1000 / noteDurations[i];
        buzzerTone(melody[i]);
        delay_ms(durations * 1.3);
        buzzerStop();
    }
    buzzerExit();
}
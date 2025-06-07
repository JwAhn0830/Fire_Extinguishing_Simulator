#include "led.h"
#include "button.h"
#include "buzzer.h"
#include "textlcd.h"
#include "gyro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define SERVER_ADDR "192.168.50.246"  // <- Kit2's server addr
int sock;
struct sockaddr_in server_addr;
// message to send
const char *messages[] = {"Game Start\n"};

pthread_t tid[2];
BUTTON_MSG_T Data;

int msgID;
void connectServer() {
    int num_messages = sizeof(messages) / sizeof(messages[0]);

    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket failed");
        return 1;
    }

    // set server addr 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock);
        return 1;
    }

    textLCDWrite("Server Conntected", 2);
    printf("Connected to Server! \n");
}

void *test() {
    int prev_pressed_local = 0;
    int prev_key_local = -1;
    char *testData[32] = {"1"};

    //snprintf(testData, sizeof(testData), "%.1f,%.1f,%.1f", gyro_f[0], gyro_f[1], gyro_f[2]);
    while (1) {
        int returnValue = msgrcv(msgID, &Data, sizeof(Data) - sizeof(long int), 0, 0);

        if (Data.type == EV_KEY) {
            printf("EV_KEY(");
            if (Data.keyInput == KEY_SEARCH) {
                if (Data.pressed && !(prev_pressed_local && prev_key_local == KEY_SEARCH)) {
                    // 처음 눌렸을 때만 전송
                    send(sock, testData, strlen(testData), 0);
                    printf("send\n");
                    textLCDWrite("send", 1);
                }
            }
            // 이전 상태 저장
            prev_pressed_local = Data.pressed;
            prev_key_local = Data.keyInput;
        }
    }
}
int main() {
    int prev_pressed = 0;
    int prev_key = -1;
    msgID = msgget(MESSAGE_ID, IPC_CREAT | 0666);;

    ledLibInit();
    buttonInit();
    textLCDInit();
    textLCDClear();
 

    connectServer();
    // game start
    while (1) {
        int returnValue = msgrcv(msgID, &Data, sizeof(Data) - sizeof(long int), 0, 0);

        if (Data.type == EV_KEY) {
            printf("EV_KEY(");
            if (Data.keyInput == KEY_HOME) {
                if (Data.pressed && !(prev_pressed && prev_key == KEY_HOME)) {
                    // 처음 눌렸을 때만 전송
                    send(sock, messages[0], strlen(messages[0]), 0);
                    textLCDWrite("Game Start", 1);
                    break;
                }
            }
            // 이전 상태 저장
            prev_pressed = Data.pressed;
            prev_key = Data.keyInput;
        }
    }
    textLCDClear();

    // create thread
    int err;
    
    err = pthread_create(&tid[0], NULL, &test, NULL);
    
    GyroInit();
    //playing

    while(1);
    pthread_join(&tid[0], NULL);
    pthread_join(GyroTh, NULL);
    //Gyroexit();
    buttonExit();




}
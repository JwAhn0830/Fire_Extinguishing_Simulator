#ifndef GYRO_H
#define GYRO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "socket.h"
pthread_t GyroTh;
pthread_t Buzz;
static pthread_mutex_t lock;
static pthread_mutex_t lock_pos;
extern volatile int move;



#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"
// 함수 선언 (필요하면)
void GyroInit(void);
void *Gyroread(void *arg);
void Gyroexit(void);

#endif
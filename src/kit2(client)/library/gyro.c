#include "gyro.h"
#define MUTEX_ENABLE
// 전역 변수 정의
static int fd = 0;
FILE *fp = NULL;
        static int  pos_x = 0;  
        static int  pos_y = 0; 


static int gyro[3];
int volatile move = 0;
//static float angle_x = 0.0f;
//static float angle_y = 0.0f;
static const float dt = 0.1f;

void GyroInit(void)
{
    fd = open(GYROPATH "enable", O_WRONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    dprintf(fd, "1");

    fp = fopen(GYROPATH "data", "rt");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    pthread_create(&GyroTh, NULL, Gyroread, NULL);
}

void *Gyroread(void *arg)
{


    while (1)
    {
        fclose(fp);
        fp = fopen(GYROPATH "data", "rt");
        if (fp == NULL) 
        {
            perror("fopen");
            break;
        }
        float gyro_f[3];
        int ret = fscanf(fp, "%f, %f, %f", &gyro_f[0], &gyro_f[1], &gyro_f[2]);
        if (ret != 3) 
        {
            printf("Error reading gyro data!\n");
            continue;
        }
        
        float gyro_x_scaled = gyro_f[0] / 100.0f;
        float gyro_y_scaled = gyro_f[1] / 100.0f;
        float gyro_z_scaled = gyro_f[2] / 100.0f;

        if (gyro_x_scaled > 10.0f && gyro_z_scaled > 10.0f) 
        {
            pos_x -= 1;  
            pos_y += 1;  
            //printf("L&U MOVE (X: %d, Y: %d)\n", pos_x, pos_y);
            gyro_x_scaled = 0.0f;
            gyro_z_scaled = 0.0f;
            printf("change to : %d\r\n", socketData.cursor);
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move=1;
            
            #ifdef MUTEX_ENABLE
            pthread_mutex_unlock(&lock);
            #endif
        }
        
        else if (gyro_x_scaled > 10.0f && gyro_z_scaled < -10.0f) 
        {
            pos_x -= 1;
            pos_y -= 1;
            //printf("L&D MOVE (X: %d, Y: %d)\n", pos_x, pos_y);
            gyro_x_scaled = 0.0f;
            gyro_z_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 1;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        else if (gyro_x_scaled < -10.0f && gyro_z_scaled > 10.0f) 
        {
            pos_x += 1;
            pos_y += 1;
            //printf("R&U MOVE (X: %d, Y: %d)\n", pos_x, pos_y);
            gyro_x_scaled = 0.0f;
            gyro_z_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 1;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        else if (gyro_x_scaled < -10.0f && gyro_z_scaled < -10.0f) 
        {
            pos_x += 1;
            pos_y -= 1;
            //printf("R&D MOVE (X: %d, Y: %d)\n", pos_x, pos_y);
            gyro_x_scaled = 0.0f;
            gyro_z_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 1;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        else if (gyro_x_scaled > 20.0f) 
        {
            pos_x -= 1;
            //printf("L MV (X: %d)\n", pos_x);
            gyro_x_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            printf("%d\n",move);
            move = 1;
            printf("%d\n",move);
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        else if (gyro_x_scaled < -20.0f) 
        {
            pos_x += 1;
            //printf("R MV (X: %d)\n", pos_x);
            gyro_x_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 1;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        else if (gyro_z_scaled > 20.0f) 
        {
            pos_y += 1;
            //printf("U MV (Y: %d)\n", pos_y);
            gyro_z_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 1;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        else if (gyro_z_scaled < -20.0f) 
        {
            pos_y -= 1;
            //printf("D MV (Y: %d)\n", pos_y);
            gyro_z_scaled = 0.0f;
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 1;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif
        }
        if (pos_x > 1) pos_x = 1;
        if (pos_x < -1) pos_x = -1;
        if (pos_y > 1) pos_y = 1;
        if (pos_y < -1) pos_y = -1;

        #ifdef MUTEX_ENABLE

        #endif
            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock_pos);
            #endif
            socketData.cursor = (1 - pos_y) * 3 + (pos_x + 1);
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock_pos);
            #endif

            #ifdef MUTEX_ENABLE
            pthread_mutex_lock(&lock);
            #endif
            move = 0;
            #ifdef MUTEX_ENABLE
                pthread_mutex_unlock(&lock);
            #endif


        // printf("Current Pos: (%d)\n", socketData.cursor);

        //printf("Gyro scaled: %.2f, %.2f, %.2f\n",gyro_x_scaled, gyro_y_scaled, gyro_z_scaled);

        usleep(200000);
    }

    return NULL;
}




void Gyroexit(void)
{
    close(fd);
    fclose(fp);
}


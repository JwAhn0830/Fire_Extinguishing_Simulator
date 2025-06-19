#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <jpeglib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "fnd.h"
#include "server.h"
#include "led.h"
#include "colorLED.h"
#include "fnd.h"
#include "buzzer.h"

//Foreground
#define FRAME_PATH_FMT "fire_img/output_%04d.jpg"
#define FRAME_NUM      210
#define TARGET_FPS     30

int target_fps = 30;
int frame_num = 705;
const char *frame_dir = "jpg/output_%04d.jpg";

#define PORT 8888

#define TEXT1 "progression rate"
#define TEXT2 "go@          end"
#define TEXT3 "go@@         end"
#define TEXT4 "go@@@        end"
#define TEXT5 "go@@@@       end"
#define TEXT6 "go@@@@@      end"
#define TEXT7 "go@@@@@@     end"
#define TEXT8 "go@@@@@@@    end"
#define TEXT9 "go@@@@@@@@   end"
#define TEXT10 "go@@@@@@@@@  end"
#define TEXT11 "go@@@@@@@@@@ end"
#define TEXT12 "go@@@@@@@@@@@end"
#define LINE 1  // 1 or 1
#define GAMEOVER "GAMEOVER"
#define ALLDEAD "ALL DEAD"

SOCKET_DATA socketData;
int data; 

#define NEW_FIRE_TIMER 7
#define FIRE_NUM 1
#define SCORE_DOWN_STEP 2

volatile  unsigned int fire[9];
volatile unsigned int cursor = 0; // 0~8
volatile static unsigned int gameReady;
volatile  unsigned int water;

volatile unsigned int life = 8;
volatile unsigned int dead = 0;
int fire_num = 0;
int off = 0;
int off_2 = 0;
volatile static unsigned int score = 0;
volatile static unsigned int timer[9] = {0,0,0,0,0,0,0,0,0}; // timer 초기화

pthread_t tid[10];
pthread_mutex_t lock;
int grid_start_coords[9][2] = {
    { 80,   0},  {440,   0},  {800,   0},
    { 80, 200},  {440, 200},  {800, 200},
    { 80, 400},  {440, 400},  {800, 400}
};
const int cursor_center[9][2] = {
    {180, 100},  {540, 100},  {900, 100},
    {180, 300},  {540, 300},  {900, 300},
    {180, 500},  {540, 500},  {900, 500}
};

int cursor_moved = 0;

void *fire_clear() {


        int fb_fd = open("/dev/fb1", O_RDWR);
        if (fb_fd < 0) { perror("open fb"); return; }
        printf("fire_cursor_init !!!!!!\n");
        // 1. Framebuffer 정보 얻기

        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
        ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

        int fb_width  = vinfo.xres;
        int fb_height = vinfo.yres;
        int fb_bpp    = vinfo.bits_per_pixel; 
        int fb_size   = finfo.smem_len;

        unsigned char* fb_ptr = mmap(0, fb_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
    pthread_mutex_unlock(&lock);

    while( water |cursor_moved | off | off_2 ) {
                cursor_moved = 0;
                off = 0;    
                off_2 = 0;    
                water = 0;
        // 2. mmap으로 Framebuffer 포인터 얻기
    pthread_mutex_unlock(&lock);  //mutex unlock
        if (fb_ptr == MAP_FAILED) {
            perror("mmap");
            close(fb_fd);
            return;
        }

        // 3. 프레임 루프
        for (int i = 1; i <= 10; ++i) {

            char fname[256];
            snprintf(fname, sizeof(fname), FRAME_PATH_FMT, i);  //#define FRAME_PATH_FMT "fire_img/output_%04d.jpg"

            // libjpeg: JPEG 파일 로드
            FILE* fp = fopen(fname, "rb");
            if (!fp) { perror(fname); break; }

            struct jpeg_decompress_struct cinfo;
            struct jpeg_error_mgr jerr;
            
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, fp);
            jpeg_read_header(&cinfo, TRUE);
            jpeg_start_decompress(&cinfo);

            int img_width = cinfo.output_width;
            int img_height = cinfo.output_height;
            int row_stride = img_width * cinfo.output_components;

            unsigned char* buffer = malloc(row_stride * img_height);
            // 한줄씩 읽기
            while (cinfo.output_scanline < img_height) {
                unsigned char* rowptr = buffer + cinfo.output_scanline * row_stride;
                jpeg_read_scanlines(&cinfo, &rowptr, 1);
            }

            jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            fclose(fp);
       
                for (int y = 0; y < 600; y++) {
                    for (int x = 0; x < 1024; x++) {
                        int fb_offset = (y * fb_width + x) * (fb_bpp / 8);
                        fb_ptr[fb_offset + 0] = 0x00; // Blue
                        fb_ptr[fb_offset + 1] = 0x00; // Green
                        fb_ptr[fb_offset + 2] = 0x00; // Red
                        fb_ptr[fb_offset + 3] = 0x00; // Alpha (불투명)
                    }
                }
            
            // printf("height :%d \n", fb_height);
            // printf("width :%d \n", fb_width);
            
            free(buffer);
    }

    }

    munmap(fb_ptr, fb_size);
    close(fb_fd);

}
// 화면에 이미지를 표시하는 함수
void *display_frames() {

    printf("display_frame\n");

    // 1. Framebuffer 정보 얻기
    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) { perror("open fb"); return; }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

    int fb_width  = vinfo.xres;
    int fb_height = vinfo.yres;
    int fb_bpp    = vinfo.bits_per_pixel; // 32를 가정
    int fb_size   = finfo.smem_len;

    // 2. mmap으로 Framebuffer 포인터 얻기
    unsigned char* fb_ptr = mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb_ptr == MAP_FAILED) {
        perror("mmap");
        close(fb_fd);
        return;
    }

    // 3. 프레임 루프
    for (int i = 1; i <= frame_num; ++i) {
        char fname[256];
        snprintf(fname, sizeof(fname), "jpg/output_%04d.jpg", i);  //FRAME_PATH_FMT "/home/ecube/jpg/fire_img/output_%04d.jpg"

        // libjpeg: JPEG 파일 로드
        FILE* fp = fopen(fname, "rb");
        if (!fp) {
            perror(fname);
            break;
        }

        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, fp);
        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        int img_width = cinfo.output_width;
        int img_height = cinfo.output_height;
        int row_stride = img_width * cinfo.output_components;

        unsigned char* buffer = malloc(row_stride * img_height);

        // 한줄씩 읽기
        while (cinfo.output_scanline < img_height) {
            unsigned char* rowptr = buffer + cinfo.output_scanline * row_stride;
            jpeg_read_scanlines(&cinfo, &rowptr, 1);
        }
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);

        // 4. Framebuffer에 복사 (좌측상단 기준, 크기 동일 가정)
        for (int y = 0; y < fb_height && y < img_height; ++y) {
            for (int x = 0; x < fb_width && x < img_width; ++x) {
                int fb_offset = (y * fb_width + x) * (fb_bpp / 8);
                unsigned char r = buffer[(y * img_width + x) * 3 + 0];
                unsigned char g = buffer[(y * img_width + x) * 3 + 1];
                unsigned char b = buffer[(y * img_width + x) * 3 + 2];
                // ARGB8888: A=0xFF(불투명), R, G, B
                fb_ptr[fb_offset + 0] = b;
                fb_ptr[fb_offset + 1] = g;
                fb_ptr[fb_offset + 2] = r;
                fb_ptr[fb_offset + 3] = 0xFF;
            }
        }

        free(buffer);

        // 5. 30fps 딜레이
        usleep(1000000 / target_fps); // 약 33,333us
    }

    munmap(fb_ptr, fb_size);
    close(fb_fd);
}

void *play_time() {
    printf("playime_init\n");
        while(1){
            if(dead) {
            textLCDClear(); // 재워니 추가 
            textLCDWrite(GAMEOVER, 1);
            sleep(1);
            textLCDWrite(ALLDEAD, 2);
            sleep(10);   
            break;
            }
            textLCDWrite(TEXT1, 1);
            sleep(1);
            textLCDWrite(TEXT2, 2);
            sleep(3);
            textLCDWrite(TEXT4, 2);
            sleep(3);
            textLCDWrite(TEXT6, 2);
            sleep(3);
            textLCDWrite(TEXT9, 2);
            sleep(3);
            textLCDWrite(TEXT10, 2);
            sleep(5);
             textLCDWrite(TEXT12, 2);
            sleep(3);
        }
}

void* fire_array(void *arg) { //fire array thread
    printf("fire_init\n");
    int random_index;
    
    pthread_mutex_lock(&lock);
    
    for(int i = 0; i<FIRE_NUM; i++){ //초반 fire 4개 random 초기화
        random_index = rand() % 9;
        while(fire[random_index]){
            random_index = rand() % 9;
        }
        fire[random_index] = 1;
        timer[random_index] = -1;
    }
       


    while(!dead){
        if(water) { //물을 발사 했는데
            water = 0;
pthread_mutex_lock(&lock); //mutex lock

   printf("[DEBUG] cursor=%d, fire=%d, timer=%d\n", cursor, fire[cursor], timer[cursor]); // for debugging

            pthread_mutex_lock(&lock); //mutex lock
            if(fire[cursor]){ //해당 위치에 불이 있으면
                fire[cursor] = 0; // 불 배열을 0으
              int delta = 2*NEW_FIRE_TIMER - 2*timer[cursor];
              
              
              
              printf("[DEBUG] score += %d\n", delta);
              //


                score += delta;// 점수를 1 올림
               fire_num = fire_num + 1;
       }
            pthread_mutex_unlock(&lock);  //mutex unlock
        }
    }
    return 0;
}

void* fire_timer(void *arg) {
    int new_fire_index;
    printf("timer_init\n");
    
    while(!dead){
        usleep(1000 * 1000); //1초 delay
        
         pthread_mutex_lock(&lock); //mutex lock
        
        for(int i = 0; i<9; i++){
            off_2 = 1;
            if(fire[i])//불이 있는 위치의 index면
                timer[i] = timer[i] + 1; //해당 위치 fire의 timer +1
            else //불이 없는 위치면 (불이 중간에 꺼졌으면)
                timer[i] = 0; //timer 초기화
                
            if(timer[i] == NEW_FIRE_TIMER){ // 만약 증가 시킨 값이 5면 (time out)
                
                fire[i] = 0; // 불 배열을 0으로 만들고
                score = score - SCORE_DOWN_STEP; // 점수를 깎고
                life = life - 1;
              fire_num = fire_num + 1;
              if(life == 0){
                    dead = 1;
                    printf("\n\n**** YOU DEAD *****(dead flag : %d)\n", dead);
                }
      //timer[i] = 0; //timer 초기화
            }
        }
        
        if(fire_num > 0) {// 없어진 불이 있으면면
            for (int i = 0; i < fire_num; i++) { 
                new_fire_index = rand()%9; //0~8 난수 생성 (초기값 생성)
                while(fire[new_fire_index] | new_fire_index == cursor) // 생성된 난수의 값에 이미 불이 있으면
                    new_fire_index = rand()%9; //새로운 난수 생성
                
                fire[new_fire_index] = 1; //새로운  불을 배열에 넣고
                timer[new_fire_index] = 0; //해당 불 timer 초기화
            }
            fire_num = 0;
            off = 1;
        }
        pthread_mutex_unlock(&lock);  //mutex unlock

        printf("*FIRE*  : %d %d %d %d %d %d %d %d %d", fire[0],fire[1],fire[2],fire[3],fire[4],fire[5],fire[6],fire[7],fire[8]);
        printf("        *TIMER* : %d %d %d %d %d %d %d %d %d", timer[0],timer[1],timer[2],timer[3],timer[4],timer[5],timer[6],timer[7],timer[8]);
        printf("        *SCORE : %d         *fire_num : %d \n", score, fire_num);
        
         
        
    }
}

void* fnd_display(void *arg) {
    printf("fnd\n");
    while (1) {
        pthread_mutex_lock(&lock);       // score 보호
        int temp_score = score;          // 복사해서 표시
        pthread_mutex_unlock(&lock);
        fndInit(temp_score, 0);  // dotflag=0
        
        usleep(300 * 1000);  // 300ms 간격으로 갱신
    }
    return NULL;
}

void* led_life(void *arg) {
    printf("led_life\n");
    while (1) {
        pthread_mutex_lock(&lock);
        int temp_life = life;
        pthread_mutex_unlock(&lock);
        
        for (int i = 0; i < 8; i++) {
                    if (i < temp_life) ledOnOff(i, 1);  // ON
                    else ledOnOff(i, 0);  // OFF
            }

            usleep(300 * 1000);  // 0.1초 주기
    }
    return NULL;
}

void *color_led(void *arg) {
    printf("colol_init\n");
    
    while(!dead)
    {
        colorLED_init();
        colorLED_pwmEnable(); //25.06.19 추가 (이 함수 추가해야 COLORLED 작동됨)
        /* ── 빨간색 ON ─────────────────── */
        colorLED_serPercent(100, 0);   // R 100 %
        colorLED_serPercent(0,   1);   // G  0 %
        colorLED_serPercent(0,   2);   // B  0 %
        colorLED_pwmEnable();          // 전체 PWM 갱신
        delay_ms(500);

                /* ── 파란색 ON ─────────────────── */
        colorLED_serPercent(0,   0);   // R  0 %
        colorLED_serPercent(100,   1);   // G  0 %
        colorLED_serPercent(0, 2);   // B 100 %
        colorLED_pwmEnable();
        delay_ms(200);
        /* ── 파란색 ON ─────────────────── */
        colorLED_serPercent(0,   0);   // R  0 %
        colorLED_serPercent(0,   1);   // G  0 %
        colorLED_serPercent(100, 2);   // B 100 %
        colorLED_pwmEnable();
        delay_ms(500);
    }
}

void *bgm_thread() {
   bgm(100); // param : tempo
   sleep(5);
}

void *Fire_and_Cursor(void *arg) {

    int fb_fd = open("/dev/fb1", O_RDWR);
    if (fb_fd < 0) { perror("open fb"); return; }

    
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

    int fb_width  = vinfo.xres;
    int fb_height = vinfo.yres;
    int fb_bpp    = vinfo.bits_per_pixel; 
    int fb_size   = finfo.smem_len;


    unsigned char* fb_ptr = mmap(0, fb_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);

    while(1) {

    printf("fire_cursor_init !!!!!!\n");
    // 1. Framebuffer 정보 얻기


    // 2. mmap으로 Framebuffer 포인터 얻기
    if (fb_ptr == MAP_FAILED) {
        perror("mmap");
        close(fb_fd);
        return;
    }

    // 3. 프레임 루프
    for (int i = 1; i <= FRAME_NUM; ++i) {
        char fname[256];
        snprintf(fname, sizeof(fname), FRAME_PATH_FMT, i);  //#define FRAME_PATH_FMT "fire_img/output_%04d.jpg"

        // libjpeg: JPEG 파일 로드
        FILE* fp = fopen(fname, "rb");
        if (!fp) { perror(fname); break; }

        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, fp);
        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        int img_width = cinfo.output_width;
        int img_height = cinfo.output_height;
        int row_stride = img_width * cinfo.output_components;

        unsigned char* buffer = malloc(row_stride * img_height);
        // 한줄씩 읽기

        while (cinfo.output_scanline < img_height) {
            unsigned char* rowptr = buffer + cinfo.output_scanline * row_stride;
            jpeg_read_scanlines(&cinfo, &rowptr, 1);
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);

        // 4. 각 fire[i] 값에 따라 이미지 그리기
        for (int j = 0; j < 9; ++j) {
            
            if (fire[j] == 1) {
                int start_x = grid_start_coords[j][0];
                int start_y = grid_start_coords[j][1];

                for (int y = 0; y < img_height && y + start_y < fb_height; ++y) {
                    for (int x = 0; x < img_width && x + start_x < fb_width; ++x) {
                        int fb_offset = ((y + start_y) * fb_width + (x + start_x)) * (fb_bpp / 8);
                        unsigned char r = buffer[(y * img_width + x) * 3 + 0];
                        unsigned char g = buffer[(y * img_width + x) * 3 + 1];
                        unsigned char b = buffer[(y * img_width + x) * 3 + 2];
                        // ARGB8888: A=0xFF(불투명), R, G, B
                        fb_ptr[fb_offset + 0] = b;
                        fb_ptr[fb_offset + 1] = g ;
                        fb_ptr[fb_offset + 2] = r;
                        fb_ptr[fb_offset + 3] = 0xFF;
                    }
                }
            }
        }

        // 커서(빨간 점) 그리기
        int cx = cursor_center[cursor][0];
        int cy = cursor_center[cursor][1];
        
        int point_size = 10; // 점의 크기(반지름), 조정 가능


        for(int dy = -point_size; dy <= point_size; dy++) {
            for(int dx = -point_size; dx <= point_size; dx++) {
                int px = cx + dx;
                int py = cy + dy;
                if(px >= 0 && px < fb_width && py >= 0 && py < fb_height) {
                    int fb_offset = (py * fb_width + px) * (fb_bpp / 8);
                    fb_ptr[fb_offset + 0] = 0xff; // Blue
                    fb_ptr[fb_offset + 1] = 0x00; // Green
                    fb_ptr[fb_offset + 2] = 0x00; // Red
                    fb_ptr[fb_offset + 3] = 0xFF; // Alpha
                }
            }
        }

        // 5초마다(즉, 150프레임마다) 전체 화면을 초기화
        if (cursor_moved) { 
            cursor_moved = 0;
            for (int y = 0; y < 600; y++) {
                for (int x = 0; x < 1024; x++) {
                    int fb_offset = (y * fb_width + x) * (fb_bpp / 8);
                    fb_ptr[fb_offset + 0] = 0x00; // Blue
                    fb_ptr[fb_offset + 1] = 0x00; // Green
                    fb_ptr[fb_offset + 2] = 0x00; // Red
                    fb_ptr[fb_offset + 3] = 0x00; // Alpha (불투명)
                }
            }
        }
        // printf("height :%d \n", fb_height);
        // printf("width :%d \n", fb_width);
        

        free(buffer);

        // 5. 30fps 딜레이
        usleep(1000000 / TARGET_FPS);
    }

    }
    munmap(fb_ptr, fb_size);
    close(fb_fd);

    

}

int main(void) {
    //connect to server
    printf("init~~~\r\n");
    int sock = init_server(PORT);
    if (sock < 0) return -1;

    // wait for client
    while (1) {
        int len = recv(sock, &(socketData.gameReady), sizeof(int), 0);
        if (socketData.gameReady == -1) {
            printf("received data : %d\n", socketData.gameReady);
            break;
        }
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf ("\n Mutex Init Failed!!\n");
        return 1;
    }
    
    // init sensors (buzzer and colorLED init in thread func)
    textLCDInit();
    ledLibInit();
    // creating thread  
    int err0 = pthread_create(&(tid[0]), NULL,&play_time, NULL);
    int err1 = pthread_create(&(tid[1]), NULL,&fire_array, NULL);
    int err2 = pthread_create(&(tid[2]), NULL,&fire_timer, NULL);
    int err3 = pthread_create(&(tid[3]), NULL,&fnd_display, NULL);
    int err4 = pthread_create(&(tid[4]), NULL,&led_life, NULL);
    int err5 = pthread_create(&(tid[5]), NULL,&color_led, NULL);
    int err6 = pthread_create(&(tid[6]), NULL,&bgm_thread, NULL);
    int err7 = pthread_create(&(tid[7]), NULL,&Fire_and_Cursor, NULL);
    int err8 = pthread_create(&(tid[8]), NULL,&display_frames, NULL);
    int err9 = pthread_create(&(tid[9]), NULL,&fire_clear, NULL);


    if (err0 != 0 | err1 != 0 | err2 != 0 | err3 != 0 | err4 != 0 | err5 != 0 | err6 != 0 | err7 != 0 | err8 != 0 | err9 != 0) {
        printf ("Thread Create Error: \n");
        return 0;
    }


    for (int i = 0; i < 8; i++) {
        ledOnOff(i, 1);
    }

    while(1){
        int len = recv(sock, &(data), sizeof(int), 0);

        // when button preseed
        if (data == -5) { //재원이 수정
            printf("buttonPressed%d!!!!\r\n",water);
            water = 1;
            printf("buttonPressed%d!!!!\r\n",water);
        }
        
        //when cursor moved 
        else if (data >= 0 && data <= 8) {
            socketData.cursor = data;
            cursor = socketData.cursor;
            printf("cursorMoved : %d\r\n", data);
            cursor_moved = 1;
        }
        //sleep(1);
        else if(data == -9) {
            
            for (int i = 0; i < 10; i++) {
                pthread_join (tid[i], NULL);
            }
            textLCDExit();
           
            ledLibExit(); //
            buzzerExit(); //
            colorLED_pwmDisable();//

            close(sock);
            printf("main done\r\n");
            return 0;

        }
    }
    
    
}

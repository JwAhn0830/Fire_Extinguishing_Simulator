#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <jpeglib.h>
#include <time.h>

#define FRAME_PATH_FMT "/home/ecube/jpg/fire_img/output_%04d.jpg"
#define FRAME_NUM      210
#define TARGET_FPS     30

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

void show_fire_and_cursor(int cursor, int fire[9]) {
    // 1. Framebuffer 정보 얻기
    int fb_fd = open("/dev/fb1", O_RDWR);
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
    unsigned char* fb_ptr = mmap(0, fb_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);

    // 3. 프레임 루프
    for (int i = 1; i <= FRAME_NUM; ++i) {
        char fname[256];
        snprintf(fname, sizeof(fname), FRAME_PATH_FMT, i);

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
                        fb_ptr[fb_offset + 1] = g;
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
        free(buffer);

        // 5. 30fps 딜레이
        usleep(1000000 / TARGET_FPS);
    }

    munmap(fb_ptr, fb_size);
    close(fb_fd);
}

int main() {
    int cursor = 4;
    int fire[9] = {0, 1, 1, 1, 1, 1, 0, 0, 0};
    show_fire_and_cursor(cursor, fire);
    return 0;
}



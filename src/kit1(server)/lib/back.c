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

#define FRAME_PATH_FMT "jpg/output_%04d.jpg"  // 파일명 형식 수정
#define FRAME_NUM      704               // 예: 1분=30fps*60초
#define TARGET_FPS     30

// Framebuffer 포맷이 32bpp(ARGB8888)임을 가정
typedef struct {
    unsigned char r, g, b, a;
} pixel_t;

// 화면에 이미지를 표시하는 함수
void display_frames(const char *frame_dir, int frame_num, int target_fps) {
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
        snprintf(fname, sizeof(fname), frame_dir, i);  // 파일 이름을 output_%04d.jpg로 수정

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

int main() {
    // 함수 호출
    display_frames(FRAME_PATH_FMT, FRAME_NUM, TARGET_FPS);
    return 0;
}


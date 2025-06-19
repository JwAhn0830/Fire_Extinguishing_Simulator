// display_jpeg_fb.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>

/* libjpeg 에러 핸들러 */
struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf buf;
};
typedef struct my_error_mgr * my_err_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo) {
    my_err_ptr err = (my_err_ptr)cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(err->buf, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <JPEG 파일>\n", argv[0]);
        return 1;
    }

    /* 1) JPEG 파일 오픈 및 헤더 읽기 */
    const char *jpg = argv[1];
    FILE *fp = fopen(jpg, "rb");
    if (!fp) { perror("fopen"); return 1; }

    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.buf)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return 1;
    }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int img_w = cinfo.output_width;
    int img_h = cinfo.output_height;
    int comps = cinfo.output_components; // 보통 3 (RGB)

    /* 한 줄씩 읽기 위한 버퍼 할당 */
    uint8_t *row = malloc(img_w * comps);
    if (!row) { perror("malloc"); return 1; }

    /* 2) 프레임버퍼 오픈 및 정보 얻기 */
    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) { perror("open /dev/fb0"); return 1; }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0 ||
        ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        perror("ioctl");
        return 1;
    }

    /* 3) mmap으로 메모리 매핑 */
    size_t screensize = finfo.smem_len;
    uint8_t *fbp = mmap(NULL, screensize,
                        PROT_READ|PROT_WRITE,
                        MAP_SHARED, fb_fd, 0);
    if (fbp == MAP_FAILED) { perror("mmap"); return 1; }

    /* 4) JPEG → 프레임버퍼 복사 (32bpp XRGB8888 가정) */
    int stride = finfo.line_length / 4; // 4바이트 단위
    for (int y = 0; y < img_h && cinfo.output_scanline < img_h; y++) {
        jpeg_read_scanlines(&cinfo, &row, 1);
        uint32_t *pixel = (uint32_t *)fbp + y * stride;
        for (int x = 0; x < img_w; x++) {
            uint8_t r = row[x * comps + 0];
            uint8_t g = row[x * comps + 1];
            uint8_t b = row[x * comps + 2];
            pixel[x] = (r << 16) | (g << 8) | b;
        }
    }

    /* 5) 정리 */
    munmap(fbp, screensize);
    close(fb_fd);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
    free(row);

    return 0;
}

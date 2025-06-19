#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "fnd.h"

// #define ONE_SEG_DISPLAY_TIME_USEC	1000

int fndInit(int num, int dotflag) {
	int fd;
	stFndWriteForm stWriteData;

	int is_negative = 0;
	if (num < 0) {
		is_negative = 1;
		num = -num;
	}

	// 1. 숫자 자릿수 파악
	int digits[6] = {0};
	int digit_count = 0;
	int temp = num;

	if (temp == 0) {
		digits[5] = 0;
		digit_count = 1;
	} else {
		while (temp > 0 && digit_count < 6) {
			digits[5 - digit_count] = temp % 10;
			temp /= 10;
			digit_count++;
		}
	}

	// 2. dot 설정 (bitmask 기반)
	for (int i = 0; i < 6; i++) {
		stWriteData.dataDot[i] = (dotflag & (1 << i)) ? 1 : 0;
		stWriteData.dataVaild[i] = 0;  // 우선 모두 꺼둠
		stWriteData.dataNumeric[i] = 0;
	}

	// 3. 숫자 넣기 (오른쪽 정렬)
	for (int i = 0; i < digit_count; i++) {
		stWriteData.dataNumeric[6 - digit_count + i] = digits[6 - digit_count + i];
		stWriteData.dataVaild[6 - digit_count + i] = 1;
	}

	// 4. 음수면 앞 칸에 '-' 넣기
	if (is_negative && (6 - digit_count - 1) >= 0) {
		stWriteData.dataNumeric[6 - digit_count - 1] = 10;  // '-' assumed to be index 10
		stWriteData.dataVaild[6 - digit_count - 1] = 1;
	}

	// 5. 드라이버 호출
	fd = open(FND_DRIVER_PATH, O_RDWR);
	if (fd < 0) {
		perror("driver open error.\n");
		return 0;
	}

	write(fd, &stWriteData, sizeof(stFndWriteForm));
	close(fd);
	return 1;
}


int fndsetZero() {
    int fd;
    stFndWriteForm stWriteData;
    for (int i = 0; i < 6; i++) {
        stWriteData.dataNumeric[i] = 0;
    }

    fd = open(FND_DRIVER_PATH,O_RDWR);
	if ( fd < 0 ) {
		perror("driver open error.\n");
		return 0;
	}	
	write(fd,&stWriteData,sizeof(stFndWriteForm));
	close(fd);
	return 1;

}
// count time
int fndTimer(int time, int MODE) { 
    int fd;
	stFndWriteForm stWriteData;

    if (MODE == UPCOUNT) {
        int num = 0;
        while(1) {
            if (num > time) {
                break;
            }
            else {
                fndInit(num, 0);
                printf("%d \r\n", num);
                num++;
                sleep(1);

            }
        }
    }
    else if (MODE == DOWNCOUNT) {
        while(1) {
            if (time < 0) {
                break;
            }
            else {
                fndInit(time, 0);
                printf("%d \r\n", time);
                time--;
                sleep(1);

            }
        }
    }
    else {
        printf("wrong mode input\r\n");
        return 0;
    }

    return 1;
}
int fndExit() {
	int fd;
	stFndWriteForm stWriteData;
	
	for (int i = 0; i < MAX_FND_NUM ; i++ ) {
		stWriteData.dataDot[i] =  0;  
		stWriteData.dataNumeric[i] = 0;
		stWriteData.dataVaild[i] = 0;
	}
	fd = open(FND_DRIVER_PATH,O_RDWR);
	if ( fd < 0 ) {
		perror("driver open error.\n");
		return 0;
	}	
	write(fd,&stWriteData,sizeof(stFndWriteForm));
	close(fd);
	return 1;
}
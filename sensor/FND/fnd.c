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
	int temp;
	stFndWriteForm stWriteData;
	
	for (int i = 0; i < MAX_FND_NUM ; i++ )
	{
		stWriteData.dataDot[i] = (dotflag & (0x1 << i)) ? 1 : 0;  
		stWriteData.dataVaild[i] = 1;
	}
	// if 6 fnd
	temp = num % 1000000;	stWriteData.dataNumeric[0]= temp /100000;
	temp = num % 100000;	stWriteData.dataNumeric[1]= temp /10000;
	temp = num % 10000;		stWriteData.dataNumeric[2] = temp /1000;
	temp = num %1000;		stWriteData.dataNumeric[3] = temp /100;
	temp = num %100;		stWriteData.dataNumeric[4] = temp /10;
							stWriteData.dataNumeric[5] = num %10;

	fd = open(FND_DRIVER_PATH,O_RDWR);
	if ( fd < 0 ) {
		perror("driver open error.\n");
		return 0;
	}	
	write(fd,&stWriteData,sizeof(stFndWriteForm));
	close(fd);
	return 1;
}

void fndsetZero() {
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
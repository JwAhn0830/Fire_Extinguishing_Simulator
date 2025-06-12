#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "textlcdtest.h"

int fd;
stTextLCD stlcd;
int textLCDInit() {
	fd = open(TEXTLCD_DRIVER_NAME, O_RDWR);
	if (fd < 0) {
		perror("TEXTLCD driver open error\r\n");
		return 1;
	}
	memset(&stlcd, 0, sizeof(stTextLCD));
}

void textLCDWrite(const char *str, int line) {


	if(line < 1 || line > 2) {
		printf("wrong line : %d, should be 1 or 2\r\n", line);
		return;
	}
	
	stlcd.cmdData = line;
	int len = strlen(str);
	if (len > COLUMN_NUM) {
		memcpy(stlcd.TextData[stlcd.cmdData - 1], str, COLUMN_NUM);
	}
	else {
		memcpy(stlcd.TextData[stlcd.cmdData - 1], str, len);
	}
	stlcd.cmd = CMD_WRITE_STRING;

	write(fd, &stlcd, sizeof(stTextLCD));
}

void textLCDClear() {
	char *clearStr = {"                "};
	textLCDWrite(clearStr, 1);
	textLCDWrite(clearStr, 2);
	printf("clear LCD\n");
}

void textLCDExit() {
	textLCDClear();
	printf("close file\n");
	close(fd);
}
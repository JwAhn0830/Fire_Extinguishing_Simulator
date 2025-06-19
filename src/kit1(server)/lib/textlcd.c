// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ioctl.h>
// #include <ctype.h>
// #include <sys/ipc.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include "textlcddrv.h" 
// #include "textlcd.h"

// int fd = 0;

// void doHelp(void)
// {
//     printf("usage: textlcdtest <linenum> <'string'>\n");
//     printf("       linenum => 1 ~ 2\n");
//     printf("  ex) textlcdtest 2 'test hello'\n");
// }

// int TlcdLibInit() 
// {
//     fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
//     return fd;
// }


// int lcdtextwrite(const char  *str, int lineFlag)
// {
//    stTextLCD  stlcd;

//    if(lineFlag != 1 && lineFlag != 2){
//        return -1;
//    }
   
//    unsigned int linenum = lineFlag;

// 	if ( linenum == 1) // firsst line
// 	{
// 		stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
// 	}
// 	else// second line
// 	{
// 		stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
// 	}


// 	int len = strlen(str);
// 	if ( len > COLUMN_NUM)
// 	{
// 		memcpy(stlcd.TextData[stlcd.cmdData - 1],str,COLUMN_NUM);
// 	}
// 	else
// 	{
// 		memcpy(stlcd.TextData[stlcd.cmdData - 1],str,len);
// 	}
// 	stlcd.cmd = CMD_WRITE_STRING;
// 	// open  driver 
	
	
// 	write(fd,&stlcd,sizeof(stTextLCD));
// }

// int TlcdLibExit(){
//     lcdtextwrite("",0);
//     close(fd);
//     return 0;
// }

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
#include "textlcd.h"

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
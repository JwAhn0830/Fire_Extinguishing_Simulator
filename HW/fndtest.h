#ifndef _FNDTEST_H_
#define _FNDTEST_H_

#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN (MAX_FND_NUM + 2)
#define FND_DRIVER_PATH "/dev/perifnd"
#define UPCOUNT 0
#define DOWNCOUNT 1

typedef struct FNDWriteDateForm_tag {
    char dataNumeric[FND_DATA_BUFF_LEN];
    char dataDot[FND_DATA_BUFF_LEN];
    char dataVaild[FND_DATA_BUFF_LEN];
}stFndWriteForm, *pStFndWriteForm;

int fndInit(int num, int dotflag);
void fndsetZero();
int fndTimer(int time, int MODE);
int fndExit();

#endif 
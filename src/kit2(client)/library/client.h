#ifndef _SOCKET_H_
#define _SOCKET_H_

typedef struct {
    int gameReady;
    int cursor;
    int buttonPressed;
} SOCKET_DATA;

extern SOCKET_DATA socketData;
int init_server(int PORT);
int connect_server(char *SERVER_ADDRESS, int PORT);
#endif


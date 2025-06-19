#pragma once

typedef struct {
    int gameReady;
    int cursor;
    int buttonPressed;
} SOCKET_DATA;

extern SOCKET_DATA socketData;

int init_server(int PORT);


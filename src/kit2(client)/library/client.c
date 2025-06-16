#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket.h"


/*
    fail to coneect : reutrn -1
*/ 

int init_server(int PORT) {
    int sock, client_fd;
    struct sockaddr_in server_addr, client_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 모든 IP에서 수신
    server_addr.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sock, 1);

    printf("Waiting for message..\n");

    return sock; 
}

int connect_server(char *SERVER_ADDRESS, int PORT) {
    int sock;
    struct sockaddr_in server_addr;

    // create socket 
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("Creating Socket Failed\r\n");
        return -1;
    }

    // set server addr 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &server_addr.sin_addr);

    // connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock);
        return -1;
    }

    printf("Connected to Server!\r\n");
    printf("IP Address : %s\r\n", SERVER_ADDRESS);
    return sock;
}
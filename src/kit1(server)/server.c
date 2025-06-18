#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"


// 전역 변수로 서버 소켓 유지
static int client_sock = -1;

int init_server(int port) {
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int opt = 1;

    // 1. 소켓 생성
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return -1;
    }

    // 2. 옵션 설정 (포트 재사용)
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_sock);
        return -1;
    }

    // 3. 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // 4. 바인드
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_sock);
        return -1;
    }

    // 5. 리슨
    if (listen(server_sock, 1) < 0) {
        perror("listen");
        close(server_sock);
        return -1;
    }

    printf("Waiting for client...\n");

    // 6. 클라이언트 연결 수락
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("accept");
        close(server_sock);
        return -1;
    }

    printf("Client connected!\n");

    // 리슨 소켓은 닫고 클라이언트 소켓만 유지
    close(server_sock);
    return client_sock;
}
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BUF_SIZE 512

void (*on_peer_msg)(const char *msg) = NULL;

void *server_thread(void *arg) {
    int port = *(int*)arg;
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0) { perror("socket"); exit(1); }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); exit(1); }
    if(listen(server_sock, 1) < 0) { perror("listen"); exit(1); }

    fd_set readfds;
    struct timeval timeout;
    char buf[BUF_SIZE];

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(server_sock, &readfds);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int sel = select(server_sock + 1, &readfds, NULL, NULL, &timeout);
        if(sel < 0) { perror("select"); continue; }

        if(FD_ISSET(server_sock, &readfds)) {
            int client = accept(server_sock, NULL, NULL);
            if(client < 0) { perror("accept"); continue; }

            while(1) {
                int n = recv(client, buf, BUF_SIZE-1, 0);
                if(n <= 0) break;
                buf[n] = '\0';
                if(on_peer_msg) on_peer_msg(buf);
            }
            close(client);
        }
    }
}

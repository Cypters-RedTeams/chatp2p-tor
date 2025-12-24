#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "socks5.h"
#include "server.h"

#define BUF_SIZE 512
#define WAIT_SECONDS 60

#define COLOR_YOU "\033[1;32m"
#define COLOR_PEER "\033[1;34m"
#define COLOR_RESET "\033[0m"

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void print_peer(const char *msg) {
    pthread_mutex_lock(&print_mutex);
    printf("\n" COLOR_PEER "[Incoming Peer]: %s\n" COLOR_RESET, msg);
    printf(COLOR_YOU "You: " COLOR_RESET);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);
}

void on_server_msg(const char *msg) {
    print_peer(msg);
}

void *recv_thread(void *arg) {
    int sock = *(int*)arg;
    char buf[BUF_SIZE];

    while(1) {
        int n = recv(sock, buf, BUF_SIZE-1, 0);
        if(n <= 0) {
            print_peer("Peer disconnected.");
            break;
        }
        buf[n] = '\0';
        print_peer(buf);
    }

    close(sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Usage: %s <LOCAL_PORT> <PEER_ONION> <PEER_PORT>\n", argv[0]);
        return 1;
    }

    int local_port = atoi(argv[1]);
    char *peer_onion = argv[2];
    int peer_port = atoi(argv[3]);

    on_peer_msg = on_server_msg;

    pthread_t st;
    pthread_create(&st, NULL, server_thread, &local_port);

    int sock = -1;
    time_t start = time(NULL);
    while(difftime(time(NULL), start) < WAIT_SECONDS) {
        sock = connect_socks5(peer_onion, peer_port);
        if(sock >= 0) break;
        printf("Peer not available yet, retrying...\n");
        sleep(1);
    }

    if(sock < 0) {
        printf("Failed to connect to peer after %d seconds.\n", WAIT_SECONDS);
        return 1;
    }

    printf(COLOR_PEER "Connected to peer %s:%d via Tor SOCKS5!\n" COLOR_RESET, peer_onion, peer_port);

    pthread_t rt;
    pthread_create(&rt, NULL, recv_thread, &sock);

    char buf[BUF_SIZE];
    while(1) {
        pthread_mutex_lock(&print_mutex);
        printf(COLOR_YOU "You: " COLOR_RESET);
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);

        if(fgets(buf, BUF_SIZE, stdin) == NULL) break;

        size_t len = strlen(buf);
        if(len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        if(strlen(buf) == 0) continue;

        if(send(sock, buf, strlen(buf), 0) < 0) {
            perror("send");
            break;
        }
    }

    close(sock);
    return 0;
}

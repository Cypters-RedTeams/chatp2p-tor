#include "socks5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TOR_SOCKS_IP "127.0.0.1"
#define TOR_SOCKS_PORT 9050

int connect_socks5(const char *onion_host, int onion_port) {
    int sock;
    struct sockaddr_in tor_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) { perror("socket"); return -1; }

    tor_addr.sin_family = AF_INET;
    tor_addr.sin_port = htons(TOR_SOCKS_PORT);
    tor_addr.sin_addr.s_addr = inet_addr(TOR_SOCKS_IP);

    if(connect(sock, (struct sockaddr*)&tor_addr, sizeof(tor_addr)) < 0) {
        perror("connect Tor SOCKS5");
        close(sock);
        return -1;
    }

    unsigned char handshake[3] = {0x05, 0x01, 0x00}; // VER=5, NMETHODS=1, METHOD=0
    if(send(sock, handshake, 3, 0) != 3) { perror("send handshake"); close(sock); return -1; }

    unsigned char resp[2];
    if(recv(sock, resp, 2, 0) != 2) { perror("recv handshake"); close(sock); return -1; }
    if(resp[1] != 0x00) { printf("SOCKS5 auth failed\n"); close(sock); return -1; }

    unsigned char req[512];
    size_t len = 0;
    req[len++] = 0x05; // VER
    req[len++] = 0x01; // CONNECT
    req[len++] = 0x00; // RSV
    req[len++] = 0x03; // ATYP DOMAINNAME
    size_t host_len = strlen(onion_host);
    req[len++] = (unsigned char)host_len;
    memcpy(req+len, onion_host, host_len);
    len += host_len;
    req[len++] = (onion_port >> 8) & 0xFF;
    req[len++] = onion_port & 0xFF;

    if(send(sock, req, len, 0) != (ssize_t)len) { perror("send CONNECT"); close(sock); return -1; }
    if(recv(sock, resp, 2, 0) != 2) { perror("recv CONNECT"); close(sock); return -1; }
    if(resp[1] != 0x00) { printf("SOCKS5 connect failed\n"); close(sock); return -1; }

    unsigned char tmp[256];
    recv(sock, tmp, sizeof(tmp), 0); // skip remaining bytes
    return sock;
}

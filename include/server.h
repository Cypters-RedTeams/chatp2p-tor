#ifndef SERVER_H
#define SERVER_H

void *server_thread(void *arg);

extern void (*on_peer_msg)(const char *msg);

#endif

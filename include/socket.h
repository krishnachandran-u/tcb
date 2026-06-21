/* include/socket.h */
#ifndef SOCKET_H
#define SOCKET_H

#define SOCKET_MAX_EVENTS 64
#define SOCKET_MAX_CLIENTS 128

int socket_init(int *out_listen_fd);
void socket_process_events(int epoll_fd, int listen_fd);

#endif // SOCKET_H
/* socket.h */
#ifndef SOCKET_H
#define SOCKET_H

int socket_init(void);
void socket_process_events(int epoll_fd, int listen_fd);

#endif // SOCKET_H
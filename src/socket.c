/* socket.c */
#include "main.h"

#define MAX_EVENTS 64

static int make_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        LOG_ERROR("fcntl F_GETFL failed");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        LOG_ERROR("fcntl F_SETFL O_NONBLOCK failed");
        return -1;
    }
    return 0;
}

int socket_init(void) {
    int listen_fd;
    struct sockaddr_un addr;

    unlink(SOCKET_PATH);

    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        LOG_FATAL("Failed to create UNIX socket");
    }

    if (make_non_blocking(listen_fd) < 0) {
        close(listen_fd);
        LOG_FATAL("Failed to set listen socket to non-blocking");
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        close(listen_fd);
        LOG_FATAL("Failed to bind socket to path: %s", SOCKET_PATH);
    }

    if (listen(listen_fd, 128) < 0) {
        close(listen_fd);
        LOG_FATAL("Socket listen configuration failed");
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        close(listen_fd);
        LOG_FATAL("Failed to initialize epoll instance");
    }

    struct epoll_event ev;
    ev.events = EPOLLIN; 
    ev.data.fd = listen_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
        close(listen_fd);
        close(epoll_fd);
        LOG_FATAL("Failed to add listen socket to epoll instance descriptor");
    }

    LOG_INFO("UNIX Socket initialized asynchronously at %s", SOCKET_PATH);
    return epoll_fd; 
}

static void handle_client_data(int client_fd) {
    protocol_msg_header_t header;
    
    ssize_t n_read = read(client_fd, &header, sizeof(protocol_msg_header_t));
    if (n_read <= 0) {
        close(client_fd);
        return;
    }

    if (n_read < (ssize_t)sizeof(protocol_msg_header_t)) {
        LOG_WARN("Malformed packet header received. Dropping client connection.");
        close(client_fd);
        return;
    }

    if (header.length > MAX_PAYLOAD_SIZE) {
        LOG_WARN("Payload size limit overflow (%u bytes). Rejected.", header.length);
        close(client_fd);
        return;
    }

    char payload_buf[MAX_PAYLOAD_SIZE + 1];
    memset(payload_buf, 0, sizeof(payload_buf));

    size_t total_bytes_read = 0;
    while (total_bytes_read < header.length) {
        ssize_t bytes = read(client_fd, payload_buf + total_bytes_read, header.length - total_bytes_read);
        if (bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue; 
            LOG_ERROR("Error reading payload from client descriptor");
            close(client_fd);
            return;
        }
        if (bytes == 0) break; 
        total_bytes_read += bytes;
    }

    if (header.type == MSG_CLIP_PUSH) {
        LOG_INFO("[CLIP RECEIVED]: %s", payload_buf);
    } else if (header.type == MSG_CLIP_QUERY) {
        LOG_INFO("[QUERY INCOMING]: Client asked for clip array.");
    }

    close(client_fd);
}

void socket_process_events(int epoll_fd, int listen_fd) {
    struct epoll_event events[MAX_EVENTS];
    
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds < 0) {
        if (errno == EINTR) return; 
        LOG_ERROR("epoll_wait loop failure encountered");
        return;
    }

    for (int i = 0; i < nfds; i++) {
        if (events[i].data.fd == listen_fd) {
            while (1) {
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    }
                    LOG_ERROR("Failed to accept incoming connection request");
                    break;
                }

                if (make_non_blocking(client_fd) < 0) {
                    close(client_fd);
                    continue;
                }

                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                    LOG_ERROR("Failed to add client socket wrapper context to epoll infrastructure");
                    close(client_fd);
                }
            }
        } else {
            handle_client_data(events[i].data.fd);
        }
    }
}
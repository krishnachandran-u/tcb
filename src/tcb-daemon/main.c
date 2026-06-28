/* src/tcb-daemon/main.c */
#include "tcb.h"

static volatile sig_atomic_t keep_running = 1;

static void handle_signal(int sig) {
    (void)sig;
    keep_running = 0; 
}

int main(void) {
    LOG_INIT(LOG_FILE_PATH);
    
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    LOG_INFO("Starting tcb daemon in foreground...");

    daemon_write_pid_file();
    LOG_INFO("PID file managed successfully.");

    storage_init();
    
    int listen_fd = -1;
    int epoll_fd = socket_init(&listen_fd);

    LOG_INFO("tcb-daemon async engine armed. Waiting for clips...");
    
    while (keep_running) {
        socket_process_events(epoll_fd, listen_fd); 
    }
    
    LOG_INFO("Shutting down tcb daemon gracefully...");
    close(epoll_fd);
    close(listen_fd);
    unlink(SOCKET_PATH);
    LOG_CLOSE(); 
    
    return EXIT_SUCCESS;
}
/* main.c */
#include "main.h"

static volatile sig_atomic_t keep_running = 1;

static void handle_signal(int sig) {
    (void)sig;
    keep_running = 0; 
}

static void run_daemon(void) {
    while (keep_running) {
        LOG_INFO("tcb daemon heartbeat pulse");
        sleep(10); 
    }
}

int main(void) {
    LOG_INIT(LOG_FILE_PATH);
    
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    daemon_init();
    
    run_daemon();
    
    LOG_INFO("Shutting down tcb daemon gracefully...");
    LOG_CLOSE(); 
    
    return EXIT_SUCCESS;
}
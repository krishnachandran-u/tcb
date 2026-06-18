/* main.c */
#include "main.h"
#include "daemon.h"
#include "log.h"
#include <stdlib.h>

static void run_daemon(void) {
    while (1) {
        LOG_INFO("tcb daemon heartbeat pulse");
        sleep(10); 
    }
}

int main(void) {
    LOG_INIT(LOG_FILE_PATH);
    run_daemon();
    LOG_CLOSE(); 
    return EXIT_SUCCESS;
}
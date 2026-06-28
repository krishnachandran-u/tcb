/* src/tcb-daemon/daemon.c */
#include "tcb.h"

bool suppress_stderr;
static char pid_file_path[STR_MAX_LEN];

// unused function
void daemon_daemonize(void) {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        LOG_FATAL("First fork failed");
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        LOG_FATAL("setsid failed");
    }

    signal(SIGCHLD, SIG_IGN); 
    signal(SIGHUP, SIG_IGN);  

    pid = fork();
    if (pid < 0) {
        LOG_FATAL("Second fork failed");
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0022); 

    if (chdir("/") < 0) {
        LOG_FATAL("chdir to root failed");
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    suppress_stderr = true;

    int dev_null = open("/dev/null", O_RDWR);
    if (dev_null >= 0) {
        dup2(dev_null, STDIN_FILENO);
        dup2(dev_null, STDOUT_FILENO);
        dup2(dev_null, STDERR_FILENO);
        if (dev_null > STDERR_FILENO) {
            close(dev_null);
        }
    }
}

void daemon_write_pid_file(void) {
    snprintf(pid_file_path, STR_MAX_LEN, PID_FILE_PATH, getuid());
    int fd = open(pid_file_path, O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        LOG_FATAL("Failed to open PID file: %s", pid_file_path);
    }

    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    
    if (fcntl(fd, F_SETLK, &fl) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            LOG_FATAL("Another instance of the daemon is already running.");
        }
        close(fd);
        LOG_FATAL("Failed to lock PID file");
    }

    if (ftruncate(fd, 0) < 0) {
        close(fd);
        LOG_FATAL("Failed to truncate PID file");
    }
    
    char buf[32];
    snprintf(buf, sizeof(buf), "%d\n", getpid());
    if (write(fd, buf, strlen(buf)) < 0) {
        close(fd);
        LOG_FATAL("Failed to write to PID file");
    }
}

void daemon_init(void) {
    LOG_INFO("Starting tcb daemon...");

    daemon_daemonize();
    LOG_INFO("Daemonization successful.");

    daemon_write_pid_file();
    LOG_INFO("PID file created successfully at: %s", pid_file_path);
}
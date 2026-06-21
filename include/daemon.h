/* include/daemon.h */
#ifndef DAEMON_H
#define DAEMON_H

#define PID_FILE_PATH "/run/user/%d/clipd.pid" 

void daemon_daemonize();
void daemon_write_pid_file();
void daemon_init();

#endif // DAEMON_H
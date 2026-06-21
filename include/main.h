/* main.h */
#ifndef MAIN_H
#define MAIN_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>

#include "daemon.h"
#include "log.h"
#include "defines.h"
#include "protocol.h"
#include "socket.h"

#endif // MAIN_H
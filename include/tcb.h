/* include/tcb.h */
/* Master header file for the TCB application */
#ifndef TCB_H
#define TCB_H

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
#include <sqlite3.h>

#include "core.h"

#include "daemon.h"
#include "log.h"
#include "protocol.h"
#include "socket.h"
#include "storage.h"
#include "helper.h"
#endif // TCB_H
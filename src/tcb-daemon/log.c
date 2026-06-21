/* src/tcb-daemon/log.c */
#include "main.h"

static FILE *log_fp = NULL;
extern bool suppress_stderr;

static const char *level_to_string(log_level_t level)
{
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        default:              return "UNKNOWN";
    }
}

static void get_timestamp(char *buf, size_t size)
{
    time_t now = time(NULL);

    struct tm tm_now;
    localtime_r(&now, &tm_now);

    strftime(buf, size, "%Y-%m-%d %H:%M:%S", &tm_now);
}

void _log_init(const char *filepath)
{
    log_fp = fopen(filepath, "a");

    if (!log_fp) {
        perror("log file");
        exit(EXIT_FAILURE);
    }
}

void _log_close(void)
{
    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}

void _log_message(
    log_level_t level,
    const char *file,
    const char *func,
    int line,
    int use_errno,
    const char *fmt,
    ...
)
{
    int saved_errno = errno;

    char ts[32];
    get_timestamp(ts, sizeof(ts));

    FILE *outputs[2] = { stderr, log_fp };

    for (size_t i = 0; i < 2; i++) {
        if (i == 0 && suppress_stderr) {
            continue;
        }
        FILE *out = outputs[i];

        if (!out)
            continue;

        fprintf(out,
                "[%s] %-5s %s:%d (%s): ",
                ts,
                level_to_string(level),
                file,
                line,
                func);

        va_list args;
        va_start(args, fmt);
        vfprintf(out, fmt, args);
        va_end(args);

        if (use_errno && saved_errno != 0) {
            fprintf(out,
                    " | errno=%d (%s)",
                    saved_errno,
                    strerror(saved_errno));
        }

        fprintf(out, "\n");
        fflush(out);
    }
}
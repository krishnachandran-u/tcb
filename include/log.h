/* log.h */
#ifndef LOG_H
#define LOG_H

#define LOG_FILE_PATH "/tmp/tcb.log" 

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} log_level_t;

void _log_init(const char *filepath);

void _log_message(
    log_level_t level,
    const char *file,
    const char *func,
    int line,
    int use_errno,
    const char *fmt,
    ...
);

void _log_close(void);

#define LOG_INIT(filepath) _log_init(filepath)

#define LOG_CLOSE() _log_close()

#define LOG_DEBUG(...) \
    _log_message(LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, 0, __VA_ARGS__)

#define LOG_INFO(...) \
    _log_message(LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, 0, __VA_ARGS__)

#define LOG_WARN(...) \
    _log_message(LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, 0, __VA_ARGS__)

#define LOG_ERROR(...) \
    _log_message(LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, 1, __VA_ARGS__)

#define LOG_FATAL(...)                                            \
    do {                                                          \
        _log_message(LOG_LEVEL_FATAL,                        \
                          __FILE__,                               \
                          __func__,                               \
                          __LINE__,                               \
                          1,                                      \
                          __VA_ARGS__);                           \
        exit(EXIT_FAILURE);                                       \
    } while (0)

#endif // LOG_H
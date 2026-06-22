/* storage.h */
#ifndef STORAGE_H
#define STORAGE_H

#include "core.h" 

#define STORAGE_MAX_ENTRIES 500
#define STORAGE_FILE_PATH "/tmp/tcb.db"

typedef struct __attribute__((packed)) {
    uint64_t timestamp;
    uint32_t length;
} storage_file_header_t;

typedef struct {
    uint64_t timestamp;
    char *data;
    uint32_t length;
} clip_entry_t;

void storage_init(void);
void storage_push(const char *data, size_t len);
void storage_get_all(clip_entry_t *out_buffer, size_t *out_count);
void storage_free(void);

#endif // STORAGE_H
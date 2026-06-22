/* src/tcb-daemon/storage.c */
#include "tcb.h"

static clip_entry_t ring_buffer[STORAGE_MAX_ENTRIES];
static size_t ring_head = 0; 
static size_t ring_count = 0; 

static void free_entry(clip_entry_t *entry) {
    if (entry->data) {
        free(entry->data);
        entry->data = NULL;
    }
    entry->length = 0;
    entry->timestamp = 0;
}

static void ring_push_mem(const char *data, size_t len, uint64_t timestamp) {
    free_entry(&ring_buffer[ring_head]);

    ring_buffer[ring_head].data = malloc(len + 1);
    if (!ring_buffer[ring_head].data) {
        LOG_ERROR("Failed to allocate memory for ring buffer entry");
        return;
    }

    memcpy(ring_buffer[ring_head].data, data, len);
    ring_buffer[ring_head].data[len] = '\0';
    ring_buffer[ring_head].length = (uint32_t)len;
    ring_buffer[ring_head].timestamp = timestamp;

    ring_head = (ring_head + 1) % STORAGE_MAX_ENTRIES;
    if (ring_count < STORAGE_MAX_ENTRIES) {
        ring_count++;
    }
}

void storage_init(void) {
    memset(ring_buffer, 0, sizeof(ring_buffer));

    FILE *fp = fopen(STORAGE_FILE_PATH, "rb");
    if (!fp) {
        LOG_INFO("No existing clip database file found at %s. Creating new on push.", STORAGE_FILE_PATH);
        return;
    }

    LOG_INFO("Loading past history clips from persistent storage database...");
    
    storage_file_header_t header;
    char load_buf[MAX_PAYLOAD_SIZE + 1];

    while (fread(&header, sizeof(storage_file_header_t), 1, fp) == 1) {
        if (header.length > MAX_PAYLOAD_SIZE) {
            LOG_ERROR("Database file corruption or upper limit sizing error detected. Aborting read.");
            break;
        }

        if (fread(load_buf, 1, header.length, fp) != header.length) {
            LOG_ERROR("Incomplete block chunk data read hit. Database truncated?");
            break;
        }

        load_buf[header.length] = '\0';
        ring_push_mem(load_buf, header.length, header.timestamp);
    }

    fclose(fp);
    LOG_INFO("Successfully restored %zu clip records into active memory cache context.", ring_count);
}

void storage_push(const char *data, size_t len) {
    if (len == 0 || len > MAX_PAYLOAD_SIZE) return;

    uint64_t now = (uint64_t)time(NULL);

    FILE *fp = fopen(STORAGE_FILE_PATH, "ab");
    if (!fp) {
        LOG_ERROR("Unable to open storage flat file for appending data log context");
    } else {
        storage_file_header_t header = { .timestamp = now, .length = (uint32_t)len };
        if (fwrite(&header, sizeof(storage_file_header_t), 1, fp) != 1 ||
            fwrite(data, 1, len, fp) != len) {
            LOG_ERROR("Failed to flush payload transaction records to disk block storage maps");
        }
        fclose(fp);
    }

    ring_push_mem(data, len, now);
}

void storage_get_all(clip_entry_t *out_buffer, size_t *out_count) {
    *out_count = ring_count;
    if (ring_count == 0) return;

    size_t start_idx = (ring_count == STORAGE_MAX_ENTRIES) ? ring_head : 0;

    for (size_t i = 0; i < ring_count; i++) {
        size_t current_ring_pos = (start_idx + i) % STORAGE_MAX_ENTRIES;
        out_buffer[i] = ring_buffer[current_ring_pos];
    }
}

void storage_free(void) {
    for (size_t i = 0; i < STORAGE_MAX_ENTRIES; i++) {
        free_entry(&ring_buffer[i]);
    }
    ring_count = 0;
    ring_head = 0;
}
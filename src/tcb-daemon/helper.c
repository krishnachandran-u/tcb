/* src/tcb-daemon/helper.c */
#include "tcb.h"

uint64_t calculate_fnv1a(const char *data, size_t len) {
    uint64_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)data[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

void get_hash_string(const char *data, size_t len, char *out_str) {
    uint64_t hash = calculate_fnv1a(data, len);
    sprintf(out_str, "%016llx", (unsigned long long)hash);
}
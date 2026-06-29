/* include/helper.h */
#ifndef HELPER_H
#define HELPER_H

#include "core.h" 

uint64_t calculate_fnv1a(const char *data, size_t len);
void get_hash_string(const char *data, size_t len, char *out_str);

#endif // HELPER_H
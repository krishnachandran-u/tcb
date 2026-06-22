/* include/protocol.h */
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "core.h"

#define SOCKET_PATH "/tmp/tcb.sock"
#define MAX_PAYLOAD_SIZE 4096
typedef enum {
    MSG_CLIP_PUSH  = 1,
    MSG_CLIP_QUERY = 2,
    MSG_CLIP_RESP  = 3
} protocol_msg_type_t;

typedef struct __attribute__((packed)) {
    uint32_t type;    
    uint32_t length;  
} protocol_msg_header_t;

#endif // PROTOCOL_H
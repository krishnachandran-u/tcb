/* src/tcb-push/main.c */
#include "tcb.h"

int main(void) {
    char payload_buf[MAX_PAYLOAD_SIZE];
    size_t total_bytes = 0;
    
    size_t bytes_read = fread(payload_buf, 1, MAX_PAYLOAD_SIZE, stdin);
    if (bytes_read == 0) {
        return EXIT_SUCCESS;
    }
    total_bytes = bytes_read;

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("tcb-push: socket creation failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("tcb-push: could not connect to running tcb-daemon");
        close(sockfd);
        return EXIT_FAILURE;
    }

    protocol_msg_header_t header;
    header.type = MSG_CLIP_PUSH;
    header.length = (uint32_t)total_bytes;

    if (write(sockfd, &header, sizeof(protocol_msg_header_t)) < 0) {
        perror("tcb-push: packet header streaming transmission failure");
        close(sockfd);
        return EXIT_FAILURE;
    }

    if (write(sockfd, payload_buf, total_bytes) < 0) {
        perror("tcb-push: data payload streaming transmission failure");
        close(sockfd);
        return EXIT_FAILURE;
    }

    close(sockfd);
    return EXIT_SUCCESS;
}
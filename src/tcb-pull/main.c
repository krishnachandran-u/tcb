/* src/tcb-pull/main.c */
#include "tcb.h"

int main(void) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("tcb-pull: socket creation failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("tcb-pull: cannot connect to daemon");
        close(sockfd);
        return EXIT_FAILURE;
    }

    protocol_msg_header_t header = { .type = MSG_CLIP_QUERY, .length = 0 };
    if (write(sockfd, &header, sizeof(header)) < 0) {
        perror("tcb-pull: send failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    protocol_msg_header_t resp_header;
    if (read(sockfd, &resp_header, sizeof(resp_header)) < sizeof(resp_header)) {
        fprintf(stderr, "tcb-pull: failed to read response header\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

    if (resp_header.type != MSG_CLIP_RESP) {
        fprintf(stderr, "tcb-pull: unexpected protocol message type %u\n", resp_header.type);
        close(sockfd);
        return EXIT_FAILURE;
    }

    uint32_t bytes_processed = 0;
    int index = 0;

    while (bytes_processed < resp_header.length) {
        uint64_t timestamp;
        uint32_t length;
        char data_buf[MAX_PAYLOAD_SIZE + 1];

        read(sockfd, &timestamp, sizeof(uint64_t));
        read(sockfd, &length, sizeof(uint32_t));
        read(sockfd, data_buf, length);
        data_buf[length] = '\0';

        time_t raw_time = (time_t)timestamp;
        struct tm *timeinfo = localtime(&raw_time);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

        printf("[%d] %s | %s\n", index++, time_str, data_buf);

        bytes_processed += sizeof(uint64_t) + sizeof(uint32_t) + length;
    }

    close(sockfd);
    return EXIT_SUCCESS;
}
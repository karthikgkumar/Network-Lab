// client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 2

typedef struct {
    int seq_num;
    char data[BUFFER_SIZE];
} Packet;

typedef struct {
    int ack_num;
} Ack;

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    Packet packet;
    Ack ack;
    struct timeval tv;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Set socket timeout
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int seq_num = 0;
    char input[BUFFER_SIZE];

    while (1) {
        printf("Enter message (or 'quit' to exit): ");
        fgets(input, BUFFER_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline

        if (strcmp(input, "quit") == 0) {
            break;
        }

        packet.seq_num = seq_num;
        strncpy(packet.data, input, BUFFER_SIZE);

        while (1) {
            // Send packet
            sendto(sockfd, &packet, sizeof(Packet), 0,
                   (const struct sockaddr *)&server_addr, addr_len);
            printf("Sent packet %d: %s\n", packet.seq_num, packet.data);

            // Wait for ACK
            int len = recvfrom(sockfd, &ack, sizeof(Ack), MSG_WAITALL,
                               (struct sockaddr *)&server_addr, &addr_len);

            if (len < 0) {
                printf("Timeout. Resending packet %d\n", packet.seq_num);
                continue;
            }

            if (ack.ack_num == seq_num) {
                printf("Received ACK %d\n", ack.ack_num);
                seq_num = 1 - seq_num;  // Toggle between 0 and 1
                break;
            } else {
                printf("Received incorrect ACK. Resending packet %d\n", packet.seq_num);
            }
        }
    }

    close(sockfd);
    return 0;
}

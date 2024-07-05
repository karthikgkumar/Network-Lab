// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8888
#define BUFFER_SIZE 1024

typedef struct {
    int seq_num;
    char data[BUFFER_SIZE];
} Packet;

typedef struct {
    int ack_num;
} Ack;

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    Packet packet;
    Ack ack;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    int expected_seq_num = 0;
    srand(time(NULL));

    while (1) {
        // Receive packet from client
        int len = recvfrom(sockfd, &packet, sizeof(Packet), MSG_WAITALL,
                           (struct sockaddr *)&client_addr, &addr_len);

        if (len < 0) {
            perror("Receive failed");
            continue;
        }

        printf("Received packet %d: %s\n", packet.seq_num, packet.data);

        // Simulate packet loss (10% chance)
        if (rand() % 10 == 0) {
            printf("Simulating packet loss. Not sending ACK.\n");
            continue;
        }

        // Check if packet is the one we're expecting
        if (packet.seq_num == expected_seq_num) {
            ack.ack_num = expected_seq_num;
            expected_seq_num = 1 - expected_seq_num;  // Toggle between 0 and 1
        } else {
            ack.ack_num = 1 - expected_seq_num;
            printf("Received out-of-order packet. Sending ACK for last correct packet.\n");
        }

        // Send ACK
        sendto(sockfd, &ack, sizeof(Ack), 0,
               (const struct sockaddr *)&client_addr, addr_len);
        printf("Sent ACK %d\n", ack.ack_num);
    }

    close(sockfd);
    return 0;
}

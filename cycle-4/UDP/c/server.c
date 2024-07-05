// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8888
#define BUFFER_SIZE 1024

void handle_client(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len) {
    char buffer[BUFFER_SIZE];
    time_t current_time;
    struct tm *time_info;
    char time_string[BUFFER_SIZE];

    // Get current time
    time(&current_time);
    time_info = localtime(&current_time);

    // Format time as a string
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // Send time to client
    sendto(sockfd, time_string, strlen(time_string), 0, 
           (struct sockaddr*)client_addr, addr_len);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

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

    printf("UDP Time Server listening on port %d...\n", PORT);

    while (1) {
        // Receive request from client
        int len = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                           (struct sockaddr *)&client_addr, &addr_len);
        buffer[len] = '\0';

        printf("Received request from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle client request
        handle_client(sockfd, &client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}

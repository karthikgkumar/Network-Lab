// client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *hello = "Request time";

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

    int n;
    socklen_t len;

    // Send time request to server
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, 
           (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Time request sent to server.\n");

    // Receive time from server
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
                 (struct sockaddr *)&server_addr, &len);
    buffer[n] = '\0';
    printf("Server's time: %s\n", buffer);

    close(sockfd);
    return 0;
}

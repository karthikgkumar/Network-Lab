#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    // Send filename to server
    send(sock, argv[1], strlen(argv[1]), 0);

    // Receive and print server response
    ssize_t bytes_received;
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_received);
    }

    if (bytes_received < 0) {
        perror("recv failed");
    }

    close(sock);
    return 0;
}

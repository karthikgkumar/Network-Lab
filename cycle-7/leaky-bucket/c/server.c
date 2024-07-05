#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define BUCKET_SIZE 10
#define OUTPUT_RATE 1  // packets per second

int bucket = 0;

void process_packet(char *packet) {
    printf("Processing packet: %s\n", packet);
    // Simulate packet processing
    usleep(100000);  // Sleep for 100ms
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    time_t last_process_time = time(NULL);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread <= 0) {
                break;
            }

            time_t current_time = time(NULL);
            int time_diff = difftime(current_time, last_process_time);

            // Leak the bucket
            int leaked = time_diff * OUTPUT_RATE;
            bucket = (bucket > leaked) ? (bucket - leaked) : 0;

            // Try to add the new packet to the bucket
            if (bucket < BUCKET_SIZE) {
                bucket++;
                process_packet(buffer);
                printf("Bucket size: %d\n", bucket);
            } else {
                printf("Packet dropped. Bucket is full.\n");
            }

            last_process_time = current_time;
        }

        close(new_socket);
    }

    return 0;
}

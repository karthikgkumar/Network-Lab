#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void get_mac_address(char *mac_address) {
    struct ifreq ifr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    strcpy(ifr.ifr_name, "eth0");  // Change this to your network interface name if different
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(sock);
        exit(1);
    }

    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    snprintf(mac_address, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    close(sock);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char mac_address[18];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        get_mac_address(mac_address);
        send(new_socket, mac_address, strlen(mac_address), 0);
        printf("MAC address sent to client: %s\n", mac_address);

        close(new_socket);
    }

    return 0;
}
/* Here's how this implementation works:

Server (server.c):

Creates a socket and binds it to port 12345.
Listens for incoming connections.
When a client connects, it gets its own MAC address using ioctl with SIOCGIFHWADDR.
Sends the MAC address to the client and closes the connection.


Client (client.c):

Prompts for the IP address of the server.
Creates a socket and connects to the server.
Receives the MAC address sent by the server and prints it.



Note:

The server uses "eth0" as the network interface. You might need to change this to match your system's network interface name (e.g., "en0" for some macOS systems, "wlan0" for some Wi-Fi connections).
This implementation is for Unix-like systems (Linux, macOS). For Windows, you'd need to use different socket and network interface APIs.
The server needs to be run with sufficient privileges to access the network interface information.

Remember to run the server before the client, and ensure both are on the same network or that proper port forwarding is set up if connecting over the internet. */

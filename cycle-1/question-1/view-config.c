// View the configuration, including addresses of your computers network interfaces.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    struct ifaddrs *ifaddr, *ifa; // Struct to hold network interface information
    int family, s;
    char host[NI_MAXHOST];

    // Get all network interfaces information
    if (getifaddrs(&ifaddr) == -1) { // Check if successful
        perror("getifaddrs"); // Print error message
        exit(EXIT_FAILURE); // Exit program with failure status
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue; // Skip if no address associated with the interface

        family = ifa->ifa_addr->sa_family;

        printf("Interface: %s\n", ifa->ifa_name); // Print interface name

        printf("Checking for IPv4 or IPv6...\n");

        // Retrieve IP address and print it based on IPv4 or IPv6
        if (family == AF_INET || family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                          sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s)); // Print error message
                exit(EXIT_FAILURE); // Exit program with failure status
            }

            printf("\tAddress: %s\n", host); // Print IP address
        }
    }

    freeifaddrs(ifaddr); // Free memory allocated for interface info
    return 0; // Return success status
}

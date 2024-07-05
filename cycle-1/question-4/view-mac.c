#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

#define MAC_LENGTH 6

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    struct arpreq arp_req;
    struct sockaddr_in *sin;

    memset(&arp_req, 0, sizeof(arp_req));
    sin = (struct sockaddr_in *)&arp_req.arp_pa;
    sin->sin_family = AF_INET;

    if (inet_aton(argv[1], &sin->sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address: %s\n", argv[1]);
        close(sock);
        exit(1);
    }

    strncpy(arp_req.arp_dev, "eth0", 15);  // Assumes eth0 interface, change if needed

    if (ioctl(sock, SIOCGARP, &arp_req) == -1) {
        perror("ioctl");
        close(sock);
        exit(1);
    }

    unsigned char *mac = (unsigned char *)arp_req.arp_ha.sa_data;

    printf("MAC address for %s: ", argv[1]);
    for (int i = 0; i < MAC_LENGTH; i++) {
        printf("%02X", mac[i]);
        if (i < MAC_LENGTH - 1) printf(":");
    }
    printf("\n");

    close(sock);
    return 0;
}

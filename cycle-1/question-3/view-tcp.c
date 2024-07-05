#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <netinet/tcp.h>

#define BUFSIZE 8192

void print_connections() {
    FILE *fp;
    char buf[BUFSIZE];
    char local_addr[INET6_ADDRSTRLEN], remote_addr[INET6_ADDRSTRLEN];
    int local_port, remote_port;
    
    fp = fopen("/proc/net/tcp", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/net/tcp");
        exit(1);
    }
    
    printf("Active TCP connections:\n");
    printf("Local Address\t\tRemote Address\t\tState\n");
    
    // Skip the header line
    fgets(buf, BUFSIZE, fp);
    
    while (fgets(buf, BUFSIZE, fp) != NULL) {
        unsigned int local_ip, remote_ip, state;
        sscanf(buf, "%*d: %x:%x %x:%x %x", &local_ip, &local_port, &remote_ip, &remote_port, &state);
        
        struct in_addr local_in_addr = { .s_addr = local_ip };
        struct in_addr remote_in_addr = { .s_addr = remote_ip };
        
        inet_ntop(AF_INET, &local_in_addr, local_addr, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET, &remote_in_addr, remote_addr, INET6_ADDRSTRLEN);
        
        printf("%s:%d\t\t%s:%d\t\t", local_addr, local_port, remote_addr, remote_port);
        
        switch(state) {
            case TCP_ESTABLISHED:
                printf("ESTABLISHED\n");
                break;
            case TCP_SYN_SENT:
                printf("SYN_SENT\n");
                break;
            case TCP_SYN_RECV:
                printf("SYN_RECV\n");
                break;
            case TCP_FIN_WAIT1:
                printf("FIN_WAIT1\n");
                break;
            case TCP_FIN_WAIT2:
                printf("FIN_WAIT2\n");
                break;
            case TCP_TIME_WAIT:
                printf("TIME_WAIT\n");
                break;
            case TCP_CLOSE:
                printf("CLOSE\n");
                break;
            case TCP_CLOSE_WAIT:
                printf("CLOSE_WAIT\n");
                break;
            case TCP_LAST_ACK:
                printf("LAST_ACK\n");
                break;
            case TCP_LISTEN:
                printf("LISTEN\n");
                break;
            case TCP_CLOSING:
                printf("CLOSING\n");
                break;
            default:
                printf("UNKNOWN\n");
        }
    }
    
    fclose(fp);
}

int main() {
    printf("Please visit a website now...\n");
    sleep(10);  // Wait for 10 seconds
    print_connections();
    return 0;
}

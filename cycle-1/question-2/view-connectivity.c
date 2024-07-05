#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>

#define PACKET_SIZE 64
#define MAX_WAIT_TIME 1
#define MAX_NO_PACKETS 1

typedef struct ping_pkt {
    struct icmphdr hdr;
    char msg[PACKET_SIZE - sizeof(struct icmphdr)];
} ping_pkt;

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_dom) {
    int ttl_val = 64, msg_count = 0, i, flag = 1, msg_received_count = 0;
    
    ping_pkt pckt;
    struct sockaddr_in r_addr;
    struct timespec time_start, time_end, tfs, tfe;
    long double rtt_msec = 0, total_msec = 0;
    struct timeval tv_out;
    tv_out.tv_sec = MAX_WAIT_TIME;
    tv_out.tv_usec = 0;

    clock_gettime(CLOCK_MONOTONIC, &tfs);

    if (setsockopt(ping_sockfd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0) {
        printf("\nSetting socket options to TTL failed!\n");
        return;
    }

    setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);

    while(msg_count < MAX_NO_PACKETS) {
        flag = 1;
        bzero(&pckt, sizeof(pckt));
        
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        
        for (i = 0; i < sizeof(pckt.msg) - 1; i++)
            pckt.msg[i] = i + '0';
        
        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

        usleep(10000);

        clock_gettime(CLOCK_MONOTONIC, &time_start);
        if (sendto(ping_sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*) ping_addr, 
                    sizeof(*ping_addr)) <= 0) {
            printf("\nPacket Sending Failed!\n");
            flag = 0;
        }

        if (flag) {
            socklen_t addr_len = sizeof(r_addr);

            if (recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0, 
                (struct sockaddr*)&r_addr, &addr_len) <= 0 && msg_count > 1) {
                printf("\nPacket receive failed!\n");
            } else {
                clock_gettime(CLOCK_MONOTONIC, &time_end);
                
                double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
                rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + timeElapsed;
                
                if (flag) {
                    if (!(pckt.hdr.type == 69 && pckt.hdr.code == 0)) {
                        printf("Error..Packet received with ICMP type %d code %d\n", 
                                pckt.hdr.type, pckt.hdr.code);
                    } else {
                        printf("%d bytes from %s (h: %s) msg_seq=%d ttl=%d rtt = %Lf ms.\n", 
                                PACKET_SIZE, ping_dom, inet_ntoa(r_addr.sin_addr), 
                                msg_count, ttl_val, rtt_msec);
                        msg_received_count++;
                    }
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &tfe);
    double timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec)) / 1000000.0;
    
    total_msec = (tfe.tv_sec - tfs.tv_sec) * 1000.0 + timeElapsed;
    
    printf("\n===%s ping statistics===\n", ping_dom);
    printf("\n%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\n", 
            msg_count, msg_received_count, 
            ((msg_count - msg_received_count) / msg_count) * 100.0, 
            total_msec);
}

int main(int argc, char *argv[]) {
    int sockfd;
    char *ip_addr, *reverse_hostname;
    struct sockaddr_in addr_con;
    struct hostent *host_entity;
    char *ip_list[] = {"8.8.8.8", "1.1.1.1", "208.67.222.222"}; // Google DNS, Cloudflare DNS, OpenDNS
    int num_ips = sizeof(ip_list) / sizeof(ip_list[0]);

    for (int i = 0; i < num_ips; i++) {
        ip_addr = ip_list[i];
        
        sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (sockfd < 0) {
            printf("\nSocket file descriptor not received!!\n");
            return 0;
        }

        memset(&addr_con, 0, sizeof(addr_con));
        addr_con.sin_family = AF_INET;
        addr_con.sin_addr.s_addr = inet_addr(ip_addr);

        host_entity = gethostbyname(ip_addr);
        if (host_entity == NULL) {
            printf("\nDNS lookup failed! Could not resolve hostname!\n");
            return 0;
        }

        reverse_hostname = host_entity->h_name;
        printf("\nTesting connectivity to %s (%s)\n", ip_addr, reverse_hostname);

        send_ping(sockfd, &addr_con, reverse_hostname);
        
        close(sockfd);
    }

    return 0;
}

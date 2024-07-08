// // client.c

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>

// #define PORT 8888
// #define BUFFER_SIZE 1024

// int main() {
//     int sockfd;
//     struct sockaddr_in server_addr;
//     char buffer[BUFFER_SIZE];
//     char *hello = "Request time";

//     // Create UDP socket
//     if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     memset(&server_addr, 0, sizeof(server_addr));

//     // Configure server address
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(PORT);
//     server_addr.sin_addr.s_addr = INADDR_ANY;

//     int n;
//     socklen_t len;

//     // Send time request to server
//     sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, 
//            (const struct sockaddr *)&server_addr, sizeof(server_addr));
//     printf("Time request sent to server.\n");

//     // Receive time from server
//     n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, 
//                  (struct sockaddr *)&server_addr, &len);
//     buffer[n] = '\0';
//     printf("Server's time: %s\n", buffer);

//     close(sockfd);
//     return 0;
// }


#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<unistd.h>

int sock_fd=0;
struct sockaddr_in server_addr;

void main(){
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(2000);
    server_addr.sin_family = AF_INET;
    
    printf("Connected to server\n");
    
    while(1){
        char r_buff[100]="";
        char s_buff[100]="";
        
        printf("Input: ");
        fgets(s_buff, sizeof(s_buff), stdin);
        s_buff[strcspn(s_buff, "\n")] = 0;  // Remove newline
        printf("%s sbuff", s_buff);
        
        sendto(sock_fd, s_buff, strlen(s_buff), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        socklen_t n;
        int bytes_received = recvfrom(sock_fd, r_buff, sizeof(r_buff) - 1, 0,(struct sockaddr *)&server_addr, &n );
        if (bytes_received > 0) {
            r_buff[bytes_received] = '\0';
            printf("Received: %s\n", r_buff);
        }
        
        if (strcmp(s_buff, "end") == 0) {
            break;
        }
    }
    
    close(sock_fd);
    printf("Connection closed\n");
}

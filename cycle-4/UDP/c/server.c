
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<unistd.h>

int sock_fd=0;
struct sockaddr_in server_addr, client_addr;


void main(){
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(2000);
    server_addr.sin_family = AF_INET;
    
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sock_fd);
        return;
    }

    
    printf("Client connected\n");
    
    while(1){
        char r_buff[100]="";
        char s_buff[100]="";

        socklen_t n, client_addr_size;
        client_addr_size=sizeof(client_addr);
        int bytes_received = recvfrom(sock_fd, r_buff, sizeof(r_buff) - 1, 0,(struct sockaddr *)&client_addr, &client_addr_size );
        if (bytes_received > 0) {
            r_buff[bytes_received] = '\0';
            printf("Received: %s\n", r_buff);
            
            printf("Reply: ");
            fgets(s_buff, sizeof(s_buff), stdin);
            s_buff[strcspn(s_buff, "\n")] = 0;  // Remove newline
            
            sendto(sock_fd, s_buff, strlen(s_buff), 0, (struct sockaddr *)&client_addr, client_addr_size);
        }
        
        if (strcmp(r_buff, "end") == 0) {
            break;
        }
    }
    
    close(sock_fd);
    printf("Server stopped\n");
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct
{
    uint32_t ip_to_send;
    uint8_t zero1;
    uint16_t socket_to_send;
    uint8_t zero2;
} message;

void error(char * msg){
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    message msg;
    memset(&msg, 0, sizeof(msg));
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket\n");
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    portno = 1025;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Error on binding");
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd < 0){
        error("Error on accept\n");
    }
    
    n = recv(newsockfd, &msg, sizeof(msg), 0);
    if(n < 0){
        error("Error reading from socket\n");
    }

    uint32_t ip_received = ntohl(msg.ip_to_send);
    uint16_t sock_received = ntohs(msg.socket_to_send);

    char ip[16];
    uint8_t octet1 = (ip_received >> 24) & 0xFF;
    uint8_t octet2 = (ip_received >> 16) & 0xFF;
    uint8_t octet3 = (ip_received >> 8) & 0xFF;
    uint8_t octet4 = ip_received & 0xFF;

    // Format the string in dotted decimal notation
    sprintf(ip, "%u.%u.%u.%u", octet1, octet2, octet3, octet4);

    printf("uint32 is %u;", ip_received);
    printf(" IP is: %s\n", ip);
    printf("uint16 is %u;", sock_received);
    printf(" Port is %d\n", (int)sock_received);

    close(newsockfd);
    close(sockfd);
}

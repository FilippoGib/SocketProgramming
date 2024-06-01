
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>

typedef struct
{
    uint32_t ip_to_send;
    uint8_t zero1;
    uint16_t socket_to_send;
    uint8_t zero2;
} message;


void error(char * msg){
    perror(msg);
    exit(0);
}

int main(int argc, char*argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if(argc != 3){
        fprintf(stderr, "mi devi dare ip e porta\n");
        exit(0);
    }
    portno = 1025;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Errore in apertura socket\n");
    }
    server = gethostbyname("loopback");
    if(server == NULL){
        fprintf(stderr, "No such host\n");
        exit(0);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd ,(const struct sockaddr *) &serv_addr,sizeof(serv_addr))<0){
        error("Error connecting\n");
    }

    const char * ip = argv[1];
    unsigned int a,b,c,d;
    if(sscanf(ip,"%u.%u.%u.%u",&a,&b,&c,&d)!= 4)
    {
        error("errore nello spacchettamento dell'ip in 4\n");
    }
    uint32_t ip_converted = (a<<24) | (b << 16) | (c<<8) | d;

    uint16_t portno_converted = (uint16_t) atoi(argv[2]);

    message msg;
    msg.ip_to_send = htonl(ip_converted);
    msg.zero1 = 0;
    msg.socket_to_send  = htons(portno_converted);
    msg.zero2 = 0;

    n = send(sockfd, &msg, sizeof(msg), 0);
    if(n < 0)
        error("error writing to socket \n");

    fprintf(stdout, "IP is %s;", ip);
    fprintf(stdout, " uint32 is %u\n", ip_converted);

    fprintf(stdout, "Port is %d;", atoi(argv[2]));
    fprintf(stdout, " uint16 is %u\n", portno_converted);

    close(sockfd);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>

void error(char * msg){
    perror(msg);
    exit(0);
}

int main(int argc, char*argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    if(argc != 2){
        fprintf(stderr, "mi devi dare il nome del server\n");
        exit(0);
    }
    portno = 2525;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Errore in apertura socket\n");
    }
    server = gethostbyname(argv[1]);
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
        exit(0);
    }

    n = read(sockfd , buffer, 256);
    if(n < 0){
        error("errore in lettura dalla socket\n");
    }
    printf("%s\n",buffer);
    return 0;
}
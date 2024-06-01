/*a simple server in the internet domain using TCP, the port number is passed as an argument*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>

void error(char * msg){
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if(argc!=2){
        fprintf(stderr, "Error, provide port number\n");
        exit(1);
    }

    sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket\n");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
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
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if(n <0 ){
        error("Error reading from socket\n");
    }
    else {
        printf("Here is the message: %s\n", buffer);
    }
    n = write(newsockfd, "I got your message ;)\n",22);
    if(n < 0){
        error("Error on writing socket\n");
    }
    return 0;
}
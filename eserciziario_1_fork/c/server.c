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
    if(argc!=1){
        fprintf(stderr, "I need no parameters\n");
        exit(1);
    }

    sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket\n");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 2525;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Error on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while(1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        int pid = fork();

        if(pid == 0)
        {
            if(newsockfd < 0)
            {
                error("Error on accept\n");
            }

            bzero(buffer, 256);
            strcpy(buffer, "Il mio nome è: ");

            char name[256];
            bzero(name, 256);

            gethostname(name, 256);

            strcat(buffer,name);

            n = write(newsockfd, buffer, sizeof(buffer));
            // fprintf(stdout, "%s\n", buffer);

            if(n < 0)
            {
                error("Error on writing socket\n");
            }
        }
        close(newsockfd);
    }
}
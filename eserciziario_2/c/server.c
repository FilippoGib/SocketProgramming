#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CONN 10

void error(char * msg)
{
    perror(msg);
    exit(1);
}

int main (int argc, char **argv)
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    int n;
    clilen = sizeof(cli_addr);

    if(argc != 1)
    {
        fprintf(stderr, "ERRORE, questo programma non richiede parametri\n");
        exit(2);
    }

    sockfd = socket(AF_INET, SOCK_STREAM,0);

    if(sockfd < 0)
    {
        error("Creazione socket non è andata a buon fine\n");
        exit(3);
    }

    bzero((char*) &serv_addr,sizeof(serv_addr)); //azzero l'area di memoria dove si trova serv_addr, per sicurezza, prima di utilizzarlo
    portno = 2525; //normalmente è un parametro ma sto giro la hardcodiamo
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd,
           (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0)
    {
        error("Binding non è andato a buon fine\n");
        exit(4);
    }

    if(listen(sockfd,5)) //5 è un numero magico)
    {
        error("Listen non è andato a buon fine\n");
        exit(4);
    }

    while(1)
    {
        newsockfd = accept(sockfd,
                        (struct sockaddr *) &cli_addr,
                        &clilen);
        
        if(newsockfd < 0)
        {
            error("Accept non è andato a buon fine\n");
            exit(5);
        }

        int pid = fork();
        if (pid == 0) //siamo nel child
        {
            close(sockfd); //il child non ha bisogno della socket su cui si listen
            bzero(buffer, 256);
            n = read(newsockfd, buffer ,sizeof(buffer));
            fprintf(stdout,"il nome del client è: %s\n",buffer);
            close(newsockfd);
            exit(0);
        }
        else if(pid > 0)
        {
            close(newsockfd); //il parent non ha bisogno di questa socket
            continue;
        }
        else
        {
            error("Fork non è andata a buon fine\n");
            exit(5);
        }
    }
    return 0;
}
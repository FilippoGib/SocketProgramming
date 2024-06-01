#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    listen(sockfd,5); //5 è un numero magico

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd,
                      (struct sockaddr *) &cli_addr,
                      &clilen);
    
    if(newsockfd < 0)
    {
        error("Accept non è andato a buon fine\n");
        exit(5);
    }

    bzero(buffer, 256);
    strcpy(buffer, "Il mio nome è: ");
    gethostname(buffer + 16, sizeof(buffer) - 16);

    n = write(newsockfd, buffer ,sizeof(buffer));

    if(n < 0)
    {
        error("Write non è andata a buon fine\n");
        exit(6);
    }
    
    return 0;
}

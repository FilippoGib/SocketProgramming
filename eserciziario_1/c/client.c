#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char * msg)
{
    perror(msg);
    exit(1);
}

int main (int argc, char **argv)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    char buffer[256];

    if(argc != 2)
    {
        fprintf(stderr, "Devi specificare il nome del server\n");
        exit(2);
    }

    portno = 2525; //hardcoded
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error("Apertura socket non è andata a buon fine\n");
        exit(3);
    }

    server = gethostbyname(argv[1]);
    if(server == NULL)
    {   
        error("Non esiste questo host\n");
        exit(4);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); //azzero l'area di memoria che contiene serv_addr per sicurezza
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd,
                (const struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0)
    {
        error("Connessione non è andata a buon fine\n");
        exit(5);
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, sizeof(buffer));
    if(n < 0)
    {   
        error("Lettura da socket non è andata a buon fine\n");
        exit(6);
    }
    printf("%s\n", buffer);
    return 0;
}
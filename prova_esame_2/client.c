#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

//standard
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
    char *ipStr;

    if(argc != 3)
    {
        fprintf(stderr, "Devi specificare il nome del server e l'ip che vuoi analizzare\n");
        exit(2);
    }

    portno = 1025; //hardcoded ma puoi prenderla da argv[]
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
    else
    {
        fprintf(stdout, "[INFO] connessione avvenuta con successo a %s\n", argv[1]);
    }
    bzero(buffer, 256);
    //fai quel che devi fare:
    n = write(sockfd, argv[2],strlen(argv[2]) + 1);
    if(n < 0)
    {
        error("Errore in scrittura del client\n");
        exit(6);
    }
    printf("Ho spedito l'indirizzo con successo\n");
    return 0;
}
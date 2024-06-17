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
        fprintf(stderr, "ERRORE, non necessito arguments\n");
        exit(2);
    }

    sockfd = socket(AF_INET, SOCK_STREAM,0);

    if(sockfd < 0)
    {
        error("Creazione socket non è andata a buon fine\n");
        exit(3);
    }

    bzero((char*) &serv_addr,sizeof(serv_addr)); //azzero l'area di memoria dove si trova serv_addr, per sicurezza, prima di utilizzarlo
    portno = 8080; //normalmente è un parametro ma sto giro la hardcodiamo
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

    listen(sockfd,5); //5 è la dimensione della coda

    clilen = sizeof(cli_addr);

    while(1) //si ferma solo con ctrlC o ctrlD
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

        if(pid == 0) //codice del figlio
        { 
            bzero(buffer, 256);
            int n = read(newsockfd, buffer, 255);
            if(n < 0)
            {
                error("errore in lettura dalla socket\n");
                exit(6);
            }
            int seed;
            int iterations;
            if(sscanf(buffer,"%d,%d\r\n",seed,iterations) != 2)
            {
                error("Formato dell'input errato\n");
                exit(7);
            }
            
            //dato che ho usato %d dovrei essere sicuro che siano degli int
            if((seed / 10) > 0)
            {
                error("Il seed deve essere una singola digit\n");
                exit(8);
            }

            for(int i = 0; i< iterations ; i++)
            {
                int seen[9] = {0,0,0,0,0,0,0,0,0,0,0}; //vettore dei numeri che ho già visto
                char * string = "";
                if(strlen(string) == 0)
                {
                    
                }
                for(int j = 0; j < 10; j++)
                {
                    printf(":()");
                }
            }
            
            return 0;
        }
        close(newsockfd);
    }
}

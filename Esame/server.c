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

unsigned int ip_to_int(unsigned char* ip)
{
    unsigned int a,b,c,d;
    if(sscanf(ip,"%u.%u.%u.%u",&a,&b,&c,&d) != 4) return 0;
    return (a << 24) | (b << 16) | (c << 8) | d;
}

int main (int argc, char **argv)
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    int n;

    if(argc != 1)
    {
        fprintf(stderr, "ERRORE, this program does not require parameters from command line\n");
        exit(2);
    }

    sockfd = socket(AF_INET, SOCK_STREAM,0);

    if(sockfd < 0)
    {
        error("Creazione socket non è andata a buon fine\n");
        exit(3);
    }

    bzero((char*) &serv_addr,sizeof(serv_addr)); //azzero l'area di memoria dove si trova serv_addr, per sicurezza, prima di utilizzarlo
    portno = 1025; //normalmente è un parametro ma sto giro la hardcodiamo
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
            //non devi validare l'input
            bzero(buffer,256);
            n = read(newsockfd, buffer,255);
            if(n < 0)
            {
                error("Error while reading from socket\n");
                exit(6);
            }
            unsigned char q = buffer[0]; //deve essere sempre così
            printf("[Debug 1]: q = %u\n",q);
            unsigned char len = buffer[1]; //è il secondo byte di 
            printf("[Debug 2]: len = %u\n",len);

            unsigned char * IpStr = calloc(len,sizeof(unsigned char)); //alloco una stringa di lunghezza len

            // for(unsigned char i = 2; i <= len + 2 ; i++) //<= perchè voglio scirverci anche lo zero terminatore
            // {
            //     unsigned char k = 0;
            //     IpStr[k] = buffer[i];
            //     k+=1;
            // }
            IpStr = buffer + 2;
            printf("[Debug 3]: IpStr = %s\n",IpStr);
            unsigned int IpBinary = ip_to_int(IpStr);

            printf("[Debug 4]: IpBinary = %u\n",IpBinary);


            unsigned char * response = calloc(6,sizeof(unsigned char));
            response[0] = 'R';
            response[1] = 4;
            unsigned int IpBinary_netFormat = htonl(IpBinary);
            response[5] = IpBinary_netFormat >> 24;
            response[4] = (IpBinary_netFormat >> 16) & 0xff;
            response[3] = (IpBinary_netFormat >> 8) & 0xff;
            response[2] = (IpBinary_netFormat) & 0xff;

            n = write(newsockfd, response ,6);
            if(n < 0)
            {
                error("Error while writing to socket\n");
                exit(7);
            }

            printf("[Debug 5]:Write successful\n");

            free(response);
            free(IpStr);
            return 0;
        }
        close(newsockfd);
    }
}

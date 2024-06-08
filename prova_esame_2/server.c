#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

void error(char * msg)
{
    perror(msg);
    exit(1);
}

char * classFinder(unsigned int ip )
{
    unsigned int firstByte = ip >> 24;
    if(firstByte >=0 && firstByte <= 127)
    {   
        return "A";
    }
    if(firstByte >=128 && firstByte <= 191)
    {   
        return "B";
    }
    if(firstByte >=192 && firstByte <= 223)
    {   
        return "C";
    }
    if(firstByte >=224 && firstByte <= 239)
    {   
        return "D";
    }
    if(firstByte >=240 && firstByte <= 254)
    {   
        return "E";
    }
    else
    {
        return "ERR";
    }
}

unsigned int ip_to_int(char* ip) //funzione che ritorna un ip binario dato un ip string
{
    unsigned int a,b,c,d;
    if(sscanf(ip,"%u.%u.%u.%u",&a,&b,&c,&d) != 4) return 0;
    return (a << 24) | (b << 16) | (c << 8) | d;
}

char* uintToIp(unsigned int ip) //funzione che ritorna un ip string dato un ip binario
{
    // Allocate memory for the IP address string
    // 16 bytes is enough to store "xxx.xxx.xxx.xxx\0"
    char* ipString = (char*)malloc(16 * sizeof(char));
    if (ipString == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Convert the unsigned int to an IP address string
    snprintf(ipString, 16, "%u.%u.%u.%u",
             (ip >> 24) & 0xFF,
             (ip >> 16) & 0xFF,
             (ip >> 8) & 0xFF,
             ip & 0xFF);

    return ipString;
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
            char ipStr[256];
            bzero(ipStr,256);
            int n = read(newsockfd,ipStr,255);
            if(n < 0)
            {
                error("Errore in lettura dalla socket\n");
                exit(6);
            }
            unsigned int ipBin = ip_to_int(ipStr);
            if(ipBin == 0)
            {
                error("ERROR\n");
                exit(6);
            }
            char * class = classFinder(ipBin); //classe dell'ip
            char * netmask = "";
            char * netIDStr = "";
            bool abc = false;

            if(strcmp(class, "ERR") == 0)
            {
                error("ERROR\n");
                exit(7);
            }
            //trovare la netmask
            if(strcmp(class, "A") == 0)
            {
                netmask = "255.0.0.0";
                abc = true;
            }
            if(strcmp(class, "B") == 0)
            {
                netmask = "255.255.0.0";
                abc = true;
            }
            if(strcmp(class, "C") == 0)
            {
                netmask = "255.255.255.0";
                abc = true;
            }
            if(strcmp(class, "D") == 0)
            {
                netmask = "";
            }
            if(strcmp(class, "E") == 0)
            {
                netmask = "";
            }

            if(abc)
            {
                unsigned int netmaskBi = ip_to_int(netmask);
                unsigned int netID = ipBin & netmaskBi;
                netIDStr = uintToIp(netID);
            }

            fprintf(stdout, "%s %s %s\n",class, netIDStr, netmask);

            return 0;
        }
        close(newsockfd);
    }
}

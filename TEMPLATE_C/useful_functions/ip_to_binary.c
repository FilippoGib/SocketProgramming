#include <stdio.h>
#include <stdlib.h>

unsigned int ip_to_int(char* ip)
{
    unsigned int a,b,c,d;
    if(sscanf(ip,"%u.%u.%u.%u",&a,&b,&c,&d) != 4) return 0;
    return (a << 24) | (b << 16) | (c << 8) | d;
}

void print_binary(unsigned int value)
{
    for(int i = 31; i >= 0; i--)
    {
        unsigned int mask = 1 << i;
        putchar((value & mask) ? '1' : '0');
    }
    putchar('\n');
}

int main()
{
    char * ipString = "10.10.1.45";

    unsigned int ipBinary = ip_to_int(ipString);
    if(ipBinary == 0)
    {
        printf("Errore nella conversione dell'indirizzo");
        return EXIT_FAILURE;
    }
    printf("Indirizzo binario: ");
    print_binary(ipBinary);
    return EXIT_SUCCESS;
}
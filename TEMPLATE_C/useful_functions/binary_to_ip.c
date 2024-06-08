#include <stdio.h>
#include <stdlib.h>

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
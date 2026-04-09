#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdbool.h>

#define DISPLAY_SIZE 16

extern bool debug;
extern bool showAscii;
extern bool showAddress;

void dbgPrint(char string[])
{
    if(debug)
        printf("[DBG] %s\n", string);
}

void addressPrint(long address, bool nl)
{
    if(showAddress)
        if(nl)
            printf("%08lx: \n", address);
        else if(!nl)
            printf("%08lx: ", address);
}

int hexToAscii(size_t bytesRead, unsigned char buffer[DISPLAY_SIZE])
{
    for(size_t i = 0; i < bytesRead; i++)
    {
        if(buffer[i] >= 32 && buffer[i] <= 126)
            printf("%c", buffer[i]);
        else
            printf(".");
    }

    return 0;
}

#endif

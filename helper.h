#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>

#define DISPLAY_SIZE 16

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

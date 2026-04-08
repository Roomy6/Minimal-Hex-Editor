#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helper.h"

char **lines = NULL;
int count = 0;

void readFile(FILE *file)
{
    /* buffer is the amount of data displayes per address */
    unsigned char buffer[DISPLAY_SIZE];
    size_t bytesRead;
    long address = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        /* Print the address */
        printf("%08lx: ", address);

        /* Print the hex */
        for (size_t i = 0; i < bytesRead; i++)
        {
            printf("%02x ", buffer[i]);
        }
        
        /* Space padding */
        for (size_t i = bytesRead; i < 16; i++)
        {
            printf("   "); // 3 spaces per missing byte
        }       
       
        printf(" |  ");
        
        /* Print ACSII representation */
        hexToAscii(bytesRead, buffer);
        printf("\n");
        
        address += bytesRead;
    }
}

void writeFile(FILE *file)
{
    int c;                          // keyboard input
    int high = -1;                  // high nibble
    long address = ftell(file);     // position in file stream

    fseek(file, 0, SEEK_END);       // mov file stream to EOF (i think)

    printf("%08lx: ", address);     // print address position
    fflush(stdout);                 // flush the output buffer
    
/*
    for(long i = 0; i < size; i++)
    {
        fputc(0x00, file);
    }
*/

    while((c = getchar()) != EOF)
    {

        /* check if user inputs 'q' for quit */
        /* Make sure this is cehcked first */
        if (c == 0x71 || c == 0x51)
        {
//            fclose(file);
            return;
        }

        /* ignore whitespace */
        if(c == ' ' || c == '\n' || c == '\t')
            continue;
       
        /* do some funky stuff with hex idk */
        int value;

        if (c >= '0' && c <= '9') value = c - '0';
        else if (c >= 'A' && c <= 'F') value = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') value = c - 'a' + 10;
        else continue;

        if (value == -1) {
            fprintf(stderr, "\nInvalid hex character: %c\n", c);
            continue;
        }

        if(high == -1) { high = value; }
        else
        {
            unsigned char byte = (high << 4) | value;
            fputc(byte, file);

            address++;
            printf("\r%08lx: ", address);

            high = -1;

            fflush(stdout);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "rb+");
    
    if(argc < 3)
    {
        printf("Invalid arguments.\n");
        /* im proud of the arg[0] thing :) */
        printf("%s [file] [r/w]\n", argv[0]);
        return 1;
    }

//    printf("Arguments: %d\n", argc);

    if(file == NULL)
    {
        if(argv[2][0] == 'w')
        {
            printf("Creating file %s\n", argv[1]);
            file = fopen(argv[1], "wb+");
            //        perror("fopen");
        }

        if(file == NULL)
        {
            printf("No such file or directory\n");
            return 1;
        }
    }

    /* convert string to long */
//    long size = atoll(argv[2]);
    
    if(argv[2][0] == 'r')
        readFile(file);
    else if(argv[2][0] == 'w')
        writeFile(file);
    else
    {
        printf("Invalid arguments.\n");
        return 1;
    }
    
    fclose(file);
    return 0;
}


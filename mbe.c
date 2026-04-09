/*
 * Minimal Binery Editor
 *
 * This is a simple and minimal program that
 * allows users to read and write any file
 * as a Hex representation
 *
 * Written by Roomy.
 * https://github.com/Roomy6/Minimal-Hex-Editor/
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helper.h"

#define RELEASE "DEV"
#define VERSION "0.0.2"
#define NAME "MBE"

/* idk why this does not work when i put it in helper.h */
bool debug = false;
bool showAscii = true;
bool showAddress = true;

void readFile(FILE *file)
{
    /* buffer is the amount of data displayes per address */
    unsigned char buffer[DISPLAY_SIZE];
    size_t bytesRead;
    long address = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        /* Print the address */
        addressPrint(address, false);

        /* Print the hex */
        for (size_t i = 0; i < bytesRead; i++)
        {
            printf("%02x ", buffer[i]);
        }
        
        if(showAscii)
        {
            /* Space padding */
            for (size_t i = bytesRead; i < 16; i++)
            {
                printf("   "); // 3 spaces per missing byte
            }       

            printf(" |");

            /* Print ACSII representation */
            hexToAscii(bytesRead, buffer);
            printf("|\n");
        }
        /* I have no idea if this is a smart way of doing this */
        else if(!showAscii && showAddress)
            printf("\n");
        else if(!showAscii && !showAddress)
            printf("\n");
        
        address += bytesRead;
    }
}

void writeFile(FILE *file)
{
    int c;                              // keyboard input
    int high = -1;                      // high nibble

    /* define address after fseek */
    fseek(file, 0, SEEK_END);           // mov file stream to EOF (i think)
    long address = ftell(file);         // position in file stream

    addressPrint(address, false);
    fflush(stdout);                     // flush the output buffer
    
    while((c = getchar()) != EOF)
    {
        /* check if user inputs 'q' for quit */
        /* Make sure this is cehcked first */
        if (c == 'q' || c == 'Q')
        {
            return;
        }

        else if(c == '*')
        {
            long addressB = 0;
            unsigned int value;

            if(scanf("%lx %x", &addressB, &value) == 2)
            {
                /* Default to 00 if no input for hex */ 
                if(value == 0)
                    value = 0x00;

                // *00000100 02
                if(debug)
                    printf("[DBG] Address %08lx - %08lx set to %02x\n", address, addressB, value);
                /* Check if current address is smaller that addressB */
                if(address < addressB)
                {
                    for(address = address; address < addressB; address++)
                    {
                        if(debug)
                            printf("[DGB] Set %08lx to %02x\n", address, value);
                        fputc(value, file);
                    }
                }
                else
                    errPrint("Cannot set address.");
            }
            /* Print the address to prevent the weird gap */
            addressPrint(address, false);
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
            if(showAscii)
                printf("\r%08lx: ", address);

            high = -1;

            fflush(stdout);
        }
    }
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    char mode = 0;

    if(debug)
        printf("[DBG] Arguments: %d\n", argc);

    for(int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            printf("MBE - Minimal Binary Editor\n");
            printf("Version %s-%s\n\n", RELEASE, VERSION);

            printf("Additional arguments:\n");
            printf("-h      Displays help page\n");
            printf("-v      Show program version\n");
            printf("-d      Enable debug output\n");
            printf("-ras    Remove ASCII representation\n");
            printf("-rad    Remove Address position\n");

            printf("\n");

            printf("While writing a binary, you can use '*' to jump fill a memory region.\n");
            printf("Eg: *[address to] [hex]\n");

            return 0;
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            printf("%s - version %s-%s\n", NAME, RELEASE, VERSION);
            return 0;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            debug = true;
        }
        else if (strcmp(argv[i], "-ras") == 0)
        {
            showAscii = false;
        }
        else if (strcmp(argv[i], "-rad") == 0)
        {
            showAddress = false;
        }

        /* positional args */
        else if (filename == NULL)
        {
            filename = argv[i];
        }
        else if (mode == 0)
        {
            if(strlen(argv[i]) != 1 || (argv[i][0] != 'r' && argv[i][0] != 'w'))
            {
                printf("Invalid mode: %s\n", argv[i]);
                printf("Use 'r' or 'w'\n");
                return 1;
            }
            mode = argv[i][0];  // 'r' or 'w'
        }
        else
        {
            printf("Unknown argument: %s\n", argv[i]);
            printf("Usage: %s [file] [r/w]\n\n", argv[0]);
            printf("Use -h for more infomration\n");

            return 1;
        }
    }

    /* Verify required arguments */
    /* Also check if argc is less than 3 to prevent seg fault */
    if (filename == NULL || mode == 0 || argc < 3)
    {
        printf("Invalid arguments.\n");
        printf("Usage: %s [file] [r/w]\n\n", argv[0]);
        printf("Use -h for more information\n");
        return 1;
    }

    if (debug)
        printf("[DBG] file=%s, mode=%c\n", filename, mode);

    FILE *file = fopen(filename, "rb+");

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
    
    if(mode == 'r')
        readFile(file);
    else if(mode  == 'w')
        writeFile(file);
    else
    {
        /* Backup else just incase it somehow bypasses xD */
        errPrint("Invalid arguments, please use -h for more info.");
    }
 
    fclose(file);
    return 0;
}


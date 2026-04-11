#ifndef HELPER_H
#define HELPER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define VERSION "v0.1.0-rc1"
#define NAME "MBE"

#define DISPLAY_SIZE 16

extern bool debug;
extern bool showAscii;
extern bool showAddress;
extern bool verbose;

void errPrint(char string[])
{
    printf("[ERR] %s\n", string);
}

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

int hexValue(int c)
{
    if(isdigit(c)) return c - '0';
    if(isxdigit(c)) return tolower(c) - 'a' + 10;

    return -1;
}

void printLine(unsigned char *buffer, size_t len, long address)
{
    if(showAddress)
        addressPrint(address, false);
    
    for(size_t i = 0; i < len; i++)
    {
        printf("%02x ", buffer[i]);
    }
    
    if(showAscii)
    {
        /* Use DISPLAY_SIZE instead of hardocding :) */
        for(size_t i = len; i < DISPLAY_SIZE; i++)
            printf("   ");

        printf(" |");
        hexToAscii(len, buffer);
        printf("|");
    }

    printf("\n");
}

void displayVersion()
{
    printf("%s %s\n", NAME, VERSION);
}

void displayHelp()
{
    displayVersion();
    printf("\n");
    printf("Usage: %s [file] [r/w]\n\n", argv[0]);
    printf("Additional arguments:\n");
    printf("-h      Displays help page\n");
    printf("-v      Show program version\n");
    printf("-d      Enable debug output\n");
    printf("-ras    Remove ASCII representation\n");
    printf("-rad    Remove Address position\n");
    printf("-ve     Enable verbose output\n");

    printf("\n");

    printf("While writing a binary, you can use '*' to jump fill a memory region.\n");
    printf("Eg: *[address to] [hex]\n");
    printf("You can move your cursor position by using '>'.\n");
    printf("Eg: >[address]\n");
}

void handleFlag(const char *arg)
{
    if      (!strcmp(arg, "-h")) {  displayHelp(); exit(0); }       /* Help */
    else if (!strcmp(arg, "-v")) {  displayVersion(); exit(0); }    /* Version */
    else if (!strcmp(arg, "-d"))    debug = true;                   /* Debug */
    else if (!strcmp(arg, "-ras"))  showAscii = false;              /* Remove ASCII */
    else if (!strcmp(arg, "-rad"))  showAddress = false;            /* Remove Address */
    else if (!strcmp(arg, "-ve"))   verbose = true;                 /* Full verbose output */
}

FILE *openFile(const char *name, char mode)
{
    FILE *f = fopen(name, mode == 'r' ? "rb" : "rb+");

    if(!f && mode == 'w')
        f = fopen(name, "wb+");

    return f;
}

#endif

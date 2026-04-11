/*
 * Minimal Binary Editor v0.1.0
 *
 * This is a simple and minimal program that
 * allows users to read and write any file
 * as a Hex representation
 *
 * Written by Roomy.
 * https://github.com/Roomy6/Minimal-Hex-Editor/
 */

#include "helper.h"

bool debug          = false;
bool showAscii      = true;
bool showAddress    = true;
bool verbose        = false;

void readFile(FILE *file)
{
    /* buffer is the amount of data displayes per address */
    unsigned char buffer[DISPLAY_SIZE];
    unsigned char prevBuffer[DISPLAY_SIZE];

    size_t bytesRead, prevBytesRead = 0;

    long address = 0;

    int isDuplicate = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        /* dont know why i need to invery verbose check */
        if(!verbose && bytesRead == prevBytesRead && 
                memcmp(buffer, prevBuffer, bytesRead) == 0)
        {
            if(!isDuplicate)
            {
                printf("*\n");
                isDuplicate = 1;
            }
        }
        else
        {
            printLine(buffer, bytesRead, address);
            isDuplicate = 0;

            memcpy(prevBuffer, buffer, bytesRead);
            prevBytesRead = bytesRead;
        }
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
        if(c == 'q' || c == 'Q')
            return;

        else if(c == '*')
        {
            long addressB = 0;
            unsigned int value = 0;
            char line[128];
            int input;

            if(fgets(line, sizeof(line), stdin))
            {
                input = sscanf(line, "%lx %x", &addressB, &value);
                
                /* Make a check if the hex value has been provided or not */
                if(input >= 1)
                {
                    /* No hex provided, default to 00 */
                    if(input == 1)
                        value = 0x00;

                    // *00000100 02
                    if(debug)
                        printf("[DBG] Address %08lx - %08lx set to %02x\n", address, addressB, value);
                    /* Check if current address is smaller that addressB */
                    if(address < addressB)
                    {
                        /* +1 to fully fill to entered address */
                        addressB += 1;
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
            }

            /* Print the address to prevent the weird gap */
            addressPrint(address, false);
        }
        
        /* Point cursor command */
        else if(c == '>')
        {
            unsigned char buffer[DISPLAY_SIZE];
            size_t bytesRead, prevBytesRead = 0;
            long addressB = 0;
            char line[128];
            char *ptr;
            int input;
            
            /* Get user address input */
            if(fgets(line, sizeof(line), stdin))
            {
                input = sscanf(line, "%lx", &addressB);
                if(input == 1)
                {
                    if(debug)
                        printf("[DBG] Pointing to address %08lx\n", addressB);
                    
                    /* Move cursor to address */
                    if (fseek(file, addressB, SEEK_SET) != 0)
                    {
                        errPrint("Invalid address.");
                    }

                    address = addressB;

                    addressPrint(address, false);
                    bytesRead = fread(buffer, 1, DISPLAY_SIZE, file);

                    /* This fixed the +16 address bug */
                    fseek(file, address, SEEK_SET);
                }
                else
                    errPrint("Could not move cursor to address.");
            }
        }

        /* ignore whitespace */
        if(isspace(c))
            continue;

        int value = hexValue(c);

        if(value < 0) continue;

        if(value == -1) {
            fprintf(stderr, "\nInvalid hex character: %c\n", c);
            continue;
        }

        if(high < 0)
        {
            high = value;
            continue;
        }

        unsigned char byte = (high << 4) | value;
        fputc(byte, file);
        address++;

        if(showAscii)
            printf("\r%08lx: ", address);

        high = -1;
    }
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    char mode = 0;

    for(int i = 1; i < argc; i++)
    {

        if(argv[i][0] == '-')
        {
            handleFlag(argv[i], argv);
            continue;
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

    /* Open the file using the helper */
    FILE *file = openFile(filename, mode);

    if(!file)
    {
        /* error out if any ocur */
        perror("");
        return 1;
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


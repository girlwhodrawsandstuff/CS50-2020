#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAXBLOCK 512

int main(int argc, char *argv[])
{
    // Check validity of input
    if (argc != 2 && argv[1] == NULL)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");

    if (inputFile == NULL)
    {
        printf("File not found error\n");
    }

    // filename takes up 8 bits as "000.jpg" is 8 bits
    char filename[8];

    FILE *outputFile = NULL;
    uint8_t buffer[MAXBLOCK];
    int count = 0;

    // Open memory card
    // Repeat until end of card
    // Read 512 bytes into a buffer
    while (fread(buffer, sizeof(uint8_t), MAXBLOCK, inputFile) || feof(inputFile) == 0)
    {

        // If start of new jpeg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If file is open
            if (outputFile != NULL)
            {
                fclose(outputFile);
            }

            // Initializing new file
            sprintf(filename, "%03i.jpg", count);
            outputFile = fopen(filename, "w");
            count++;

        }

        if (outputFile != NULL)
        {
            //Writing to file
            fwrite(buffer, sizeof(buffer), 1, outputFile);
        }
    }

    // Close all open files
    if (inputFile != NULL)
    {
        fclose(inputFile);
    }
    if (outputFile != NULL)
    {
        fclose(outputFile);
    }
    return 0;
}

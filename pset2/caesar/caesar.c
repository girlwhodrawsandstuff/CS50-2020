// Reference formulae: ci = (pi + k) % 26

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int main(int argc, string argv[])
{

    // Checks for valid input, and validity of key
    if (argc == 2)
    {
        // Checking the validity of our key
        bool checkValidity = false;

        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isdigit(argv[1][i]))
            {
                checkValidity = true;
            }
            else
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

    }
    else if (argc != 2 || isalpha(*argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Index 1 of argv needs to be converted to an int from a string
    int key = atoi(argv[1]);

    char plainText[100];
    printf("plaintext: ");
    fgets(plainText, sizeof(plainText), stdin);
    
    printf("ciphertext: ");
    for (int i = 0; i < strlen(plainText) - 1; i++)
    {
        // for lowercase letters:
        if (islower(plainText[i]))
        {
            plainText[i] = ((int)plainText[i] - 97 + key) % 26 + 97; // To stay within alphabet & lower case range
        }
        // for uppercase letters:
        else if (isupper(plainText[i]))
        {
            plainText[i] = ((int)plainText[i] - 65 + key) % 26 + 65; // To stay within alphabet & upper case range
        }


    }
    
    printf("%s", plainText);
    printf("\n");
}
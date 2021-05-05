#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    // Will be used later to check validity of user input
    bool checkValidity = false;

    if (argc == 2)
    {
        //checks for number of alphabets entered
        if (strlen(argv[1]) == 26)
        {
            for (int firstAlpha = 0; firstAlpha < strlen(argv[1]); firstAlpha++)
            {
                for (int secondAlpha = firstAlpha + 1; secondAlpha < strlen(argv[1]) + 1; secondAlpha++)
                {
                    // alphabet checker condition
                    if ((isalpha(argv[1][firstAlpha])))
                    {
                        checkValidity = true;
                        
                        // 1. Checking for duplicates:
                        bool checkRepeat[26];
                        string argvCopy = argv[1];
                        char numofAlpha[26];
                        
                        for (int i = 0; i < 26; i++)
                        {
                            checkRepeat[i] = false;
                        }
                        
                            
                        for (int i = 0; i < 26; i++)
                        {
                            // 1.a. Checking uppercase
                            if (isupper(argvCopy[i]))
                            {
                                if (checkRepeat[argvCopy[i] - 'A'] == true)
                                {
                                    printf("Repeated alphabets");
                                    return 1;
                                }
                                else
                                {
                                    checkRepeat[argvCopy[i] - 'A'] = true;
                                    numofAlpha[i] = argvCopy[i] - 'A';
                                }
                            }
                            else
                                // 1.b. Checking lowercase for repeats
                            {
                                if (checkRepeat[argvCopy[i] - 'a'] == true)
                                {
                                    printf("Repeated alphabets\n");
                                    return 1;
                                }
                                else
                                {
                                    checkRepeat[argvCopy[i] - 'a'] = true;
                                    numofAlpha[i] = argvCopy[i] - 'a';
                                }
                            }
                        }
                    }
                    else
                    {
                        printf("The plainText needs to be alphabets only\n");
                        return 1;
                    }

                }
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }

    }
    else
    {
        printf("Usage ./substitution key\n");
        return 1;
    }


    // Taking user plainText
    string plainText = get_string("plaintext: ");

    // Unlike caesar cipher, we do not %26 or add back 97 & 65
    for (int i = 0; i < strlen(plainText); i++)
    {
        // For lowercase alphabets
        if (islower(plainText[i]))
        {
            plainText[i] = tolower(argv[1][plainText[i] - 97]);
        }
        // For uppercase alphabets
        else if (isupper(plainText[i]))
        {
            plainText[i] = toupper(argv[1][plainText[i] - 65]);
        }
    }

    printf("ciphertext: ");
    printf("%s\n", plainText);

    // Ends program execution
    return 0;

}

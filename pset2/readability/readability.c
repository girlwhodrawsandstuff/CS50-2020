// Thanks to Coleman-Liau wiki page for the formulae

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Function declarations
long letterCount(char text[]);
long wordCount(char text[]);
long sentenceCount(char text[]);

int main(void)
{
    char text[1000];
    float index;

    // Prompts user input
    do
    {
        printf("Text: ");
        fgets(text, sizeof(text), stdin);
    }
    while (strlen(text) - 1 == 0);

    float numOfLetters = letterCount(text);
    float numOfWords = wordCount(text);
    float numOfSentences = sentenceCount(text);

    float L = numOfLetters / numOfWords * 100;
    float S = numOfSentences / numOfWords * 100;

    //Calculating grade
    index = 0.0588 * L - 0.296 * S - 15.8;

    if ((int) round(index) < 1)
    {
        printf("Before Grade 1\n");
    }
    else if ((int) round(index) >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", (int) round(index));
    }

}


// Counting Number of Letters
long letterCount(char text[])
{
    long count = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }

    return count++;
}

// Counting Number of Words
long wordCount(char text[])
{
    long count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            count++;
        }
    }

    return count++;
}

// Counting Number of Sentences
long sentenceCount(char text[])
{
    long count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            count++;
        }

    }

    return count;
}
// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root = NULL;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Set children's value
        node *children = root;

        // Loop trough every char in word
        for (int i = 0; word[i] != '\0'; i++)
        {
            // Hash the char for a position
            int cPos = (word[i] == '\'') ? N - 1 : word[i] - 'a';

            // Check whether the pointer to the char is null
            if (children->children[cPos] == NULL)
            {
                // Create a note to be pointed by the char position
                node *nextChildren = malloc(sizeof(node));
                if (nextChildren == NULL)
                {
                    unload();
                    return false;
                }

                nextChildren->is_word = false;
                for (int j = 0; j < N; j++)
                {
                    nextChildren->children[j] = NULL;
                }

                // Point the position to the new node
                children->children[cPos] = nextChildren;

                // If it is the first char in the word
                // Point the root to the new node
                if (i == 0)
                {
                    root->children[cPos] = nextChildren;
                }
            }

            // Pointer moves to the next node
            children = children->children[cPos];
        }

        // Set the end of the word to true
        children->is_word = true;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Recursive defaul value or
    // Dictionary not loaded
    if (!root)
    {
        return 0;
    }

    // Dictionary size
    int tSize = 0;

    // Root pointer track
    node *ptr = root;

    // Adds +1 to size if current node is a word
    if (ptr->is_word == true)
    {
        tSize++;
    }

    // Loop through current node children's and recursively call size()
    for (int i = 0; i < N; i++)
    {
        // Move *root to the next node
        root = ptr->children[i];

        // Add to tSize the size of the rest of the trie
        tSize += size();
    }

    // Reset root to the defaul value
    root = ptr;

    // Return current node and it's childrens size
    return tSize;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Return false if dictionary not loaded
    if (!root)
    {
        return false;
    }

    node *ptr = root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        char c = tolower(word[i]);

        int cPos = (c == '\'') ? N - 1 : c - 'a';

        if (!ptr->children[cPos])
        {
            return false;
        }

        ptr = ptr->children[cPos];
    }

    return ptr->is_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO

    if (!root)
    {
        return false;
    }

    node *ptr = root;

    for (int i = 0; i < N; i++)
    {
        root = ptr->children[i];

        unload();
    }

    root = ptr;

    free(ptr);

    return true;
}
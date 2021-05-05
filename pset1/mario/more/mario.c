#include <stdio.h>

int main(void)
{
    int input, row, column, indent;

    do
    {
        printf("Input a number from 1-8: \n");
        scanf("%d", &input);
    }
    while (input > 8 || input < 1);  // Parameters to check the input range

    for (row = 0; row < input; row++)
    {
        for (indent = 0; indent < (input - row - 1);
             indent++) //This ensures that the number of spaces is always one less than height of the pyramid - rows of pyramid
        {
            printf(" ");

        }

        printf("#");
        for (column = 0; column < row; column++)
        {
            printf("#");

        }

        printf("  "); // Prints the space between two pyramids

        // left aligned pyramid

        for (column = 0; column <= row; column++)
        {
            printf("#");

        }

        printf("\n"); // Moves to the next line after completing a row
    }

}

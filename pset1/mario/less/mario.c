#include <stdio.h>

int main(void)
{
    int input, row, column, indent;

    // Checking the range
    do
    {
        printf("Input a number from 1-8: \n");
        scanf("%d", &input);
    }
    while (input > 8 || input < 1);  

    for (row = 0; row < input; row++)
    {
        //This ensures that the number of spaces is always one less than height of the pyramid - rows of pyramid
        for (indent = 0; indent < (input - row - 1); indent++) 
        {
            printf(" ");

        }

        printf("#");
        for (column = 0; column < row; column++)
        {
            printf("#");

        }
        // Moves to the next line after completing a row
        printf("\n"); 
    }

}

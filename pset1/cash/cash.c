#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollar;
    int coins = 0;

    do
    {
        dollar = get_float("Enter the amount in dollars: \n");
    }
    while (dollar <= 0);

    int denom = round(dollar * 100); // Converts dollars to cents

    // quarters
    while (denom >= 25)
    {
        denom -= 25;
        coins++;
    }

    // dimes
    while (denom >= 10)
    {
        denom -= 10;
        coins++;
    }


    // nickels
    while (denom >= 5)
    {
        denom -= 5;
        coins++;
    }

    // pennies
    while (denom >= 1)
    {
        denom -= 1;
        coins++;
    }

    printf("Number of coins needed are: %i\n", coins);
}
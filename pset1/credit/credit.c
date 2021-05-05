#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

void validateCreditCard(long cardNumber);

int main(void)
{
    long cardNumber;

    // Get number
    cardNumber = get_long("Enter your credit card number: \n");
    validateCreditCard(cardNumber);

    return 0;
}

void validateCreditCard(long cardNumber)
{
    int firstDigit;
    int twoDigits;
    int totalSum = 0;
    int currentCount = 0;

    char cardNumberString[256];
    sprintf(cardNumberString, "%ld", cardNumber);
    int cardNumberLength = strlen(cardNumberString);

    if (cardNumberLength < 13 || cardNumberLength > 16)
    {
        printf("INVALID\n");
        return;
    }

    long originalNum = cardNumber;



    // Checksum
    long cardNumberCopy = cardNumber;

    // For the first alternate additions

    do
    {
        currentCount = cardNumberCopy % 10; // To get the last digit
        totalSum += currentCount; // Adds last digit to the total
        cardNumberCopy /= 100; // Iteration
    }
    while (cardNumberCopy != 0);

    // For the second alternate additions

    int multiplyNum = 0;

    cardNumberCopy = cardNumber / 10;
    do
    {
        currentCount = cardNumberCopy % 10;
        multiplyNum = currentCount * 2; //According to Luhn's algorithm, we have to double the number
        totalSum = totalSum + (multiplyNum % 10) + (multiplyNum / 10);
        cardNumberCopy /= 100;

    }
    while (cardNumberCopy != 0);


    // Checking validity
    bool isValid = totalSum % 10 == 0;


    // All American Express numbers start with 34 or 37;
    // most MasterCard numbers start with 51, 52, 53, 54, or 55
    // And all Visa numbers start with 4.

    //  To check this criteria:
    int countDigit = 0;
    do
    {
        cardNumber /= 10;
        countDigit++;
    }
    while (cardNumber > 0);


    long divideBy = pow(10, countDigit - 2);
    
    twoDigits = originalNum / divideBy;
    firstDigit = (twoDigits / 10);

    if (isValid)
    {

        if (firstDigit == 4 && (countDigit == 13 || countDigit == 16))
        {
            printf("VISA\n");
        }
        else if ((twoDigits == 34 || twoDigits == 37) && countDigit == 15)
        {
            printf("AMEX\n");
        }
        else if ((twoDigits > 50 && twoDigits < 56) && countDigit == 16)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

def main():
    # Global variable declarations
    cardNumber = input("Enter your credit card number: ")
    firstCopy = int(cardNumber)
    secondCopy = int(cardNumber) / 10
    
    # Checks initial validity of card number
    checkValidity(cardNumber)

    # Calculates Luhn's Algorithm for card number
    calculateLuhn(firstCopy, secondCopy)
    
    # Declares if it's AMEX\n or MASTERCARD\n or VISA\n or INVALID\n,
    declare(cardNumber)


def checkValidity(n):
    
    if len(str(n)) < 13 or len(str(n)) > 16:
        print("INVALID")
        exit(1)
    else:
        return True

    
def calculateLuhn(x, y):    
    totalSum = 0
    # Luhn's Algorithm
    # Part 1: Alternate Additions
    while True:
        currentCount = int(x % 10)
        totalSum += currentCount
        x /= 100
        if x == 0:
            break
    
    # Part 2: Other Alternate Additions
    multiplyNum = 0
    while True:
        currentCount = int(y % 10)
        multiplyNum = currentCount * 2
        totalSum = totalSum + int((multiplyNum % 10)) + int((multiplyNum / 10))
        y /= 100
        if y == 0:
            break
    
    isValid = bool(totalSum % 10 == 0)

    
def declare(n):
    # American Express uses 15-digit numbers, American Express numbers start with 34 or 37
    # MasterCard uses 16-digit numbers, and MasterCard numbers start with 51, 52, 53, 54, or 55 
    # Visa numbers start with 4 Visa uses 13- and 16-digit
    
    # Finding the first digit of the cardNumber by dividing it until we get 1s place
    firstDigit = int(n)
    while (firstDigit >= 10):
        firstDigit = int(firstDigit / 10)
    
    # Finding the last two digits of the cardNumber by dividing it until we get 10s place
    twoDigit = int(n)
    while (twoDigit >= 100):
        twoDigit = int(twoDigit / 10)
    
    digitCount = len(str(n))

    if firstDigit == 4 and (digitCount == 13 or digitCount == 16):
        print("VISA")
    elif (twoDigit == 34 or twoDigit == 37) and digitCount == 15:
        print("AMEX")
    elif (twoDigit in range(51, 56)) and digitCount == 16:
        print("MASTERCARD")
    else:
        print("INVALID")
main()


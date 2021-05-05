from cs50 import get_float

coins = 0

while True:
    dollar = get_float("Enter the amount in dollars: ")
    if dollar > 0:
        break

# Converts dollars to cents
denom = round(dollar * 100)

# Quarters
while denom >= 25:
    denom -= 25
    coins += 1
    
# Dimes
while denom >= 10:
    denom -= 10
    coins += 1

# Nickels
while denom >= 5:
    denom -= 5
    coins += 1

# Pennies
while denom >= 1:
    denom -= 1
    coins += 1

print(f"change owed: {coins}")
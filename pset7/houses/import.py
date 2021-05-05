import sys
import csv
import cs50

# Checks for valid input
if len(sys.argv) != 2:
    print("Input error")
    exit(1)

db = cs50.SQL("sqlite:///students.db")
with open(sys.argv[1], "r") as characters:
    reader = csv.DictReader(characters)

    # Iterating over CSV file:
    for row in reader:
        name = row['name'].split()
        # Takes middle name only if name has 3 words, otherwise adds NULL as input
        if len(name) == 3:
            first, middle, last = name
        else:
            first, last = name
            middle = None

        house = row['house']
        birth = row['birth']

        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", first, middle, last, house, birth)

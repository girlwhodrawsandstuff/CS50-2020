import sys
import cs50

# Checks for valid input
if len(sys.argv) != 2:
    print("Input error")
    exit(1)

house = sys.argv[-1]
db = cs50.SQL("sqlite:///students.db")

database = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", sys.argv[1])

# Reads each row in database students.db
for row in database:
    print(row['first'] + ' ' + (row['middle'] + ' ' if row['middle'] else '') + row['last'] + ', born ' + str(row['birth']))

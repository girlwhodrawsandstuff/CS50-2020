import sys
import csv


def main():
    # Checking validity of input
    if len(sys.argv) != 3:
        print("Input error")
        exit(1)
        
    # opening our csv file
    file = open(sys.argv[1], "r")
    data = csv.DictReader(file)
    # This is for reading our sequence txt file
    with open(sys.argv[2]) as f:
        sequence = f.read()
    
    # Getting the longest count of STR
    counts = {}
    for subsequence in data.fieldnames[1:]:
        counts[subsequence] = longest_match(sequence, subsequence)
    
    # Finding the matching person
    for row in data:
        if all(counts[subsequence] == int(row[subsequence]) for subsequence in counts):
            print(row["name"])
            file.close()
            return

    print("No match")    
    file.close()
 
   
def longest_match(sequence, subsequence):
    longest = 0
    length = len(subsequence)
    for i in range(len(sequence)):
        count = 0
        while True:
            start = i + length * count
            end = start + length
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break
        longest = max(longest, count)
    return longest
    

main()
# User input
text = input("Enter your text: ")

# Number of letters in given input
numLetters = 0
for char in text:
    if char.isalpha():
        numLetters += 1

# Number of words in the given
numWords = len(text.split())

# Number of sentences in the given input
numSentences = 0
for char in text:
    if char in ['?', '.', '!']:
        numSentences += 1

# average number of letters per 100 words in the text
L = (numLetters * 100) / numWords
# average number of sentences per 100 words
S = (numSentences * 100) / numWords

# Calculating Coleman-Liau index
answer = int((0.0588 * L - 0.296 * S - 15.8) + 0.5)

if answer < 1:
    print("Before Grade 1")
elif answer >= 16:
    print("Grade 16+")
else:
    print(f"Grade {answer}")
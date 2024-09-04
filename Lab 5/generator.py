def main():
    import random

    nPatterns = 1000 
    patternSize = 3 
    textSize = 10000

    alphabet = 'qwertyuiopasdfghjklzxcvbnm'
    alphabetFull = '\n' + ' ' + alphabet

    patternsLine = ""
    for i in range(nPatterns):
        curPattern = ""
        for j in range(random.randint(1, patternSize)):
            curPattern += random.choice(alphabet)
        patternsLine += curPattern + " "

    text = ""
    for i in range(textSize):
        text += random.choice(alphabetFull)

    with open("test.txt", 'w') as file:
        file.write(patternsLine + '\n')
        file.write(text)

if __name__ == "__main__":
    main()

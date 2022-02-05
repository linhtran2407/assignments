#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void playGuessWord (char word[33]) {
  int len = strlen(word) - 1; // not count last char, which is either "/0" terminator or "\n"

  // store the word being filled up after each turn
  char* currentWord = malloc(sizeof(char) * len + 1); 

  if (currentWord == NULL) { 
    printf("Cannot allocate memory. Exiting..."); 
    exit(1); 
  }
  
  // initialize the word with all '_'
  for (int i = 0; i < len; i++) {
    currentWord[i] = '_';
  }
  
  int guessedCount = 0; // number of letters having been guessed correctly
  int turnCounter = 1;

  printf("Welcome to Word Guess! \n");

  while (guessedCount < len) {
    printf("Turn: %i\n", turnCounter++);

    for (int i = 0; i < len; i++) {
      printf("%c ", currentWord[i]);
    }

    printf("\n");

    char input = 0;
    printf("Guess a character: ");
    scanf(" %c", &input);

    printf("\n");

    // store the indices at which the character matches the input
    int matchedIndices[len]; 
    int j = -1;

    // find indices at which the character matches the chosen one
    for (int i = 0; i < len; i++) {
      if (word[i] == input) {
        j++;
        matchedIndices[j] = i;
      }
    }

    // there is no match
    if (j == -1) {
      printf("Sorry, %c not found!\n\n", input);
      continue;
    }

    for (int i = 0; i <= j; i++) {
      int matchedIndex = matchedIndices[i];
      if (currentWord[matchedIndex] == '_') {
        // replace '_' by the input
        currentWord[matchedIndex] = input;
        guessedCount++;
      }
    }
  }

  for (int i = 0; i < len; i++) {
    printf("%c ", currentWord[i]);
  }

  printf("\n");

  printf("You won in %d turns!\n", (turnCounter-1));

  free(currentWord);
  currentWord = NULL;
}

int main() {
  FILE* infile = NULL;

  infile = fopen("words.txt", "r");
  // check if file has been read
  if (infile == NULL) {
    printf("Error: unable to open file");
    exit(1);
  }

  char buffer[33]; // 33 = max number of characters (32) + 1 for /0 terminator

  // get the number of words
  fgets(buffer, 33, infile);
  int numWords = atoi(buffer); // 69000

  // pick a random word index from 0 to 68999
  srand(time(0));
  int wordIndex = rand() % numWords;;
  
  int index = 0;

  // read file until reach the word
  while (fgets(buffer, 33, infile) != NULL && index < wordIndex) {  
    index++; 
  }

  printf("Chosen word is %s", buffer);
  
  // play the game!
  playGuessWord(buffer);

  fclose(infile);

  return 0;
}

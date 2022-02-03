#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // the string to be repeated, assume the length is 32
  char s[33];

  int n; // number of repetitions

  printf("Enter a word: ");
  scanf("%s", s);
  printf("Enter a count: ");
  scanf("%d", &n);

  char *output = malloc(sizeof(char)*strlen(s)*n + 1);

  // check if malloc is successful
  if (output == NULL) {
    printf("Cannot allocate new string. Exiting...\n");
    exit(1);
  }

  // create output
  for (int i = 0; i < n; i++) {
    strcat(output, s);
  }

  printf("Your word is: %s \n", output);

  free(output);
  output = NULL;

  return 0;
}

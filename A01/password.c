#include <stdio.h>
#include <string.h>

int main()
{
  char input[128];
  printf("Enter a word: ");
  scanf("%s", input);

  int n = strlen(input);

  for (int i = 0; i < n; i++)
  {
    if (input[i] == 'e')
    {
      input[i] = '3';
    }
    else if (input[i] == 'l')
    {
      input[i] = '1';
    }
    else if (input[i] == 'a')
    {
      input[i] = '@';
    }
  }

  printf("Your bad password is %s\n", input);
  return 0;
}
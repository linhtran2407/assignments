#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void play(int numRounds)
{
  char humanChoice[128];
  char choices[3][10] = {"rock", "paper", "scissors"};
  int humanPoint = 0, AIPoint = 0;
  int chooseRock, choosePaper, chooseScissor; // choice of human: is 0 for choosing, not is 0 otherwise

  for (int i = 0; i < numRounds; i++)
  {
    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", humanChoice);

    srand(time(0));
    int AIChoiceIndex = rand() % 3;
    printf("AI chose %s\n", choices[AIChoiceIndex]);

    // compare human choice with 3 choices
    chooseRock = strcmp(humanChoice, "rock");
    choosePaper = strcmp(humanChoice, "paper");
    chooseScissor = strcmp(humanChoice, "scissors");

    if (chooseRock != 0 && choosePaper != 0 && chooseScissor != 0)
    {
      printf("You entered an invalid choice: %s\n", humanChoice);
    }

    // count point
    if ((chooseRock == 0 && AIChoiceIndex == 2) || (choosePaper == 0 && AIChoiceIndex == 0) || (chooseScissor == 0 && AIChoiceIndex == 1))
    {
      humanPoint++; // human wins
    }
    else if ((chooseRock == 0 && AIChoiceIndex == 1) || (choosePaper == 0 && AIChoiceIndex == 2) || (chooseScissor == 0 && AIChoiceIndex == 0))
    {
      AIPoint++; // AI wins
    }

    // print rule - reason of winning
    if ((chooseRock == 0 && AIChoiceIndex == 2) || (chooseScissor == 0 && AIChoiceIndex == 0))
    {
      printf("Rock bashes scissors.\n");
    }
    else if ((choosePaper == 0 && AIChoiceIndex == 0) || (chooseRock == 0 && AIChoiceIndex == 1))
    {
      printf("Paper covers rock.\n");
    }
    else if ((chooseScissor == 0 && AIChoiceIndex == 1) || (choosePaper == 0 && AIChoiceIndex == 2))
    {
      printf("Scissors cut paper.\n");
    }

    printf("AI score: %i, Player score: %i\n", AIPoint, humanPoint);
  }

  if (AIPoint > humanPoint)
  {
    printf("AI wins.\n");
  }
  else if (AIPoint < humanPoint)
  {
    printf("Human wins.\n");
  }
  else
  {
    printf("There is a tie.\n");
  }
}

int main()
{
  // pick number of rounds
  printf("Welcome to Rock, Paper, Scissors!\nHow many rounds do you want to play? ");
  int numRounds;
  scanf("%i", &numRounds);

  // play with the given number of rounds
  play(numRounds);

  return 0;
}

#include <stdio.h>

struct snack
{
  char foodName[64];
  double cost;
  int quantity;
};

// handle purchase request
void buySnack(struct snack snacksArray[], double money, int snackIndex)
{
  struct snack chosenSnack = snacksArray[snackIndex];
  if (chosenSnack.quantity == 0)
  {
    printf("Sorry, we are out of %s\n", chosenSnack.foodName);
  }
  else if (chosenSnack.cost > money)
  {
    printf("You can't afford it!\n");
  }
  else
  {
    printf("You bought %s\n", chosenSnack.foodName);
    printf("You have $%.1f left\n", money - chosenSnack.cost);
  }
}

int main()
{
  const int SIZE = 3; // size of snack bar

  // initialize 3 snacks
  struct snack snacksArray[SIZE] = {
      {"Bimbim oishi", 4.5, 4},
      {"Pho ko hanh", 15.5, 6},
      {"Bun bo hue", 20.5, 0}};

  printf("Welcome to Linh's Snack Bar.\nHow much money do you have? ");
  // take money input
  double money;
  scanf("%lf", &money);
  printf("Your money: $%.1f\n", money);

  // print the snack bar
  for (int i = 0; i < SIZE; i++)
  {
    struct snack currentSnack = snacksArray[i];
    printf("%i) %s\t\t cost: $%.1f\t quantity: %i\n", i, currentSnack.foodName, currentSnack.cost, currentSnack.quantity);
  }

  // take in purchase request
  printf("What snack would you like to buy? [0,1,2] ");
  int choice;
  scanf("%i", &choice);

  // check for valid choice and handle purchase request
  if (choice < 0 || choice > SIZE)
  {
    printf("Invalid choice of snack.\n");
  }
  else
  {
    buySnack(snacksArray, money, choice);
  }

  return 0;
}

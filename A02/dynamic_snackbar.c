#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

struct snack create(char* name, float cost, int quantity) {
  struct snack s;
  strcpy(s.name, name);
  s.cost = cost;
  s.quantity = quantity;
  return s;
}

int main() {
  int numSnacks;
  printf("Enter a number of snacks: ");
  scanf("%d", &numSnacks);

  struct snack *snacks = malloc(sizeof(struct snack) * numSnacks);

  // check if malloc is successful
  if (snacks == NULL) {
    printf("Cannot allocate new snacks array. Exiting...\n");
    exit(1);
  }

  char name[32];
  float cost;
  int quantity;

  // create each snack item and add to the bar
  for (int i = 0; i < numSnacks; i++) {
    printf("Enter a name: ");
    scanf(" %s", name);
    
    printf("Enter a cost: ");
    scanf(" %g", &cost);

    printf("Enter a quantity: ");
    scanf(" %d", &quantity);

    struct snack newSnack = create (name, cost, quantity);
    snacks[i] = newSnack;
  }
  
  // print out the bar
  printf("Welcome to Linh's Snack Bar.\n");
  for (int i = 0; i < numSnacks; i++) {
    printf("%i) %-32s\t\t cost: $%-8.2f\t quantity: %-8i\n", i, snacks[i].name, snacks[i].cost, snacks[i].quantity);
  }

  free(snacks);
  snacks = NULL;

  return 0;
}

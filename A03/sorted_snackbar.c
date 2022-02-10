//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Linh Tran
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack* snacks, const char* name, int quantity, float cost) {
  struct snack* newSnack = malloc(sizeof(struct snack));
  if (newSnack == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  strcpy(newSnack->name, name);
  newSnack->quantity = quantity;
  newSnack->cost = cost;

  if (snacks == NULL) {
    return newSnack;
  }

  // insertion sort
  struct snack* prevSnack = NULL;
  for (struct snack* snack = snacks; snack != NULL; snack = snack->next) {
    int ret = strcmp(name, snack->name);
    if (ret < 0) {
      // insert new snack before the current snack
      newSnack->next = snack;

      if (prevSnack == NULL) { 
        return newSnack; 
      } else { 
        prevSnack->next = newSnack; 
        return snacks;  
      }
    }
    
    prevSnack = snack;
  }

  prevSnack->next = newSnack;

  return snacks;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  for (struct snack* n = snacks; n != NULL; n = n->next) {
    printf("Clear: %-32s\t\t cost: $%-8.2f\t quantity: %-8i\n", n->name,n->cost, n->quantity);
    free(n);
    n = NULL;
  }
}

void print(struct snack* snacks) {
  printf("Welcome to Linh's Snack Bar.\n");
  
  int i = 0;
  for (struct snack* n = snacks; n != NULL; n = n->next) {
      printf("%i) %-15s\t\t cost: $%-4.2f\t quantity: %-4i\n", i++, n->name,n->cost, n->quantity);
  }
}

int main() {
  int numSnacks;
  printf("Enter a number of snacks: ");
  scanf("%d", &numSnacks);

  struct snack* head = NULL;

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

    head = insert_sorted(head, name, quantity, cost);
  }
  print(head);
  clear(head);

  return 0;
}


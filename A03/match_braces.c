//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name: Linh Tran
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param col: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node* newNode = malloc(sizeof(struct node));

  if (newNode == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }

  newNode->sym = sym;
  newNode->linenum = line;
  newNode->colnum = col;
  newNode->next = NULL;

  if (top == NULL) {
    return newNode;
  }

  // insert front
  newNode->next = top;
  top = newNode;

  return top;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  if (top == NULL) {
    return NULL;
  }

  struct node* currTop = top;
  top = top->next;

  free(currTop);
  currTop = NULL;

  return top;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  struct node* n = top;
  while (n != NULL) {
    struct node* temp = n->next;
    free(n);
    n = temp;
  }
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  printf("Print stack: \n");
  while (top != NULL) {
      printf("Symbol: %c\t Line number: %d\t Column Number: %d\n", top->sym, top->linenum, top->colnum);
      top = top->next;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("usage: %s\n", argv[0]);
    exit(1);
  }

  FILE* infile = NULL;

  infile = fopen(argv[1], "r");
  // check if file has been read
  if (infile == NULL) {
    printf("Error: unable to open file");
    exit(1);
  }

  struct node* head = NULL;
  char currentChar; 
  int linenum = 1, colnum = 1;

  while (1) {
    currentChar = fgetc(infile);

    if (currentChar == EOF) { break; }

    if (currentChar == '\n') {
      linenum++;
      colnum = 1;
      continue;
    }

    if (currentChar == '{') {
      head = push(currentChar, linenum, colnum, head);
    } else if (currentChar == '}') {
      if (head == NULL) {
        printf("Unmatched brace on Line %d and Column %d\n", linenum, colnum);
      } else {
        printf("Found matching braces: (%d, %d) -> (%d, %d)\n", head->linenum, head->colnum, linenum, colnum);
        head = pop(head);
      }

    }

    colnum++;
  }

  // if there is any node left in the stack, then the node contains an unmatched opening bracket
  while (head != NULL) {
    printf("Unmatched brace on Line %d and Column %d\n", head->linenum, head->colnum);
    head = pop(head);
  }

  fclose(infile);

  return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main ( ) {
  void *init = sbrk(0);
  
  for (int i = 0 ; i < 10 ; i++) {
    int *memory = malloc(100);

    if (memory == NULL) {
      fprintf(stderr, "malloc failed\n");
      return(1);
    } 
    /* writing to the memory so we know it exists */
    *memory = 123;
    free(memory);

  }
  void *last = sbrk(0);
  int allocated = (int) (last - init);
  printf("The initial top of the heap is %p.\n", init);
  printf("The last top of the heap is %p.\n", last);
  printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
  
  return 0 ;
}
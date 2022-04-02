#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

struct chunk {
  int size;
  int inUse;
  struct chunk *next;
};
struct chunk *flist = NULL;

// malloc() implementation using first-fit strategy
void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }

  struct chunk *curr = flist;
  struct chunk *prev = NULL;

  // find the first-fit chunk
  while (curr != NULL) {
    if (curr->size >= size) {
      curr->inUse += size;
      // unlink the fit chunk
      if (prev != NULL) {
        prev->next = curr->next;
      } else {
        flist = curr->next;
      }

      // skip the header
      // return the data chunk to the user
      return (void *) (curr + 1); 
    } else {
      prev = curr;
      curr = curr->next;
    }
  }

  // if no fit chunk is found
  // allocate the desired size of memory and an additional header chunk
  void *memory = sbrk (size + sizeof(struct chunk));
  
  if (memory == (void *) - 1){
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk *) memory;
    cnk->size = size;
    cnk->inUse = size;
    return (void *) (cnk + 1);
  }
}

void free(void *memory) {
  if (memory != NULL) {
    // add the header to free list
    struct chunk *header = (struct chunk *) ((struct chunk *) memory - 1);
    header->inUse = 0;
    header->next = flist;
    flist = header;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  int freeCnks = 0, inuseCnks = 0, unusedMemInUsedCks = 0, totalSizeFree = 0;
  int largestUnusedMemInUsedChunk = INT_MIN, smallestUnusedMemInUsedChunk = INT_MAX;
  int freeLargestSize = INT_MIN, freeSmallestSize = INT_MAX;

  for (int i = 0; i < len; i++) {
    if(buffers[i] != NULL) {
      inuseCnks++;
      struct chunk * currUsedCnk = (struct chunk *) ((struct chunk*) buffers[i] - 1);
      int unusedMem = currUsedCnk->size - currUsedCnk->inUse;
      unusedMemInUsedCks += unusedMem;

      if (unusedMem > largestUnusedMemInUsedChunk) {
        largestUnusedMemInUsedChunk = unusedMem;
      }

      if (unusedMem < smallestUnusedMemInUsedChunk) {
        smallestUnusedMemInUsedChunk = unusedMem;
      }
    }
  }

  struct chunk *curr = flist;
  while (curr != NULL) {
    freeCnks++;
    if (curr->size > freeLargestSize) {
      freeLargestSize = curr->size;
    }
    if (curr->size < freeSmallestSize) {
      freeSmallestSize = curr->size;
    }
    totalSizeFree += curr->size;
    curr = curr->next;
  }

  printf("Total blocks: %d\t Free: %d\t Used: %d\n", freeCnks + inuseCnks, freeCnks, inuseCnks);
  printf("Internal unused: total: %d\t average: %f\t smallest: %d\t largest: %d\n", unusedMemInUsedCks, (float) unusedMemInUsedCks / inuseCnks, smallestUnusedMemInUsedChunk, largestUnusedMemInUsedChunk);
  printf("External unused: total: %d\t average: %f\t smallest: %d\t largest: %d\n", totalSizeFree, (float) totalSizeFree / freeCnks, freeSmallestSize, freeLargestSize);
}
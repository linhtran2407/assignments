#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"

// divided the decoded bits into each block of 8 digits and convert from binary to ascii var
void genResult (int index, int* decodedBits) {  
  printf("Max number of characters in the image %d \n", (index /8));
  char *result = malloc(index / 8 + 1);  
  if (result == NULL) {
    printf("Cannot allocate space for result\n");
    exit(1);
  }

  int counter = 0, resIndex = 0;
  while (counter < index) {
    int decVal = 0;
    for (int i = 0; i < 8; i++) {
      if (decodedBits[counter] == 1) {
        decVal += (1 << (7 - i));
      }
      counter++;
    }
    result[resIndex] = (char) decVal;
    printf("%c", result[resIndex]);
    resIndex++;
  }

  free(result);
  result = NULL;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s\n", argv[0]);
    exit(1);
  }

  int w, h;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);

  if (pixels == NULL) {
    printf("Error reading PPM file and creating 2d pixel array!");
    exit(1);
  }

  int *decodedBits = malloc(sizeof(int) * (w) * (h) * 3);
  if (decodedBits == NULL) {
    printf("Cannot allocate space for decodedBits\n");
    exit(1);
  }

  int index = 0; // count length of decodedBits
  unsigned int mask = 0x1;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pixels[i*w + j];
      for (int k = 0; k < 3; k++) {
        decodedBits[index++] = mask & currPixel.colors[k];
      }
    }
  }

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  genResult(index, decodedBits);

  free(pixels);
  pixels = NULL;
  free(decodedBits);
  decodedBits = NULL;

  return 0;
}


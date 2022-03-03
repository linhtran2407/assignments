#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"

// convert each block of 8 digits to ascii var
void genResult (int* numBits, int* decodedBits) {  
  printf("Max number of characters in the image %d \n", (*numBits /8));
  // char *result = malloc(*index / 8 + 1);  
  // if (result == NULL) {
  //   printf("Cannot allocate space for result\n");
  //   exit(1);
  // }
  // int resIndex = 0;

  int counter = 0; 
  while (counter < *numBits) {
    int decVal = 0;
    for (int i = 0; i < 8; i++) {
      if (decodedBits[counter] == 1) {
        decVal += (1 << (7 - i));
      }
      counter++;
    }
    printf("%c", (char) decVal);
    // resIndex++;
  }

  // free(result);
  // result = NULL;
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

  int numBits = 0; // count length of decodedBits
  unsigned int mask = 0x1;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pixels[i*w + j];
      for (int k = 0; k < 3; k++) {
        decodedBits[numBits++] = mask & currPixel.colors[k];
      }
    }
  }

  // int count = 0; // count length of decodedBits
  // int decVal = 0;
  // int line = 0;
  // unsigned int mask = 0x1;

  // for (int i = 0; i < h; i++) {
  //   for (int j = 0; j < w; j++) {
  //     for (int k = 0; k < 3; k++) {
  //       int decodedBit = mask & pixels[i*w + j].colors[k];
  //       if (decodedBit == 1) {
  //         decVal += (1 << (7 - count));
  //       }
  //       count++;
  //       if (count == 8) {
  //         if (decVal < 127) {
  //           if (decVal == 10) { printf ("line %d: ", line++);}
  //          printf("%c", (char) decVal);
  //         }
  //         decVal = 0;
  //         count = 0;
  //       }
  //     }
  //   }
  // }

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  genResult(&numBits, decodedBits);

  free(pixels);
  pixels = NULL;
  free(decodedBits);
  decodedBits = NULL;

  return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"

int calcPower (int base, int exp) {
  int res = 1;
  while (exp > 0) {
    res *= base;
    --exp;
  }
  
  return res;
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

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image %d \n", ((w*h*3) /8));
  char *message = malloc(w*h*3);
  if (message == NULL) {
    printf("Cannot allocate space for message\n");
    exit(1);
  }

  int messIndex = 0, decVal = 0, bitCounter = 0, exp = 7; 
  unsigned int mask = 0x1;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      for (int k = 0; k < 3; k++) {
        int decodedBit = mask & pixels[i*w + j].colors[k];
        bitCounter++;

        if (decodedBit == 1) {
            decVal += calcPower(2, exp);
        }
        exp--;

        if (bitCounter == 8) {
            message[messIndex++] = (char) decVal;
            decVal = 0;
            exp = 7;
            bitCounter = 0;
        }
      }
    }
  }

  printf("%s", message);

  free(pixels);
  pixels = NULL;
  free(message);
  message = NULL;

  return 0;
}

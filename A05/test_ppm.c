#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-ascii.ppm", &w, &h);

  if (pixels == NULL) {
    printf("Error reading PPM file and creating 2d pixel array!");
    exit(1);
  }
  
  // print struct ppm_pixel
  printf("Testing file feep-ascii.ppm: %d %d\n", w, h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pixels[i*w + j];
      printf("(%hhu,%hhu,%hhu) ", currPixel.red, currPixel.green, currPixel.blue);
    }
    printf("\n");
  }

  free(pixels);
  pixels = NULL;
  return 0;
}


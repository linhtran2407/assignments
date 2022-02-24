#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Invalid runtime arguments");
    exit(1);
  }

  int w, h;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);

  if (pixels == NULL) {
    printf("Error reading PPM file and creating 2d pixel array!");
    exit(1);
  }
  
  // print struct ppm_pixel
  printf("Testing file %s: %d %d\n", argv[1], w, h);
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


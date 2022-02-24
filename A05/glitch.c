#include <stdio.h>
#include "read_ppm.h"
#include <time.h>
#include <stdlib.h>

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

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);

  // implement glitch
  srand(time(0));

  // printf("Initial color %s: %d %d\n", argv[1], w, h);
  // for (int i = 0; i < h; i++) {
  //   for (int j = 0; j < w; j++) {
  //     struct ppm_pixel currPixel = pixels[i*w + j];
  //     printf("(%hhu,%hhu,%hhu) ", currPixel.red, currPixel.green, currPixel.blue);
  //   }
  //   printf("\n");
  // }
  
  // printf("\nTesting shift %s: %d %d\n", argv[1], w, h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pixels[i*w + j];
      for (int k = 0; k < 3; k++) {
        currPixel.colors[k] = currPixel.colors[k] << (rand() % 3);
      }
      pixels[i*w + j] = currPixel;
      // printf("(%hhu,%hhu,%hhu) ", currPixel.red, currPixel.green, currPixel.blue);
    }
    // printf("\n");
  }

  write_ppm("test.ppm", pixels, w, h);

  free(pixels);
  pixels = NULL;

  return 0;
}

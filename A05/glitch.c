#include <stdio.h>
#include "read_ppm.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

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

  // create filename for the glitch image
  char glitchFileName[128] = "";
  for (int i = 0; i < strlen(argv[1]); i++) {
    if (argv[1][i] == '.') {
      break;
    }
    glitchFileName[i] = argv[1][i];
  }

  strcat(glitchFileName, "-glitch.ppm");

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Writing file %s\n", glitchFileName);

  // implement glitch
  srand(time(0));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pixels[i*w + j];
      for (int k = 0; k < 3; k++) {
        currPixel.colors[k] = currPixel.colors[k] << (rand() % 2);
      }
      pixels[i*w + j] = currPixel;
    }
  }

  write_ppm(glitchFileName, pixels, w, h);

  free(pixels);
  pixels = NULL;

  return 0;
}

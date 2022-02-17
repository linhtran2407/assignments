#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
    printf("Invalid runtime arguments");
    exit(1);
  }

  int w, h;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);

  if (pixels == NULL) {
    printf("Error reading PPM file and create 2d pixel array!");
    exit(1);
  }

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pixels[i*w + j];
      int avgIntensity = ((int) currPixel.red + (int) currPixel.green + (int) currPixel.blue) / 3;
      if (avgIntensity >= 0 && avgIntensity <= 25) {
        printf("@");
      } else if (avgIntensity >= 26 && avgIntensity <= 50) { 
        printf("#");
      } else if (avgIntensity >= 51 && avgIntensity <= 75) {
        printf("%%");
      } else if (avgIntensity >= 76 && avgIntensity <= 100) {
        printf("*");
      } else if (avgIntensity >= 101 && avgIntensity <= 125) { 
        printf("o");
      } else if (avgIntensity >= 126 && avgIntensity <= 150) { 
        printf(";");
      } else if (avgIntensity >= 151 && avgIntensity <= 175) { 
        printf(":");
      } else if (avgIntensity >= 176 && avgIntensity <= 200) { 
        printf(",");
      } else if (avgIntensity >= 201 && avgIntensity <= 225) { 
        printf(".");
      } else if (avgIntensity >= 226 && avgIntensity <= 255) { 
        printf(" ");
      } 
    }
    printf("\n");
  }
  
  free(pixels);
  pixels = NULL;
  return 0;
}


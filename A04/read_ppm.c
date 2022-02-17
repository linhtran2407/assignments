#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE* infile = NULL;

  infile = fopen(filename, "r");
  if (infile == NULL) {
    printf("Error: unable to open file");
    return NULL;
  }  
  
  char currentLine[300];
  fgets(currentLine, 300, infile); // skip the line containing type of PPM

  // this line might be a comment or the dimension 
  fgets(currentLine, 300, infile);
  if (currentLine[0] == '#') {
      // skip the comment
      fgets(currentLine, 300, infile);
  }

  *w = atoi(&currentLine[0]);
  *h = atoi(&currentLine[2]);
  printf("Testing file %s: %d %d\n", filename, *w, *h);

  // skip the line of maximum color value - we can assume it's 1 byte
  fgets(currentLine, 300, infile);

  struct ppm_pixel *pixels = malloc(sizeof(struct ppm_pixel) * (*w) * (*h));
  if (pixels == NULL) {
    printf("Cannot allocate space for pixels\n");
    return NULL;
  }

  unsigned char r, g, b;
  int index = 0;
  // read the 2D raster
  while (fscanf(infile, " %hhu %hhu %hhu", &r, &g, &b) == 3) {
      struct ppm_pixel pixel = {r, g, b};
      pixels[index++] = pixel;
  }
  
  fclose(infile);

  return pixels;


}


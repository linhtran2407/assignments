#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"
#include <ctype.h>

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE* infile = NULL;

  infile = fopen(filename, "rb");
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

  // find index of the first digit of the height value
  int indexFirstDigitHeight = 0;
  for (int i = 0; i < strlen(currentLine); i++) {
    if (isspace(currentLine[i]) != 0) {
      indexFirstDigitHeight = i + 1;
      break;
    }
  }
  
  *w = atoi(&currentLine[0]);
  *h = atoi(&currentLine[indexFirstDigitHeight]);

  // skip the line of maximum color value - assume it's 1 byte
  fgets(currentLine, 300, infile);

  struct ppm_pixel *pixels = malloc(sizeof(struct ppm_pixel) * (*w) * (*h));
  if (pixels == NULL) {
    printf("Cannot allocate space for pixels\n");
    return NULL;
  }

  fread(pixels, sizeof(struct ppm_pixel), (*w) * (*h), infile); 
  
  fclose(infile);

  return pixels;
}


extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {
  FILE* fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("Error: unable to open file");
    return;
  }

  // header
  fprintf(fp, "P6\n# This is comment\n%d %d\n255\n", w, h);

  // write each color
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      struct ppm_pixel currPixel = pxs[i*w + j];
      fwrite(currPixel.colors, 1, 3, fp);
    }
  }

  fclose(fp); 
}


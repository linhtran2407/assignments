#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"

void encode (struct ppm_pixel* pxs, int bits[], int *currPixelIndex, int *currColorIndex) {
  for (int i = 0; i < 8; i++) {
    if (*currColorIndex == 3) {
      int temp = *currPixelIndex+1;
      *currPixelIndex = temp;
      *currColorIndex = 0;
    }

    if ((bits[i] == 1 && pxs[*currPixelIndex].colors[*currColorIndex] % 2 == 0) ||
       (bits[i] == 0 && pxs[*currPixelIndex].colors[*currColorIndex] % 2 == 1)) {
      if (pxs[*currPixelIndex].colors[*currColorIndex] == 255) { // avoid exceeding max val
        pxs[*currPixelIndex].colors[*currColorIndex]--;  
      } else {
        pxs[*currPixelIndex].colors[*currColorIndex]++;
      }
    }

    int temp = *currColorIndex+1;
    *currColorIndex = temp;
  }
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
  printf("Max number of characters in the image (including terminator) %d \n", ((w*h*3) /8));

  char *message = malloc(w*h*3/8+1);
  if (message == NULL) {
    printf("Cannot allocate space for message\n");
    exit(1);
  }

  printf("Enter a phrase: ");
  scanf("%[^\n]s", message); // allow spaces in message

  int currPixelIndex = 0, currColorIndex = 0;

  for (int i = 0; i < strlen(message); i++) { 
    int decVal = (int) message[i];
    int bits[8] = { 0 };
    int bitIndex = 7;

    while (decVal != 0) {
      bits[bitIndex--] = decVal % 2;
      decVal = decVal>>1;
    }

    encode (pixels, bits, &currPixelIndex, &currColorIndex);
  }

  int bits[8] = { 0 }; // terminator bits
  encode (pixels, bits, &currPixelIndex, &currColorIndex);
  
  // create filename for the encoded image
  char encodedFileName[128] = "";
  for (int i = 0; i < strlen(argv[1]); i++) {
    if (argv[1][i] == '.') {
      break;
    }
    encodedFileName[i] = argv[1][i];
  }

  strcat(encodedFileName, "-encoded.ppm");
  write_ppm (encodedFileName, pixels, w, h);

  free(pixels);
  pixels = NULL;
  free(message);
  message = NULL;
  return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"

struct ppm_pixel* genPalette (int maxIterations) {
  struct ppm_pixel *pallet = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if (pallet == NULL) {
    printf("Cannot allocate space for pallet\n");
    return NULL;
  }
  
  srand(time(0));
  // create a palette with maxIterations colors
  for (int i = 0; i < maxIterations; i++) {
    pallet[i].red = rand() % 255;
    pallet[i].green = rand() % 255;
    pallet[i].blue = rand() % 255;  
  }

  return pallet;
}

struct ppm_pixel* computeImg (struct ppm_pixel* pallet, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations) {
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  int iter;
  struct ppm_pixel color;
  struct ppm_pixel* pixels = malloc (sizeof(struct ppm_pixel) * size * size);
  if (pixels == NULL) {
    printf("Cannot allocate space for pixels\n");
    return NULL;
  }

  for (int col = 0; col < size; col++) {
    for (int row = 0; row < size; row++) {
      xfrac = (float) row / size;
      yfrac = (float) col / size;
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);

      x = 0.0;
      y = 0.0;
      iter = 0;
      while (iter < maxIterations && ((x*x + y*y) <= 2*2)) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < maxIterations) {// escaped
        color = pallet[iter];
      } else {
        color.red = 0;
        color.blue = 0;
        color.green = 0;
      }

      pixels[col*size + row] = color;
    }
  }     
    return pixels;
}



int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  double timer;
  struct timeval tstart, tend;
  time_t tvalue;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // generate pallet
  struct ppm_pixel *pallet = genPalette(maxIterations);

  // compute image
  gettimeofday(&tstart, NULL);
  struct ppm_pixel *pixels = computeImg(pallet, size, xmin, xmax, ymin, ymax, maxIterations);
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);

  // create file name and write the image
  char filename[128] = "";
  snprintf(filename, 128, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(filename, pixels, size, size);

  free(pallet);
  pallet = NULL;
  free(pixels);
  pixels = NULL;
}

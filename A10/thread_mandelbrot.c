#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>

struct thread_data { 
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int startRow;
  int endRow;
  int startCol;
  int endCol;
  struct ppm_pixel *pallet;
  struct ppm_pixel *pixels;
};

struct ppm_pixel * genPalette (int maxIterations) {
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

void *compute (void *userdata) {
  struct thread_data *data = (struct thread_data *) userdata;
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  int iter;
  struct ppm_pixel color;

  for (int col = data->startCol; col < data->endCol; col++) {
    for (int row = data->startRow; row < data->endRow; row++) {
      xfrac = (float) row / (data->size*2);
      yfrac = (float) col / (data->size*2);
      x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      y0 = data->ymin + yfrac * (data->ymax - data->ymin);

      x = 0.0;
      y = 0.0;
      iter = 0;
      while (iter < data->maxIterations && ((x*x + y*y) <= 2*2)) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < data->maxIterations) {// escaped
        color = data->pallet[iter];
      } else {
        color.red = 0;
        color.blue = 0;
        color.green = 0;
      }

      data->pixels[col*data->size*2 + row] = color;
    }
  }

  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  double timer;
  struct timeval tstart, tend;
  time_t tvalue;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // generate pallet
  struct ppm_pixel *pallet = genPalette(maxIterations);

  // compute image
  pthread_t threads[4];
  struct thread_data data[4];
  struct ppm_pixel *pixels = malloc(sizeof(struct ppm_pixel) * size * size);
  if (pixels == NULL) {
    printf("Cannot allocate space for pixels\n");
    exit(1);
  }

  // range of row and col for each child
  // {startRow, endRow, startCol, endCol}
  int rowColRange[4][4] = {  
    {0, size/2, 0, size/2} ,   
    {0, size/2, size/2, size} ,   
    {size/2, size, 0, size/2},
    {size/2, size, size/2, size}
  };

  gettimeofday(&tstart, NULL);
  for (int i = 0; i < 4; i++) {
    data[i].size = size/2;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
    data[i].startRow = rowColRange[i][0];
    data[i].endRow = rowColRange[i][1];
    data[i].startCol = rowColRange[i][2];
    data[i].endCol = rowColRange[i][3];
    data[i].pallet = pallet;
    data[i].pixels = pixels;
    pthread_create(&threads[i], NULL, compute, (void*) &data[i]);
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n", i, rowColRange[i][2], rowColRange[i][3], rowColRange[i][0], rowColRange[i][1]);
  }

  int threadResult = 1;
  for (int i = 0; i < 4; i++) {
    threadResult = pthread_join(threads[i], NULL);
    if (threadResult == 0) {
      printf("Thread %d) finished\n", i);
    }
  }

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

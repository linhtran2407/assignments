#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include <sys/wait.h>
#include <sys/time.h>
#include<stdbool.h>
#include <math.h>

static unsigned long long maxCount = 0;
static pthread_barrier_t barrier; 
static pthread_mutex_t mutex;

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
  struct ppm_pixel *pixels;
  bool *membership;
  int *counts;
};

void * start(void* userdata) {
  // perform step 1
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
        data->membership[row*data->size*2 + col] = false;
      } else {
        data->membership[row*data->size*2 + col] = true;
      }
    }
  }

  // perform step 2
  int yrow, xcol;
  for (int col = data->startCol; col < data->endCol; col++) {
    for (int row = data->startRow; row < data->endRow; row++) {
      if (data->membership[row*data->size*2 + col]) {continue;}

      xfrac = (float) row / (data->size*2);
      yfrac = (float) col / (data->size*2);
      x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      y0 = data->ymin + yfrac * (data->ymax - data->ymin);

      x = 0.0;
      y = 0.0;
      while ((x*x + y*y) < 2*2) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;

        yrow = round (data->size*2 *(y - data->ymin)/(data->ymax - data->ymin));
        xcol = round (data->size*2 *(x - data->xmin)/(data->xmax - data->xmin));

        if (yrow < 0 || yrow >= data->size*2 || xcol < 0 || xcol >= data->size*2) {continue;}
        pthread_mutex_lock(&mutex);
        data->counts[yrow*data->size*2 + xcol]++;
        if (data->counts[yrow*data->size*2 + xcol] > maxCount) { maxCount = data->counts[yrow*data->size*2 + xcol]; }
        pthread_mutex_unlock(&mutex); 
      }
    }
  }

  // use a thread barrier to wait for all threads to finish steps 1 and 2  
  // perform step 3
  pthread_barrier_wait(&barrier);
  float gamma = 0.681;
  float value;
  float factor = 1.0 / gamma;
  struct ppm_pixel wriitenColor;

  for (int col = data->startCol; col < data->endCol; col++) {
    for (int row = data->startRow; row < data->endRow; row++) {
      value = 0.0;

      if (data->counts[row*data->size*2 + col] > 0) {
        value = log(data->counts[row*data->size*2 + col]) / log (maxCount);
        value = pow(value, factor);
      }

      wriitenColor.red = value * 255;
      wriitenColor.green = value * 255;
      wriitenColor.blue = value * 255;

      data->pixels[row*data->size*2 + col] = wriitenColor;
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // compute image
  pthread_t threads[4];
  struct thread_data data[4];
  struct ppm_pixel *pixels = malloc(sizeof(struct ppm_pixel) * size * size);
  if (pixels == NULL) {
    printf("Cannot allocate space for pixels\n");
    exit(1);
  }

  bool *membership = malloc(sizeof(bool) * size * size);
  if (membership == NULL) {
    printf("Cannot allocate space for membership\n");
    exit(1);
  }

  int *counts = malloc(sizeof(int) * size * size);
  if (counts == NULL) {
    printf("Cannot allocate space for counts\n");
    exit(1);
  }

  for (int i = 0; i < size*size; i++) {
    counts[i] = 0;
  }

    // range of row and col for each child
  // {startRow, endRow, startCol, endCol}
  int rowColRange[4][4] = {  
    {0, size/2, 0, size/2} ,   
    {0, size/2, size/2, size} ,   
    {size/2, size, 0, size/2},
    {size/2, size, size/2, size}
  };
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 4);
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
    data[i].membership = membership;
    data[i].counts = counts;
    data[i].pixels = pixels;
    pthread_create(&threads[i], NULL, start, (void*) &data[i]);
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n", i, rowColRange[i][2], rowColRange[i][3], rowColRange[i][0], rowColRange[i][1]);
  }

  int threadResult = 1;
  for (int i = 0; i < 4; i++) {
    threadResult = pthread_join(threads[i], NULL);
    if (threadResult == 0) {
      printf("Thread %d) finished\n", i);
    }
  }

  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&mutex); 
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %g seconds\n", size, size, timer);

  // create file name and write the image
  char filename[128] = "";
  snprintf(filename, 128, "buddhabrot-%d-%ld.ppm", size, time(0));
  write_ppm(filename, pixels, size, size);
  
  free(counts);
  counts = NULL;
  free(membership);
  membership = NULL;
  free(pixels);
  pixels = NULL;
}

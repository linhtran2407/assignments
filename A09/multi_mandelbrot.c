#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
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

void computeImg (struct ppm_pixel* pallet, struct ppm_pixel* pixels, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, int startRow, int endRow, int startCol, int endCol) {
  float xfrac, yfrac, x0, y0, x, y, xtmp;
  int iter;
  struct ppm_pixel color;

  for (int row = startRow; row < endRow; row++) {
    for (int col = startCol; col < endCol; col++) {
      xfrac = (float) row / (size*2);
      yfrac = (float) col / (size*2);
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

      pixels[row*size*2 + col] = color;
    }
  }     
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
  int shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel* pixels = shmat(shmid, NULL, 0);
  if (pixels == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 

  gettimeofday(&tstart, NULL);

  // range of row and col for each child
  // {startRow, endRow, startCol, endCol}
  int rowColRange[4][4] = {  
    {0, size/2, 0, size/2} ,   
    {0, size/2, size/2, size} ,   
    {size/2, size, 0, size/2},
    {size/2, size, size/2, size}
  };

  for (int i = 0; i < 4; i++) {
    int pid = fork();
    if (pid == 0) {
      if (i == 0) {
        computeImg(pallet, pixels, size/2, xmin, xmax, ymin, ymax, maxIterations, rowColRange[0][0], rowColRange[0][1], rowColRange[0][2], rowColRange[0][3]);
      } else if (i == 1) {
        computeImg(pallet, pixels, size/2, xmin, xmax, ymin, ymax, maxIterations, rowColRange[1][0], rowColRange[1][1], rowColRange[1][2], rowColRange[1][3]);
      } else if (i == 2) {
        computeImg(pallet, pixels, size/2, xmin, xmax, ymin, ymax, maxIterations, rowColRange[2][0], rowColRange[2][1], rowColRange[2][2], rowColRange[2][3]);
      } else {
        computeImg(pallet, pixels, size/2, xmin, xmax, ymin, ymax, maxIterations, rowColRange[3][0], rowColRange[3][1], rowColRange[3][2], rowColRange[3][3]);
      }
      exit(0);
    } else {
      printf("Launched child process: %d\n", pid);
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d) \n", pid, rowColRange[i][2], rowColRange[i][3], rowColRange[i][0], rowColRange[i][1]);
    }
  }

  for (int i = 0; i < 4; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);

  // create file name and write the image
  char filename[128] = "";
  snprintf(filename, 128, "multi-mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(filename, pixels, size, size);

  if (shmdt(pixels) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }   

  free(pallet);
  pallet = NULL;
}

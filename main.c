#include "wbm.h"
#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

void command_line_parser(int argc, char **argv);

int main(int argc, char **argv){

  // default values

  N = 100;
  n_threads = 10;
  rho = 5.0;

  command_line_parser(argc, argv);

  printf ("rho = %f, N = %d\n", rho, N);
  
  int i, j, t;
  double** w    = (double**)malloc(N * sizeof(double*));
  
  // made up w
  for (i = 0; i < N; i++){
    w[i] = (double*)malloc(N * sizeof(double));
    for (j = 0; j < N; j++){
      if (i == j){
        w[i][j] = i;
      } 
      else{
        w[i][j] = 0.0;
      }
    }
  }

  // initialize global variabless
  blk = (block**)malloc(N * sizeof(block*));
  for (i = 0; i < N; i++){
    blk[i] = (block*)malloc(sizeof(block));
    init_block(blk[i], w[i], i);
  }
  
  xa_bar = (double*)malloc(N * sizeof(double));
  xi_bar = (double*)malloc(N * sizeof(double));
  for (i = 0; i < N; i++){
    xa_bar[i] = 0.0;
    xi_bar[i] = 0.0;
  }

  admm_parallel_v1();

  // printf("xa: ");
  double obj = 0.0;
  for (i = 0; i < N; i++){
    obj += xi_bar[i] + xa_bar[i];
  }
  double target_obj = N*(N-1)/2.0;
  double err_p = fabs((target_obj - obj)/target_obj);

  printf("objective: %f, target_obj: %f, error: %f\n", obj, target_obj, err_p);

  for (i = 0; i < N; i++){
    free_block(blk[i]);
    free(blk[i]);
  }
  free(blk);
}

void command_line_parser(int argc, char **argv){
  int index;
  int c;
  opterr = 0;
  while ((c = getopt (argc, argv, "r:n:")) != -1)
    switch (c)
    {
      case 'r':
        rho = atof(optarg);
        break;
      case 'n':
        N = atoi(optarg);
        break;
      case '?':
        if ((optopt == 'n') || (optopt == 'r'))
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                  "Unknown option character `\\x%x'.\n",
                  optopt);
      default:
        abort ();
    }
}

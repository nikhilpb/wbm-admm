#include "wbm.h"
#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

/*
 * Flag options:
 *  -n : size of the problem
 *  -t : number of threads
 *  -r : rho
 *  -b : blocks per thread
 *  -l : tolerance
 *  -m : min tolerance
 *  -w : weighted matching
 */

double** w;
int w_flag;
int ver;

void gen_w();

void command_line_parser(int argc, char **argv);

int main(int argc, char **argv){

  N = 100;
  n_threads = 10;
  rho = 5.0;
  blks_per_ad = 1;
  tol = 0.001;
  min_tol = 0.00001;
  w_flag = 0;
  ver = 1;

  command_line_parser(argc, argv);

  printf("rho = %f, N = %d, num of threads = %d, blocks_per_ad = %d\n", 
          rho, 
          N, 
          n_threads,
          blks_per_ad);

	if (w_flag){
		printf("performing weighted matching\n");
	}
	else {
		printf("performing unweighted matching\n");
	}
	printf("tol = %f, min_tol = %f\n", tol, min_tol);	
  bc = N * blks_per_ad;
  
  int i, j, t;
  
  gen_w();

  // initialize global variabless
  int adi;
  blk = (block**)malloc(bc * sizeof(block*));
  for (i = 0; i < bc; i++){
    adi = i / blks_per_ad;
    blk[i] = (block*)malloc(sizeof(block));
    init_block(blk[i], w[adi], adi);
  }
  
  xa_bar = (double*)malloc(N * sizeof(double));
  xi_bar = (double*)malloc(N * sizeof(double));
  for (i = 0; i < N; i++){
    xa_bar[i] = 0.0;
    xi_bar[i] = 0.0;
  }
	if (ver == 1){
  	admm_parallel_v1();
	}	else {
		admm_parallel_v2();
	}


  // printf("xa: ");
  double obj = 0.0;
  for (i = 0; i < N; i++){
    obj += xi_bar[i] + xa_bar[i];
  }
  double target_obj;
  if (w_flag){
    target_obj = N * (N - 1)/ 2.0;
  }
  else {
    target_obj = N;
  }
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
  while ((c = getopt (argc, argv, "r:n:t:b:wl:m:v:")) != -1)
    switch (c)
    {
      case 'r':
        rho = atof(optarg);
        break;
      case 'n':
        N = atoi(optarg);
      	break;
			case 't':
        n_threads = atoi(optarg);
        break;
      case 'b':
        blks_per_ad = atoi(optarg);
        break;
      case 'w':
        w_flag = 1;
        break;
			case 'l':
				tol = atof(optarg);
				break;
			case 'v':
				ver = atoi(optarg);
				break;
			case 'm':
				min_tol = atof(optarg);
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


void gen_w(){
  w    = (double**)malloc(N * sizeof(double*));
  int i, j;
  // made up w
  for (i = 0; i < N; i++){
    w[i] = (double*)malloc(N * sizeof(double));
    for (j = 0; j < N; j++){
      if (i == j){
        if (w_flag){
          w[i][j] = i;
        }
        else {
          w[i][j] = 1.0;
        }
      } 
      else{
        w[i][j] = 0.0;
      }
    }
  }
}

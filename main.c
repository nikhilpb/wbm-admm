#include "wbm.h"
#include "block.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
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
  printf("objective: %f\n", obj);

  for (i = 0; i < N; i++){
    free_block(blk[i]);
    free(blk[i]);
  }
  free(blk);
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wbm-aux.h"

void project(double* xab, double* xib, block* b){
  double* res = (double*)malloc(N * sizeof(double));
  int i, ic;
  int sat = 1;
  
  // check which constraints are satisfied
  for (i = 0; i < N; i++){
    res[i] = b->w[i] - xab[b->ind] - xib[i];
    if (res[i] < tol){
      b->lmbd[i] = 0.0;
    }
    else {
      sat = 0;
    }
  }
  
  // if all constraints are satisfied set x = xb and quit
  if (sat){
    for (i = 0; i < N; i++){
      b->xi[i] = xib[i];
      b->xa[i] = xab[i];
    }
    free(res);
    return;
  }

  // main loop of the projection function
  double ln, lo;
  while(1){
    // co-ordinate search
    sat = 1;
    for (i = 0; i < N; i++){
      if (b->lmbd[i] + b->lmbd_sum - res[i] < -tol){
        ic = i;
        sat = 0;
        break;
      }
      else if((b->lmbd[i] > tol) & 
              (fabs(b->lmbd[i] + b->lmbd_sum - res[i]) > tol)){
        ic = i;
        sat = 0;
        break;
      }
    }

    // if no direction exists, update x and break
    if (sat){
      for (i = 0; i < N; i++){
        b->xi[i] = xib[i] + b->lmbd[i];
        b->xa[i] = xab[i];
      }
      b->xa[b->ind] = xab[b->ind] + b->lmbd_sum;
      break;
    }

    // update lambda and sum of lambdas
    lo = b->lmbd[ic];
    ln =  (res[ic] - b->lmbd_sum + b->lmbd[ic])/2.0;
    if (ln >= 0.0){
      b->lmbd[ic] = ln;
    }
    else{
      b->lmbd[ic] = 0.0; 
    }
    b->lmbd_sum += b->lmbd[ic] - lo;
  }

  free(res);
  return;
}

void print_block(block* b){ 
  int i;

  printf("-----------\n");
  
  printf("w: ");
  for (i = 0; i < N; i++){
    printf("%f ", b->w[i]);
  }
  printf("\n");

  printf("xa: ");
  for (i = 0; i < N; i++){
    printf("%f ", b->xa[i]);
  }
  printf("\n");
  
  printf("xi: ");
  for (i = 0; i < N; i++){
    printf("%f ", b->xi[i]);
  }
  printf("\n");
  
  printf("lmbd: ");
  for (i = 0; i < N; i++){
    printf("%f ", b->lmbd[i]);
  }
  printf("\n");
  
  printf("lmbd_sum: %f\n", b->lmbd_sum);
 

  printf("ya: ");
  for (i = 0; i < N; i++){
    printf("%f ", b->ya[i]);
  }
  printf("\n");

  printf("yi: ");
  for (i = 0; i < N; i++){
    printf("%f ", b->yi[i]);
  }
  printf("\n");

  printf("ind: %d\n", b->ind);
}

void init_block(block* b, double* w, int ind){
  b->w        = (double*)malloc(N * sizeof(double));
  b->xa       = (double*)malloc(N * sizeof(double));
  b->xi       = (double*)malloc(N * sizeof(double));
  b->lmbd     = (double*)malloc(N * sizeof(double));
  b->lmbd_sum = 0.0;
  b->ya       = (double*)malloc(N * sizeof(double));
  b->yi       = (double*)malloc(N * sizeof(double));
  b->ind = ind;

  int i;
  for (i = 0; i < N; i++){
    b->w[i]    = w[i];
    b->xa[i]   = 0.0;
    b->xi[i]   = 0.0;
    b->lmbd[i] = 0.0;
    b->ya[i]   = 0.0;
    b->yi[i]   = 0.0; 
  }
}

void free_block(block* b){
  free(b->w);
  free(b->xa);
  free(b->xi);
  free(b->lmbd);
  free(b->ya);
  free(b->yi);
}




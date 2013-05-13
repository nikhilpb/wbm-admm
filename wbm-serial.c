#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "wbm-aux.h"

#define N     100
#define rho   5.0

typedef struct {
  double*   w;        // weights
  double*   xa;       // primal variable, ad
  double*   xi;       // primal variable, impression
  double*   lmbd;     // dual variables
  double    lmbd_sum; // sum of dual variables
  double*   ya;				// dual variable advertiser		  
  double*   yi;				// dual variable impression
  int       ind;			// the index of the corresponding ad
} block;

double tol = 0.001;
block** blk;
double* xa_bar;
double* xi_bar;


void project(double* xab, double* xib, block* b);

void print_block(block* b);

void init_block(block* b, double* w, int ind);

void free_block(block* b);

void admm();

int main(){
  int i, j, t;
  double** w    = (double**)malloc(N * sizeof(double*));
  
  // made up w
  for (i = 0; i < N; i++){
    w[i] = (double*)malloc(N * sizeof(double));
    for (j = 0; j < N; j++){
      if (i == j){
        w[i][j] = 1.0;
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

  clock_t begin, end;
  double time_spent;

  begin = clock();
  admm();
  end = clock();

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  // printf("xa: ");
  // for (i = 0; i < N; i++){
  //   printf("%f ", xi_bar[i]);
  // }
  // printf("\n");

  // printf("xi: ");
  // for (i = 0; i < N; i++){
  //   printf("%f ", xa_bar[i]);
  // }
  // printf("\n");

  printf("done!\ntime required is: %f seconds\n", time_spent);

  for (i = 0; i < N; i++){
    free_block(blk[i]);
    free(blk[i]);
  }
  free(blk);
}

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

void admm(){
  int i, j, t;

  double rhoi = 1.0/rho;
  double ni   = 1.0/N;

  double* xab = (double*)malloc(N * sizeof(double));
  double* xib = (double*)malloc(N * sizeof(double));
  // ADMM steps
  for (t = 0; t < 1000; t++){
    tol = tol/1.01;
    
    // projection
    for (i = 0; i < N; i++){
      for (j = 0; j < N; j++){
        xab[j] = xa_bar[j] - rhoi * (1.0 + blk[i]->ya[j]);
        xib[j] = xi_bar[j] - rhoi * (1.0 + blk[i]->yi[j]);
      }
      project(xab, xib, blk[i]);
    }

    // averaging 
    for (i = 0; i < N; i++){
      xa_bar[i] = 0.0;
      xi_bar[i] = 0.0;
      for (j = 0; j < N; j++){
        xa_bar[i] += blk[j]->xa[i];
        xi_bar[i] += blk[j]->xi[i];
      }
      xi_bar[i] = xi_bar[i] * ni;
      xa_bar[i] = xa_bar[i] * ni;
    }

    // dual update
    for (i = 0; i < N; i++){ 
      for (j = 0; j < N; j++){
        blk[i]->yi[j] += rho * (blk[i]->xi[j] - xi_bar[j]);
        blk[i]->ya[j] += rho * (blk[i]->xa[j] - xa_bar[j]);
      }
    }
  }
}

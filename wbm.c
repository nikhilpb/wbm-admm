#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "wbm-aux.h"
#include "wbm.h"

double**  xab_aux;
double**  xib_aux;

void init(){
  int i;
  xab_aux    = (double**)malloc(n_threads * sizeof(double*));
  xib_aux    = (double**)malloc(n_threads * sizeof(double*));
  for (i = 0; i < n_threads; i++){
    xab_aux[i] = (double*)malloc(N * sizeof(double));
    xib_aux[i] = (double*)malloc(N * sizeof(double));
  }
}

void admm_serial(){
  int i, j, t;
	tolerence = tol;
  init();
  // ADMM steps

  for (t = 0; t < 1000; t++){
    tolerence = tolerence/1.01;
    if (tolerence < min_tol) {
      tolerence = min_tol;
    }
    dual_update(0, N);
    project_all(0, N, 0); 
    average();
  }
}

void* admm_parallel_helper(void* tn){
  long tid = (long)tn;
  int low = tid * N / n_threads;
  int high = (tid + 1) * N / n_threads;
  dual_update(low, high);
  project_all(low, high, tid); 
}

void admm_parallel(){
  int t;
  long i;
  init();
  tolerence = tol;
  pthread_t threads[n_threads];

  for (t = 0; t < 1000; t++){
    tolerence = tolerence/1.01;
    if (tolerence < min_tol) {
      tolerence = min_tol;
    }
    for (i = 0; i < n_threads; i++){
      pthread_create(&threads[i],
                    NULL, 
                    admm_parallel_helper, 
                    (void *) i);
    }
    for (i = 0; i < n_threads; i++){
      pthread_join(threads[i], NULL);
    }
    average();
  }
}

void dual_update(int low, int high){
  int i, j;
  for (i = low; i < high; i++){ 
    for (j = 0; j < N; j++){
      blk[i]->yi[j] += rho * (blk[i]->xi[j] - xi_bar[j]);
      blk[i]->ya[j] += rho * (blk[i]->xa[j] - xa_bar[j]);
    }
  }
}

void project_all(int low, int high, long tid){
  int i, j;
  for (i = low; i < high; i++){
    for (j = 0; j < N; j++){
      xab_aux[tid][j] = xa_bar[j] - (1.0 + blk[i]->ya[j])/rho;
      xib_aux[tid][j] = xi_bar[j] - (1.0 + blk[i]->yi[j])/rho;
    }
    project(xab_aux[tid], xib_aux[tid], blk[i]);
  } 
}

void average(){
  int i, j;
  for (i = 0; i < N; i++){
    xa_bar[i] = 0.0;
    xi_bar[i] = 0.0;
    for (j = 0; j < N; j++){
      xa_bar[i] += blk[j]->xa[i];
      xi_bar[i] += blk[j]->xi[i];
    }
    xi_bar[i] = xi_bar[i] / N;
    xa_bar[i] = xa_bar[i] / N;
  }
}



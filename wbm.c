#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "wbm-aux.h"

int n_threads = 2;
pthread_barrier_t barr, all_done;

void admm_serial();

void admm_parallel();

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
  admm_parallel();
  end = clock();

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("xa: ");
  for (i = 0; i < N; i++){
    printf("%f ", xi_bar[i]);
  }
  printf("\n");

  printf("xi: ");
  for (i = 0; i < N; i++){
    printf("%f ", xa_bar[i]);
  }
  printf("\n");

  printf("solution successful.\ntime required is: %f seconds\n", time_spent);

  for (i = 0; i < N; i++){
    free_block(blk[i]);
    free(blk[i]);
  }
  free(blk);
}

void admm_serial(){
  int i, j, t;
	double tolerence = tol;
  
	double rhoi = 1.0/rho;
  double ni   = 1.0/N;

  double* xab = (double*)malloc(N * sizeof(double));
  double* xib = (double*)malloc(N * sizeof(double));
  // ADMM steps
  for (t = 0; t < 1000; t++){
    tolerence = tolerence/1.01;
    
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

void* admm_parallel_helper(void* tn){
  long tid = (long)tn;
  int i, j, t;
  double tolerence = tol;
  
  int low_ind = tid * N / n_threads;
  int high_ind = (tid + 1) * N / n_threads;

  double rhoi = 1.0/rho;
  double ni   = 1.0/N;
  double* xab = (double*)malloc(N * sizeof(double));
  double* xib = (double*)malloc(N * sizeof(double));
  // ADMM steps
  for (t = 0; t < 1000; t++){
    tolerence = tolerence/1.01;
    
    // projection
    for (i = low_ind; i < high_ind; i++){
      for (j = 0; j < N; j++){
        xab[j] = xa_bar[j] - rhoi * (1.0 + blk[i]->ya[j]);
        xib[j] = xi_bar[j] - rhoi * (1.0 + blk[i]->yi[j]);
      }
      project(xab, xib, blk[i]);
    }

    pthread_barrier_wait(&barr);

    // averaging 
    if (tid == 0){
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
    }

    pthread_barrier_wait(&barr);

    // dual update
    for (i = low_ind; i < high_ind; i++){ 
      for (j = 0; j < N; j++){
        blk[i]->yi[j] += rho * (blk[i]->xi[j] - xi_bar[j]);
        blk[i]->ya[j] += rho * (blk[i]->xa[j] - xa_bar[j]);
      }
    }
  }
  pthread_barrier_wait(&all_done);
  pthread_exit(NULL);
}

void admm_parallel(){
  pthread_t threads[n_threads];
  pthread_barrier_init(&barr, NULL, n_threads);
  pthread_barrier_init(&all_done, NULL, n_threads + 1);
  long t;
  for (t = 0; t < n_threads; t++){
    pthread_create(&threads[t],
                  NULL, 
                  admm_parallel_helper, 
                  (void *) t);
  }
  pthread_barrier_wait(&all_done);
}

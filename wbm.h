#ifndef WBM_H
#define WBM_H
#include "block.h"

int n_threads;
double rho;
int blks_per_ad;

block**   blk;
double*   xa_bar;
double*   xi_bar;

void admm_serial();

void dual_update(int low, int high);

void project_all(int low, int high, long tid);

void average();

void admm_parallel_v1();

void admm_parallel_v2();

void average_thr(int low, int high, long tid);

void aggregate();

#endif

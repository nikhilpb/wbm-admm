#ifndef WBM_H
#define WBM_H
#include "wbm-aux.h"

#define n_threads   10

block**   blk;
double*   xa_bar;
double*   xi_bar;

void admm_serial();

void dual_update(int low, int high);

void project_all(int low, int high, long tid);

void average();

void admm_parallel();

#endif

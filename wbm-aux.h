#ifndef WBM_AUX_H
#define WBM_AUX_H


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

#define N           100
#define rho         5.0
#define tol         0.0001
#define min_tol     0.00001


double tolerence; 

void project(double* xab, double* xib, block* b);

void print_block(block* b);

void init_block(block* b, double* w, int ind);

void free_block(block* b);

#endif
